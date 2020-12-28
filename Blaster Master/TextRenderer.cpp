#include "TextRenderer.h"
#include "Debug.h"
#include <fstream>
#include "Utils.h"
#include "Textures.h"

#define FONTFILE_SECTION_UNKNOWN -1
#define FONTFILE_SECTION_TEXTURE 1
#define FONTFILE_SECTION_CHARINFO 2
#define FONTFILE_SECTION_FONTINFO 3

void TextRenderer::_Parse_Section_Char_Info(string line)
{
	vector<string> tokens = split(line);

	if (tokens[0] == "") return;

	char c = tokens[0][0];
	int left = atoi(tokens[1].c_str());
	int top = atoi(tokens[2].c_str());
	int right = atoi(tokens[3].c_str());
	int bottom = atoi(tokens[4].c_str());

	charInfo[c] = FRECT(left, top, right, bottom);
}

void TextRenderer::LoadFont(string font_filepath)
{
	DebugOut(L"[INFO] Start loading font resources from : %s \n", font_filepath);

	ifstream f;
	f.open(font_filepath);

	// current resource section flag
	int section = FONTFILE_SECTION_UNKNOWN;

	char str[2048];
	while (f.getline(str, 2048))
	{
		string line(str);

		if (line[0] == '#') continue;	// skip comment lines	

		if (line == "[TEXTURE]") { section = FONTFILE_SECTION_TEXTURE; continue; }
		if (line == "[CHARINFO]") { section = FONTFILE_SECTION_CHARINFO; continue; }
		if (line == "[FONTINFO]") { section = FONTFILE_SECTION_FONTINFO; continue; }
		if (line[0] == '[') { section = FONTFILE_SECTION_UNKNOWN; continue; }

		//
		// data section
		//
		switch (section)
		{
		case FONTFILE_SECTION_TEXTURE:
		{
			vector<string> tokens = split(line);

			if (tokens.size() < 5) break; // skip invalid lines

			int texID = atoi(tokens[0].c_str());
			wstring path = ToWSTR(tokens[1]);
			int R = atoi(tokens[2].c_str());
			int G = atoi(tokens[3].c_str());
			int B = atoi(tokens[4].c_str());

			CTextures::GetInstance()->Add(texID, path.c_str(), D3DCOLOR_XRGB(R, G, B));

			this->textureID = texID;

			break;
		}
		case FONTFILE_SECTION_FONTINFO:
		{
			vector<string> tokens = split(line);

			if (tokens[0] == "") break;

			fontInfo.witdh = atoi(tokens[0].c_str());
			fontInfo.height = atoi(tokens[1].c_str());
			fontInfo.character_space_width = atof(tokens[2].c_str());
			fontInfo.word_space_width = atof(tokens[3].c_str());
			fontInfo.line_spacing_value = atof(tokens[4].c_str());

			break;
		}
		case FONTFILE_SECTION_CHARINFO: _Parse_Section_Char_Info(line); break;
		}
	}

	f.close();

	DebugOut(L"[INFO] Done loading scene resources %s\n", font_filepath);
}


void TextRenderer::SetContent(string value)
{
	content = value;
}

void TextRenderer::LoadContent(string filepath)
{
	content.clear();

	ifstream f;
	f.open(filepath);

	string buffer;
	
	while (std::getline(f, buffer))
	{
		content += (buffer + "\n");
	}
}

/// <summary>
/// Render the content into the screen
/// </summary>
/// <param name="x">start position of the first character: x</param>
/// <param name="y">start position of the first character: y</param>
void TextRenderer::Render(float x, float y)
{
	float current_x = x;
	float current_y = y;

	for (int i = 0; i < content.length(); i++)
	{
		if (content[i] == '\n')
		{
			current_x = x;
			current_y += fontInfo.height + fontInfo.line_spacing_value;
			continue;
		}

		if (content[i] == ' ')
		{
			current_x += fontInfo.witdh + fontInfo.word_space_width;
			continue;
		}

		if (!charInfo.contains(content[i])) continue; // if we dont have info about that weird character, skip it

		FRECT cRect = charInfo[content[i]];
		auto texture = CTextures::GetInstance()->Get(this->textureID);

		CGame::GetInstance()->Draw(current_x, current_y, texture, cRect.left, cRect.top, cRect.right, cRect.bottom);

		current_x += fontInfo.witdh + fontInfo.character_space_width;
	}
}

void TextRenderer::Render(float x, float y, string content)
{
	float current_x = x;
	float current_y = y;

	for (int i = 0; i < content.length(); i++)
	{
		if (!charInfo.contains(content[i])) continue; // if we dont have info about that weird character, skip it

		if (content[i] == '\n')
		{
			current_x = x;
			current_y += fontInfo.height + fontInfo.line_spacing_value;
			continue;
		}

		if (content[i] == ' ')
		{
			current_x += fontInfo.witdh + fontInfo.word_space_width;
			continue;
		}

		FRECT cRect = charInfo[content[i]];
		auto texture = CTextures::GetInstance()->Get(this->textureID);

		CGame::GetInstance()->Draw(current_x, current_y, texture, cRect.left, cRect.top, cRect.right, cRect.bottom);

		if (i != content.length() - 1) // only calculate next position when this is not the last character
		{
			int currentLetterWidth = cRect.right - cRect.left;

			cRect = charInfo[content[i + 1]];

			int nextLetterWidth = cRect.right - cRect.left;

			current_x += (float)currentLetterWidth/2 + (float)nextLetterWidth/2 + fontInfo.character_space_width;
		}
	}
}
