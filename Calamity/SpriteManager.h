#pragma once

//#include <glm/glm.hpp>
#include <vector>
#include <map>
#include <string>

#include "Renderer/Renderer.h"
#include "Renderer/RenderLayer.h"
#include "Math/Vec2.h"

#include "Box2D/Collision/b2Collision.h"

using std::vector;
using std::map;
using std::wstring;

namespace clm {

struct Sprite {
	unsigned int textureId;
	unsigned int topLeftId, topRightId, bottomLeftId, bottomRightId;

	float posX, posY;
	float width, height;
	float originX, originY;
	float rotation;
	float scale;

	b2AABB aabb;

	RenderLayer* renderLayer;
};

struct Text {
	wstring text;
	unsigned int fontId;
	bool shadow;
	float x, y;
	char r, g, b, a;

	RenderLayer* renderLayer;
};

struct Line {
	unsigned int v1, v2;

	RenderLayer* renderLayer;
};

class SpriteManager
{
private:
	Renderer* renderer;
	int screenWidth, screenHeight;

	//vector<Sprite> sprites;
	map<unsigned int, Sprite> sprites;
	unsigned int cSprite;

	map<unsigned int, Text> texts;
	unsigned int cText;

	map<unsigned int, Line> lines;
	unsigned int cLine;
public:
	explicit SpriteManager(Renderer* renderer, int screenWidth, int screenHeight);
	~SpriteManager();

	Renderer* getRenderer();
	int getScreenWidth();
	int getScreenHeight();

	unsigned int addSprite(float posX, float posY, const char* textureName, RenderLayer* renderLayer);
	unsigned int addSprite(float posX, float posY, float width, float height, const char* textureName, RenderLayer* renderLayer);
	unsigned int addSpriteCentered(float posX, float posY, const char* textureName, RenderLayer* renderLayer);
	void removeSprite(unsigned int spriteId);

	b2AABB getSpriteAABB(unsigned int spriteId);
	Vec2 getSpriteVertexPosition(unsigned int spriteId, int v);
	/*float getSpriteWidth(unsigned int spriteId);
	float getSpriteHeight(unsigned int spriteId);*/

	void setSpritePosition(unsigned int spriteId, float posX, float posY);
	void setSpritePositionAndSize(unsigned int spriteId, float posX, float posY, float width, float height);
	void setSpriteRotation(unsigned int spriteId, float rotation);
	void setSpriteScale(unsigned int spriteId, float scale);

	void setVertexPositions(unsigned int spriteId);

	unsigned int addText(wstring text, unsigned int fontId, float x, float y, char r, char g, char b, char a, RenderLayer* renderLayer, bool shadow = false);
	void removeText(unsigned int textId);

	int getTextWidth(unsigned int textId);
	int calculateTextWidth(wstring text, unsigned int fontId);

	void setTextPosition(unsigned int textId, float posX, float posY);

	unsigned int addLine(float x1, float y1, float x2, float y2, char r1, char g1, char b1, char a1, char r2, char g2, char b2, char a2, RenderLayer* renderLayer);
	void removeLine(unsigned int lineId);

	//int addBackgroundSprite(int screenWidth, int screenHeight, const char* textureName, int spriteLayer);

	//void removeSprite(int id);

	void uploadTextures();

	void flagSpriteForRender(unsigned int spriteId);
	void flagTextForRender(unsigned int textId);
	void flagLineForRender(unsigned int lineId);
	void flagAllForRender();
};

}
