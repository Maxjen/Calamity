#pragma once

#include <ft2build.h>
#include FT_FREETYPE_H

#include <unordered_map>
#include <vector>
#include <string>

#include "TextureAtlas.h"

using std::unordered_map;
using std::vector;
using std::string;

namespace clm {

struct Glyph {
	wchar_t charcode;
	size_t width;
	size_t height;
	int offsetX;
	int offsetY;
	float minU;
	float maxU;
	float minV;
	float maxV;
	float advanceX;
	float advanceY;
	unordered_map<wchar_t, float> kerning;
};

struct Font {
	TextureAtlas* textureAtlas;
	string fileName;
	vector<Glyph*> glyphs;
	float fontSize;
	//int hinting;
	//int outlineType;
	//float outlineThickness;
	//int filtering;
	//int kerning;
	//unsigned char lcdWeights[5];
	float height;
	float linegap;
	float ascender;
	float descender;
	//float underlinePosition;
	//float underlineThickness;
};

class FontManager {
private:
	void loadGlyphs(unsigned int fontId);

	size_t atlasDimension;
	float wastedCapacity;

	unordered_map<unsigned int, Font> fonts;
	unsigned int cFont;
public:
	explicit FontManager();
	~FontManager();
	void rebuild();

	unsigned int createFont(const char* fileName, float size);
	void deleteFont();
	int loadFace(FT_Library* library, FT_Face* face, const char* fileName, float size);
	void writeToPng(int fontId);

	//TextureAtlas* getFontAtlas(int fontId);
	Glyph getGlyph(unsigned int fontId, wchar_t letter);

	void bindFontAtlas(GLenum textureUnit, unsigned int fontId);
	void unbind();
};

}
