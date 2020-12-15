#pragma once
#include "Game.h"
#include <string>
#include "RECT.h"

struct FontInfo
{
	int witdh;
	int height;
	float character_space_width; // the space between 2 character
	float word_space_width; // the space between 2 word
	float line_spacing_value; // the space between 2 line
};

class TextRenderer
{
private:
	string font_filepath;
	string content;

	int textureID;

	unordered_map<char, FRECT> charInfo;
	FontInfo fontInfo;

private:
	void _Parse_Section_Char_Info(string line);
public:
	void LoadFont(string font_filepath);
	void SetContent(string value);
	void LoadContent(string filepath);
	void Render(float x, float y);
};