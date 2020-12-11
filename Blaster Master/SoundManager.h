#pragma once
#include <xaudio2.h>
#include <string>

#ifdef _XBOX //Big-Endian
#define fourccRIFF 'RIFF'
#define fourccDATA 'data'
#define fourccFMT 'fmt '
#define fourccWAVE 'WAVE'
#define fourccXWMA 'XWMA'
#define fourccDPDS 'dpds'
#endif

#ifndef _XBOX //Little-Endian
#define fourccRIFF 'FFIR'
#define fourccDATA 'atad'
#define fourccFMT ' tmf'
#define fourccWAVE 'EVAW'
#define fourccXWMA 'AMWX'
#define fourccDPDS 'sdpd'
#endif

class SoundManager
{
private:
	IXAudio2* pXAudio2 = NULL;
	IXAudio2MasteringVoice* pMasterVoice = NULL;
	IXAudio2SourceVoice* pSourceVoice;

	static SoundManager* __instance;

	std::string DefaultSoundPath = "Sound\\";

private:
	SoundManager();
	HRESULT FindChunk(HANDLE hFile, DWORD fourcc, DWORD& dwChunkSize, DWORD& dwChunkDataPosition);
	HRESULT ReadChunkData(HANDLE hFile, void* buffer, DWORD buffersize, DWORD bufferoffset);
	HRESULT LoadAudioDataFile(std::string filename, WAVEFORMATEXTENSIBLE& wfx, XAUDIO2_BUFFER& buffer);

public:
	static SoundManager* GetInstance();

	void PlaySoundW(std::string filename);
	void PlaySoundInfinite(std::string filename);
	void Release();
};