#include "SoundManager.h"
#include "Debug.h"
#include "Utils.h"

SoundManager* SoundManager::__instance;

SoundManager::SoundManager()
{
	HRESULT hr;
	if (FAILED(hr = XAudio2Create(&pXAudio2, 0, XAUDIO2_DEFAULT_PROCESSOR)))
	{
		DebugOut(L"Create XAudio2 Fail: %d\n", hr);
	}

	if (FAILED(hr = pXAudio2->CreateMasteringVoice(&pMasterVoice)))
	{
		DebugOut(L"Create MasterVoice Fail: %d\n", hr);
	}
}

HRESULT SoundManager::FindChunk(HANDLE hFile, DWORD fourcc, DWORD& dwChunkSize, DWORD& dwChunkDataPosition)
{
    HRESULT hr = S_OK;
    if (INVALID_SET_FILE_POINTER == SetFilePointer(hFile, 0, NULL, FILE_BEGIN))
        return HRESULT_FROM_WIN32(GetLastError());

    DWORD dwChunkType;
    DWORD dwChunkDataSize;
    DWORD dwRIFFDataSize = 0;
    DWORD dwFileType;
    DWORD bytesRead = 0;
    DWORD dwOffset = 0;

    while (hr == S_OK)
    {
        DWORD dwRead;
        if (0 == ReadFile(hFile, &dwChunkType, sizeof(DWORD), &dwRead, NULL))
            hr = HRESULT_FROM_WIN32(GetLastError());

        if (0 == ReadFile(hFile, &dwChunkDataSize, sizeof(DWORD), &dwRead, NULL))
            hr = HRESULT_FROM_WIN32(GetLastError());

        switch (dwChunkType)
        {
        case fourccRIFF:
            dwRIFFDataSize = dwChunkDataSize;
            dwChunkDataSize = 4;
            if (0 == ReadFile(hFile, &dwFileType, sizeof(DWORD), &dwRead, NULL))
                hr = HRESULT_FROM_WIN32(GetLastError());
            break;

        default:
            if (INVALID_SET_FILE_POINTER == SetFilePointer(hFile, dwChunkDataSize, NULL, FILE_CURRENT))
                return HRESULT_FROM_WIN32(GetLastError());
        }

        dwOffset += sizeof(DWORD) * 2;

        if (dwChunkType == fourcc)
        {
            dwChunkSize = dwChunkDataSize;
            dwChunkDataPosition = dwOffset;
            return S_OK;
        }

        dwOffset += dwChunkDataSize;

        if (bytesRead >= dwRIFFDataSize) return S_FALSE;

    }

    return S_OK;
}

HRESULT SoundManager::ReadChunkData(HANDLE hFile, void* buffer, DWORD buffersize, DWORD bufferoffset)
{
    HRESULT hr = S_OK;
    if (INVALID_SET_FILE_POINTER == SetFilePointer(hFile, bufferoffset, NULL, FILE_BEGIN))
        return HRESULT_FROM_WIN32(GetLastError());
    DWORD dwRead;
    if (0 == ReadFile(hFile, buffer, buffersize, &dwRead, NULL))
        hr = HRESULT_FROM_WIN32(GetLastError());
    return hr;
}

HRESULT SoundManager::LoadAudioDataFile(string filename, WAVEFORMATEXTENSIBLE& wfx, XAUDIO2_BUFFER& buffer)
{

    // Open the audio file with CreateFile
    HANDLE hFile = CreateFileA(
        filename.c_str(),
        GENERIC_READ,
        FILE_SHARE_READ,
        NULL,
        OPEN_EXISTING,
        0,
        NULL);

    if (INVALID_HANDLE_VALUE == hFile)
        return HRESULT_FROM_WIN32(GetLastError());

    if (INVALID_SET_FILE_POINTER == SetFilePointer(hFile, 0, NULL, FILE_BEGIN))
        return HRESULT_FROM_WIN32(GetLastError());

    // Locate the 'RIFF' chunk in the audio file, and check the file type.
    DWORD dwChunkSize;
    DWORD dwChunkPosition;
    //check the file type, should be fourccWAVE or 'XWMA'
    FindChunk(hFile, fourccRIFF, dwChunkSize, dwChunkPosition);
    DWORD filetype;
    ReadChunkData(hFile, &filetype, sizeof(DWORD), dwChunkPosition);
    if (filetype != fourccWAVE)
        return S_FALSE;

    // Locate the 'fmt ' chunk, and copy its contents into a WAVEFORMATEXTENSIBLE structure.
    FindChunk(hFile, fourccFMT, dwChunkSize, dwChunkPosition);
    ReadChunkData(hFile, &wfx, dwChunkSize, dwChunkPosition);

    // Locate the 'data' chunk, and read its contents into a buffer.
    //fill out the audio data buffer with the contents of the fourccDATA chunk
    FindChunk(hFile, fourccDATA, dwChunkSize, dwChunkPosition);
    BYTE* pDataBuffer = new BYTE[dwChunkSize];
    ReadChunkData(hFile, pDataBuffer, dwChunkSize, dwChunkPosition);

    // Populate an XAUDIO2_BUFFER structure
    buffer.AudioBytes = dwChunkSize;  //size of the audio buffer in bytes
    buffer.pAudioData = pDataBuffer;  //buffer containing audio data
    buffer.Flags = XAUDIO2_END_OF_STREAM; // tell the source voice not to expect any data after this buffer

    return S_OK;
}

SoundManager* SoundManager::GetInstance()
{
	if (__instance == NULL)
	{
		__instance = new SoundManager();
	}
	return __instance;
}

void SoundManager::PlaySoundW(std::string filename)
{
    WAVEFORMATEXTENSIBLE wfx = { 0 };
    XAUDIO2_BUFFER buffer = { 0 };

    HRESULT hr;
    buffer.LoopCount = XAUDIO2_NO_LOOP_REGION;
    hr = LoadAudioDataFile(DefaultSoundPath + filename, wfx, buffer);
    if (hr != S_OK)
    {
        DebugOut(L"Fail to load Audio Data file: %d\n", hr);
        return;
    }
    if (FAILED(hr = pXAudio2->CreateSourceVoice(&pSourceVoice, (WAVEFORMATEX*)&wfx)))
    {
        DebugOut(L"Fail to create Source Voice: %d\n", hr);
    }

    if (FAILED(hr = pSourceVoice->SubmitSourceBuffer(&buffer)))
    {
        DebugOut(L"Fail to submit Source Voice: %d\n", hr);
    }

    if (FAILED(hr = pSourceVoice->Start(0)))
    {
        DebugOut(L"Fail to start Source Voice: %d\n", hr);
    }
    
}
void SoundManager::PlaySoundInfinite(std::string filename)
{
    WAVEFORMATEXTENSIBLE wfx = { 0 };
    XAUDIO2_BUFFER buffer = { 0 };

    HRESULT hr;
    buffer.LoopCount = XAUDIO2_LOOP_INFINITE;
    hr = LoadAudioDataFile(DefaultSoundPath + filename, wfx, buffer);
    if (hr != S_OK)
    {
        DebugOut(L"Fail to load Audio Data file: %d\n", hr);
        return;
    }

    if (FAILED(hr = pXAudio2->CreateSourceVoice(&pSourceVoice, (WAVEFORMATEX*)&wfx)))
    {
        DebugOut(L"Fail to create Source Voice: %d\n", hr);
    }

    if (FAILED(hr = pSourceVoice->SubmitSourceBuffer(&buffer)))
    {
        DebugOut(L"Fail to submit Source Voice: %d\n", hr);
    }

    if (FAILED(hr = pSourceVoice->Start(0)))
    {
        DebugOut(L"Fail to start Source Voice: %d\n", hr);
    }
    if (FAILED(hr = pSourceVoice->SetVolume(0.5)))
    {
        DebugOut(L"Fail to start Source Voice: %d\n", hr);
    }
}

void SoundManager::StopSource()
{
    if (pSourceVoice != NULL)
    {
        pSourceVoice->Stop();
    }
}
void SoundManager::Release()
{
    if (pSourceVoice != NULL)
    {
        pSourceVoice->Stop();
    }

    if (pMasterVoice != NULL)
    {
        pMasterVoice->DestroyVoice();
    }

    if (pXAudio2 != NULL)
    {
        pXAudio2->Release();
    }
}
