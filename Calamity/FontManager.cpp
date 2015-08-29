#include "FontManager.h"

#undef __FTERRORS_H__
#define FT_ERRORDEF( e, v, s )  { e, s },
#define FT_ERROR_START_LIST     {
#define FT_ERROR_END_LIST       { 0, 0 } };
const struct {
	int          code;
	const char*  message;
} FT_Errors[] =
#include FT_ERRORS_H

#include <assert.h>

namespace clm {

FontManager::FontManager() :
	atlasDimension(512),
	wastedCapacity(0),

	fonts(),
	cFont(0)
{}

FontManager::~FontManager() {
	//delete textureAtlas;
	// TODO
	// delete glyphs
}

void FontManager::rebuild() {

}

unsigned int FontManager::createFont(const char* fileName, float fontSize) {
	assert(fileName);
	assert(fontSize);

	FT_Library library;
	FT_Face face;
	FT_Size_Metrics metrics;

	if (loadFace(&library, &face, fileName, fontSize * 100) != 0) {
		return -1;
	}

	/*FT_Error error;

	// Initialize library
	error = FT_Init_FreeType(&library);
	if (error) {
		fprintf(stderr, "FT_Error (line %d, code 0x%02x) : %s\n", __LINE__, FT_Errors[error].code, FT_Errors[error].message);
		return -1;
	}

	// Load face
	error = FT_New_Face(library, fileName, 0, &face);
	if (error) {
		fprintf(stderr, "FT_Error (line %d, code 0x%02x) : %s\n", __LINE__, FT_Errors[error].code, FT_Errors[error].message);
		FT_Done_FreeType(library);
		return -1;
	}

	// Select charmap
	error = FT_Select_Charmap(face, FT_ENCODING_UNICODE);
	if (error) {
		fprintf(stderr, "FT_Error (line %d, code 0x%02x) : %s\n", __LINE__, FT_Errors[error].code, FT_Errors[error].message);
		FT_Done_Face(face);
		FT_Done_FreeType(library);
		return -1;
	}

	// Set char size
	//error = FT_Set_Char_Size(face, (int)(fontSize*64), 0, 72*hres, 72);
	error = FT_Set_Char_Size(face, (int)(fontSize*64), 0, 72, 0);
	if (error) {
		fprintf(stderr, "FT_Error (line %d, code 0x%02x) : %s\n", __LINE__, FT_Errors[error].code, FT_Errors[error].message);
		FT_Done_Face(face);
		FT_Done_FreeType(library);
		return -1;
	}*/

	metrics = face->size->metrics;

	Font& font = fonts[cFont];
	font.textureAtlas = new TextureAtlas(atlasDimension, atlasDimension, 1);
	font.fileName = fileName;
	font.fontSize = fontSize;
	font.height = (metrics.height >> 6) / 100.0;
	font.ascender = (metrics.ascender >> 6) / 100.0;
	font.descender = (metrics.descender >> 6) / 100.0;
	font.linegap = font.height - font.ascender + font.descender;


	FT_Done_Face(face);
	FT_Done_FreeType(library);

	// Set transform matrix
	//FT_Set_Transform(face, &matrix, NULL);


	/*self->outline_type = 0;
	self->outline_thickness = 0.0;
	self->hinting = 1;
	self->kerning = 1;
	self->filtering = 1;
	// FT_LCD_FILTER_LIGHT   is (0x00, 0x55, 0x56, 0x55, 0x00)
	// FT_LCD_FILTER_DEFAULT is (0x10, 0x40, 0x70, 0x40, 0x10)
	self->lcd_weights[0] = 0x10;
	self->lcd_weights[1] = 0x40;
	self->lcd_weights[2] = 0x70;
	self->lcd_weights[3] = 0x40;
	self->lcd_weights[4] = 0x10;*/

	/*vertex.x = x;
	vertex.y = y;
	vertex.u = u;
	vertex.v = v;
	vertex.r = r;
	vertex.g = g;
	vertex.b = b;
	vertex.a = a;
	vertex.textureId = textureId;*/
	loadGlyphs(cFont);
	return cFont++;
}

void FontManager::loadGlyphs(unsigned int fontId) {
	FT_Library library;
	FT_Face face;
	//FT_Glyph ftGlyph;
	/*FT_GlyphSlot slot;
	FT_Bitmap ftBitmap;
	FT_Error error;

	Glyph* glyph;
	AtlasRegion region;*/

	if (loadFace(&library, &face, fonts[fontId].fileName.c_str(), fonts[fontId].fontSize) != 0) {
		return;
	}

	vector<unsigned int> glyphsToLoad;
	for (unsigned int i = 0; i < 128; ++i)
		glyphsToLoad.push_back(i);
	glyphsToLoad.push_back(9654);
	glyphsToLoad.push_back(9660);

	//for (unsigned int i = 0; i < 128; ++i) {
	for (auto& it : glyphsToLoad) {
		FT_GlyphSlot slot;
		FT_Bitmap ftBitmap;
		FT_Error error;

		FT_Int32 flags = 0;
		int ftBitmapWidth = 0;
		int ftBitmapRows = 0;
		//int ftBitmapPitch = 0;
		int ftGlyphTop = 0;
		int ftGlyphLeft = 0;

		FT_UInt glyphIndex = FT_Get_Char_Index(face, it);

		flags |= FT_LOAD_RENDER;
		//flags |= FT_LOAD_FORCE_AUTOHINT;

		error = FT_Load_Glyph(face, glyphIndex, flags);
		if (error) {
			fprintf(stderr, "FT_Error (line %d, code 0x%02x) : %s\n", __LINE__, FT_Errors[error].code, FT_Errors[error].message);
			FT_Done_Face(face);
			FT_Done_FreeType(library);
			return;
		}

		slot = face->glyph;
		ftBitmap = slot->bitmap;
		ftBitmapWidth = slot->bitmap.width;
		ftBitmapRows = slot->bitmap.rows;
		//ftBitmapPitch = slot->bitmap.pitch;
		ftGlyphTop = slot->bitmap_top;
		ftGlyphLeft = slot->bitmap_left;

		int depth = 1;
		int w = ftBitmapWidth/depth + 1;
		int h = ftBitmapRows + 1;
		AtlasRegion region = fonts[fontId].textureAtlas->getRegion(w, h);
		if (region.x < 0) {
			fprintf(stderr, "Texture atlas is full (line %d)\n", __LINE__);
			continue;
		}
		w -= 1;
		h -= 1;
		//printf("%d", ftBitmap.buffer[0]); //crashes
		fonts[fontId].textureAtlas->setRegion(region.x, region.y, w, h, ftBitmap.buffer, ftBitmap.pitch);
		Glyph* glyph = new Glyph();
		glyph->charcode = it;
		glyph->width = w;
		glyph->height = h;
		glyph->offsetX = ftGlyphLeft;
		glyph->offsetY = ftGlyphTop;
		glyph->minU = region.x / (float)atlasDimension;
		glyph->minV = region.y / (float)atlasDimension;
		glyph->maxU = (region.x + glyph->width) / (float)atlasDimension;
		glyph->maxV = (region.y + glyph->height) / (float)atlasDimension;

		// Discard hinting to get advance
		FT_Load_Glyph(face, glyphIndex, FT_LOAD_RENDER | FT_LOAD_NO_HINTING);
		slot = face->glyph;
		glyph->advanceX = slot->advance.x/64.0;
		glyph->advanceY = slot->advance.y/64.0;
		fonts[fontId].glyphs.push_back(glyph);
	}

	for (auto& it1 : fonts[fontId].glyphs) {
		FT_UInt glyphIndex = FT_Get_Char_Index(face, it1->charcode);
		for (auto& it2 : fonts[fontId].glyphs) {
			FT_UInt prevGlyphIndex = FT_Get_Char_Index(face, it2->charcode);
			FT_Vector kerning;
			FT_Get_Kerning(face, prevGlyphIndex, glyphIndex, FT_KERNING_UNFITTED, &kerning);
			if (kerning.x) {
				it1->kerning[it2->charcode] =  kerning.x / (float)(64.0f*64.0f);
				//printf("%f\n", it1->kerning[it2->charcode]);
			}
		}
	}

	FT_Done_Face(face);
	FT_Done_FreeType(library);
	fonts[fontId].textureAtlas->upload();
	//texture_font_generate_kerning( self );

	/*size_t i, x, y, width, height, depth, w, h;
	FT_Library library;
	FT_Error error;
	FT_Face face;
	FT_Glyph ft_glyph;
	FT_GlyphSlot slot;
	FT_Bitmap ft_bitmap;

	FT_UInt glyph_index;
	texture_glyph_t *glyph;
	ivec4 region;
	size_t missed = 0;

	assert( self );
	assert( charcodes );


	width  = self->atlas->width;
	height = self->atlas->height;
	depth  = self->atlas->depth;*/
}

int FontManager::loadFace(FT_Library* library, FT_Face* face, const char* fileName, float size) {
	FT_Error error;

	// Initialize library
	error = FT_Init_FreeType(library);
	if (error) {
		fprintf(stderr, "FT_Error (line %d, code 0x%02x) : %s\n", __LINE__, FT_Errors[error].code, FT_Errors[error].message);
		return -1;
	}

	// Load face
	error = FT_New_Face(*library, fileName, 0, face);
	if (error) {
		fprintf(stderr, "FT_Error (line %d, code 0x%02x) : %s\n", __LINE__, FT_Errors[error].code, FT_Errors[error].message);
		FT_Done_FreeType(*library);
		return -1;
	}

	// Select charmap
	error = FT_Select_Charmap(*face, FT_ENCODING_UNICODE);
	if (error) {
		fprintf(stderr, "FT_Error (line %d, code 0x%02x) : %s\n", __LINE__, FT_Errors[error].code, FT_Errors[error].message);
		FT_Done_Face(*face);
		FT_Done_FreeType(*library);
		return -1;
	}

	// Set char size
	//error = FT_Set_Char_Size(face, (int)(fontSize*64), 0, 72*hres, 72);
	error = FT_Set_Char_Size(*face, (int)(size * 64), 0, 72, 0);
	if (error) {
		fprintf(stderr, "FT_Error (line %d, code 0x%02x) : %s\n", __LINE__, FT_Errors[error].code, FT_Errors[error].message);
		FT_Done_Face(*face);
		FT_Done_FreeType(*library);
		return -1;
	}

	return 0;
}

void FontManager::writeToPng(int fontId) {
	fonts[fontId].textureAtlas->writeToPng("Font.png");
}

/*TextureAtlas* FontManager::getFontAtlas(int fontId) {
	return fonts[fontId].textureAtlas;
}*/

Glyph FontManager::getGlyph(unsigned int fontId, wchar_t letter) {
	Glyph result;
	result.charcode = -1;
	//if (letter >= 0 && letter <= 128) {
		for (auto& it : fonts[fontId].glyphs) {
			if (it->charcode == letter) {
				result = *it;
				break;
			}
		}
	//}
	return result;
}

void FontManager::bindFontAtlas(GLenum textureUnit, unsigned int fontId) {
	fonts[fontId].textureAtlas->bind(textureUnit);
}

void FontManager::unbind() {
	glBindTexture(GL_TEXTURE_2D, 0);
}

}
