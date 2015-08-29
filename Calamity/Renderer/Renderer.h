#pragma once

#include <unordered_map>
#include <vector>

//#define GLM_FORCE_RADIANS
#include <glm/gtx/transform.hpp>

#include "Math/Vec2.h"

#include "../TextureManager.h"
#include "../Shader/SpriteShader.h"
#include "../Shader/TextureShader.h"

#include "../FontManager.h"
#include "../Shader/FontShader.h"

#include "../Shader/ColorShader.h"

using std::unordered_map;
using std::vector;

namespace clm {

// struct for uploading to vertex buffer (doesn't need textureId)
struct SpriteVertexSimple {
	float x, y;
	float u, v;
	char r, g, b, a;
};

struct SpriteVertex {
	float x, y;
	float u, v;
	char r, g, b, a;
	int textureId;
};

// struct for uploading to vertex buffer (doesn't need textureId)
struct TextureVertexSimple {
	float x, y;
	float u, v;
	char r, g, b, a;
	float texOriginU, texOriginV;
	float texWidth, texHeight;
};

struct TextureVertex {
	float x, y;
	float u, v;
	char r, g, b, a;
	float texOriginU, texOriginV;
	float texWidth, texHeight;
	int textureId;
};


struct ColorVertex {
	float x, y;
	char r, g, b, a;
};

// TODO: clear up difference when used for sprites and letters
struct VerticesWithAtlas {
	unsigned int atlasId; // or fontId
	int numVertices; // or numLetters
};

class SpriteLayer {
public:
	GLuint vertexBufferId;
	GLuint indexBufferId;
	GLuint vaoId;

	unordered_map<unsigned int, int> indices;
	vector<SpriteVertexSimple> vertexBufferData;
	vector<int> indexBufferData;

	vector<VerticesWithAtlas> verticesWithAtlas;

	~SpriteLayer();
	void uploadBuffers();
};

class TextureTriangleLayer {
public:
	GLuint vertexBufferId;
	GLuint indexBufferId;
	GLuint vaoId;

	unordered_map<unsigned int, int> indices;
	vector<TextureVertexSimple> vertexBufferData;
	vector<int> indexBufferData;

	vector<VerticesWithAtlas> verticesWithAtlas;

	~TextureTriangleLayer();
	void uploadBuffers();
};

class FontLayer {
public:
	GLuint vertexBufferId;
	GLuint indexBufferId;
	GLuint vaoId;

	//unordered_map<int, int> indices;
	int cIndex;
	vector<SpriteVertexSimple> vertexBufferData;
	vector<int> indexBufferData;

	vector<VerticesWithAtlas> lettersWithFont;

	~FontLayer();
	void uploadBuffers();
};

class LineLayer {
public:
	GLuint vertexBufferId;
	GLuint indexBufferId;
	GLuint vaoId;

	unordered_map<unsigned int, int> indices;
	vector<ColorVertex> vertexBufferData;
	vector<int> indexBufferData;

	~LineLayer();
	void uploadBuffers();
};

class TriangleLayer {
public:
	GLuint vertexBufferId;
	GLuint indexBufferId;
	GLuint vaoId;

	unordered_map<unsigned int, int> indices;
	vector<ColorVertex> vertexBufferData;
	vector<int> indexBufferData;

	~TriangleLayer();
	void uploadBuffers();
};

class Renderer {
private:
	TextureManager* textureManager;
	SpriteShader* spriteShader;
	TextureShader* textureShader;

	FontManager* fontManager;
	FontShader* fontShader;

	ColorShader* colorShader;

	unordered_map<unsigned int, SpriteVertex> spriteVertices;
	unsigned int cSpriteVertex;

	unordered_map<unsigned int, TextureVertex> textureVertices;
	unsigned int cTextureVertex;

	unordered_map<unsigned int, ColorVertex> colorVertices;
	unsigned int cColorVertex;

	void extractSpriteVertexSimple(SpriteVertexSimple& result, unsigned int v);
	void extractTextureVertexSimple(TextureVertexSimple& result, unsigned int v);

public:
	explicit Renderer(TextureManager* textureManager, FontManager* fontManager, SpriteShader* spriteShader, TextureShader* textureShader, FontShader* fontShader, ColorShader* colorShader);

	void setSpriteShaderViewPos(float viewPosX, float viewPosY);
	void setTextureShaderViewPos(float viewPosX, float viewPosY);
	void setColorShaderViewPos(float viewPosX, float viewPosY);

	TextureManager* getTextureManager();
	FontManager* getFontManager();

	void enableBlending();
	void setBlendFunc(GLenum sFactor, GLenum dFactor);
	void disableBlending();


	unsigned int addSpriteVertex(float x, float y, float u, float v, char r, char g, char b, char a, int textureId);
	void removeSpriteVertex(unsigned int v);
	void setSpriteVertexPosition(unsigned int v, float x, float y);
	Vec2 getSpriteVertexPosition(unsigned int v);

	void initSpriteLayer(SpriteLayer* spriteLayer);
	void addSpriteTriangleToLayer(SpriteLayer* spriteLayer, unsigned int v1, unsigned int v2, unsigned int v3);
	void drawSpriteLayer(SpriteLayer* spriteLayer);


	unsigned int addTextureVertex(float x, float y, float u, float v, char r, char g, char b, char a, int textureId);
	void removeTextureVertex(unsigned int v);
	void setTextureVertexPosition(unsigned int v, float x, float y);
	void setTextureVertexUV(unsigned int v, float texU, float texV);
	void setTextureVertexColor(unsigned int v, char r, char g, char b, char a);

	void initTextureTriangleLayer(TextureTriangleLayer* textureTriangleLayer);
	void addTextureTriangleToLayer(TextureTriangleLayer* textureTriangleLayer, unsigned int v1, unsigned int v2, unsigned int v3);
	void drawTextureTriangleLayer(TextureTriangleLayer* textureTriangleLayer);


	void writeTextureAtlasesToPng();


	void initFontLayer(FontLayer* fontLayer);
	void addLetterToLayer(FontLayer* fontLayer, wchar_t letter, float x, float y, char r, char g, char b, char a, unsigned int fontId);
	void drawFontLayer(FontLayer* fontLayer);
	//void removeLetter(int letterId);
	//void setLetterPosition(int letterId, float x, float y);


	unsigned int addColorVertex(float x, float y, char r, char g, char b, char a);
	void removeColorVertex(unsigned int v);
	void setColorVertexPosition(unsigned int v, float x, float y);
	void setColorVertexColor(unsigned int v, char r, char g, char b, char a);

	void initLineLayer(LineLayer* lineLayer);
	void addLineToLayer(LineLayer* lineLayer, unsigned int v1, unsigned int v2);
	void drawLineLayer(LineLayer* lineLayer);

	void initTriangleLayer(TriangleLayer* triangleLayer);
	void addTriangleToLayer(TriangleLayer* triangleLayer, unsigned int v1, unsigned int v2, unsigned int v3);
	void drawTriangleLayer(TriangleLayer* triangleLayer);
};

}
