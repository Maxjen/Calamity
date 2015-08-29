#include "SpriteManager.h"

namespace clm {

SpriteManager::SpriteManager(Renderer* renderer, int screenWidth, int screenHeight) :
	renderer(renderer),
	screenWidth(screenWidth),
	screenHeight(screenHeight),

	sprites(),
	cSprite(0),

	texts(),
	cText(0),

	lines(),
	cLine(0)
{}

SpriteManager::~SpriteManager() {
	// TODO
}

Renderer* SpriteManager::getRenderer() {
	return renderer;
}

int SpriteManager::getScreenWidth() {
	return screenWidth;
}

int SpriteManager::getScreenHeight() {
	return screenHeight;
}

unsigned int SpriteManager::addSprite(float posX, float posY, const char* textureName, RenderLayer* renderLayer) {
	Sprite& s = sprites[cSprite];
	s.textureId = renderer->getTextureManager()->createTexture(textureName);
	s.posX = posX;
	s.posY = posY;

	s.width = renderer->getTextureManager()->getTextureWidth(s.textureId);
	s.height = renderer->getTextureManager()->getTextureHeight(s.textureId);
	s.originX = 0;
	s.originY = 0;
	s.rotation = 0;
	s.scale = 1.0f;
	s.renderLayer = renderLayer;

	s.topLeftId = renderer->addSpriteVertex(posX, posY, 0, 0, 255, 255, 255, 255, s.textureId);
	s.bottomLeftId = renderer->addSpriteVertex(posX, posY - s.height, 0, 1, 255, 255, 255, 255, s.textureId);
	s.bottomRightId = renderer->addSpriteVertex(posX + s.width, posY - s.height, 1, 1, 255, 255, 255, 255, s.textureId);
	s.topRightId = renderer->addSpriteVertex(posX + s.width, posY, 1, 0, 255, 255, 255, 255, s.textureId);
	setVertexPositions(cSprite);

	return cSprite++;
}

unsigned int SpriteManager::addSprite(float posX, float posY, float width, float height, const char* textureName, RenderLayer* renderLayer) {
	Sprite& s = sprites[cSprite];
	s.textureId = renderer->getTextureManager()->createTexture(textureName);
	s.posX = posX;
	s.posY = posY;

	s.width = width;
	s.height = height;
	s.originX = 0;
	s.originY = 0;
	s.rotation = 0;
	s.scale = 1.0f;
	s.renderLayer = renderLayer;

	s.topLeftId = renderer->addSpriteVertex(posX, posY, 0, 0, 255, 255, 255, 255, s.textureId);
	s.bottomLeftId = renderer->addSpriteVertex(posX, posY - s.height, 0, 1, 255, 255, 255, 255, s.textureId);
	s.bottomRightId = renderer->addSpriteVertex(posX + s.width, posY - s.height, 1, 1, 255, 255, 255, 255, s.textureId);
	s.topRightId = renderer->addSpriteVertex(posX + s.width, posY, 1, 0, 255, 255, 255, 255, s.textureId);
	setVertexPositions(cSprite);

	return cSprite++;
}

unsigned int SpriteManager::addSpriteCentered(float posX, float posY, const char* textureName, RenderLayer* renderLayer) {
	Sprite& s = sprites[cSprite];
	s.textureId = renderer->getTextureManager()->createTexture(textureName);
	s.posX = posX;
	s.posY = posY;

	s.width = renderer->getTextureManager()->getTextureWidth(s.textureId);
	s.height = renderer->getTextureManager()->getTextureHeight(s.textureId);
	s.originX = floor(s.width / 2);
	s.originY = -floor(s.height / 2);
	s.rotation = 0;
	s.scale = 1.0f;
	s.renderLayer = renderLayer;

	s.topLeftId = renderer->addSpriteVertex(posX, posY, 0, 0, 255, 255, 255, 255, s.textureId);
	s.bottomLeftId = renderer->addSpriteVertex(posX, posY - s.height, 0, 1, 255, 255, 255, 255, s.textureId);
	s.bottomRightId = renderer->addSpriteVertex(posX + s.width, posY - s.height, 1, 1, 255, 255, 255, 255, s.textureId);
	s.topRightId = renderer->addSpriteVertex(posX + s.width, posY, 1, 0, 255, 255, 255, 255, s.textureId);
	setVertexPositions(cSprite);

	return cSprite++;
}

void SpriteManager::removeSprite(unsigned int spriteId) {
	// TODO: remove texture
	Sprite& s = sprites[spriteId];
	renderer->removeSpriteVertex(s.topLeftId);
	renderer->removeSpriteVertex(s.bottomLeftId);
	renderer->removeSpriteVertex(s.bottomRightId);
	renderer->removeSpriteVertex(s.topRightId);
	sprites.erase(spriteId);
}

b2AABB SpriteManager::getSpriteAABB(unsigned int spriteId) {
	return sprites[spriteId].aabb;
}

Vec2 SpriteManager::getSpriteVertexPosition(unsigned int spriteId, int v) {
	Sprite& s = sprites[spriteId];

	if (v == 0)
		return renderer->getSpriteVertexPosition(s.topLeftId);
	else if (v == 1)
		return renderer->getSpriteVertexPosition(s.bottomLeftId);
	else if (v == 2)
		return renderer->getSpriteVertexPosition(s.bottomRightId);
	else
		return renderer->getSpriteVertexPosition(s.topRightId);
}

/*float SpriteManager::getSpriteWidth(unsigned int spriteId) {
	return sprites[spriteId].width;
}

float SpriteManager::getSpriteHeight(unsigned int spriteId) {
	return sprites[spriteId].height;
}*/

void SpriteManager::setSpritePosition(unsigned int spriteId, float posX, float posY) {
	Sprite& s = sprites[spriteId];
	s.posX = posX;
	s.posY = posY;

	/*renderer->setSpriteVertexPosition(s.topLeftId, posX, posY);
	renderer->setSpriteVertexPosition(s.bottomLeftId, posX, posY - s.height);
	renderer->setSpriteVertexPosition(s.bottomRightId, posX + s.width, posY - s.height);
	renderer->setSpriteVertexPosition(s.topRightId, posX + s.width, posY);*/
	setVertexPositions(spriteId);

}

void SpriteManager::setSpritePositionAndSize(unsigned int spriteId, float posX, float posY, float width, float height) {
	Sprite& s = sprites[spriteId];
	s.posX = posX;
	s.posY = posY;
	s.width = width;
	s.height = height;

	/*renderer->setSpriteVertexPosition(s.topLeftId, posX, posY);
	renderer->setSpriteVertexPosition(s.bottomLeftId, posX, posY - height);
	renderer->setSpriteVertexPosition(s.bottomRightId, posX + width, posY - height);
	renderer->setSpriteVertexPosition(s.topRightId, posX + width, posY);*/
	setVertexPositions(spriteId);
}

void SpriteManager::setSpriteRotation(unsigned int spriteId, float rotation) {
	Sprite& s = sprites[spriteId];
	s.rotation = rotation;

	setVertexPositions(spriteId);
}

void SpriteManager::setSpriteScale(unsigned int spriteId, float scale) {
	Sprite& s = sprites[spriteId];
	s.scale = scale;

	setVertexPositions(spriteId);
}

void SpriteManager::setVertexPositions(unsigned int spriteId) {
	Sprite& s = sprites[spriteId];

	Vec2 position(s.posX, s.posY);
	Vec2 origin(s.originX, s.originY);

	Vec2 topLeft = Vec2(0, 0) - origin;
	topLeft = vectorRot2D(topLeft, Vec2(0, 0), s.rotation) * s.scale;
	topLeft += position;

	Vec2 bottomLeft = Vec2(0, -s.height) - origin;
	bottomLeft = vectorRot2D(bottomLeft, Vec2(0, 0), s.rotation) * s.scale;
	bottomLeft += position;

	Vec2 bottomRight = Vec2(s.width, -s.height) - origin;
	bottomRight = vectorRot2D(bottomRight, Vec2(0, 0), s.rotation) * s.scale;
	bottomRight += position;

	Vec2 topRight = Vec2(s.width, 0) - origin;
	topRight = vectorRot2D(topRight, Vec2(0, 0), s.rotation) * s.scale;
	topRight += position;

	renderer->setSpriteVertexPosition(s.topLeftId, topLeft.x, topLeft.y);
	renderer->setSpriteVertexPosition(s.bottomLeftId, bottomLeft.x, bottomLeft.y);
	renderer->setSpriteVertexPosition(s.bottomRightId, bottomRight.x, bottomRight.y);
	renderer->setSpriteVertexPosition(s.topRightId, topRight.x, topRight.y);

	float xMin = std::min(topLeft.x, bottomLeft.x);
	xMin = std::min(xMin, bottomRight.x);
	xMin = std::min(xMin, topRight.x);
	float xMax = std::max(topLeft.x, bottomLeft.x);
	xMax = std::max(xMax, bottomRight.x);
	xMax = std::max(xMax, topRight.x);

	float yMin = std::min(topLeft.y, bottomLeft.y);
	yMin = std::min(yMin, bottomRight.y);
	yMin = std::min(yMin, topRight.y);
	float yMax = std::max(topLeft.y, bottomLeft.y);
	yMax = std::max(yMax, bottomRight.y);
	yMax = std::max(yMax, topRight.y);

	s.aabb.lowerBound.x = xMin;
	s.aabb.lowerBound.y = yMin;
	s.aabb.upperBound.x = xMax;
	s.aabb.upperBound.y = yMax;
}

/*int SpriteManager::addBackgroundSprite(int screenWidth, int screenHeight, const char* textureName, int spriteLayer) {
	Sprite s;
	s.textureId = spriteRenderer->getTextureManager()->createTexture(textureName);

	float textureWidth = spriteRenderer->getTextureManager()->getTextureWidth(s.textureId);
	float textureHeight = spriteRenderer->getTextureManager()->getTextureHeight(s.textureId);
	float s1 = screenWidth / textureWidth;
	float s2 = screenHeight / textureHeight;
	float scale = std::max(s1, s2);
	s.width = scale * textureWidth;
	s.height = scale * textureHeight;

	s.posX = 0;
	s.posY = screenHeight;
	s.offsetX = 0;
	s.offsetY = 0;
	s.rotation = 0;
	s.spriteLayer = spriteLayer;

	s.topLeftId = spriteRenderer->addSpriteVertex(s.posX, s.posY, 0, 0, 255, 255, 255, 255, s.textureId);
	s.bottomLeftId = spriteRenderer->addSpriteVertex(s.posX, s.posY - s.height, 0, 1, 255, 255, 255, 255, s.textureId);
	s.bottomRightId = spriteRenderer->addSpriteVertex(s.posX + s.width, s.posY - s.height, 1, 1, 255, 255, 255, 255, s.textureId);
	s.topRightId = spriteRenderer->addSpriteVertex(s.posX + s.width, s.posY, 1, 0, 255, 255, 255, 255, s.textureId);

	sprites.push_back(s);

	return 0;
}*/

/*void SpriteManager::removeSprite(int it) {

}*/

unsigned int SpriteManager::addText(wstring text, unsigned int fontId, float x, float y, char r, char g, char b, char a, RenderLayer* renderLayer, bool shadow) {
	Text& t = texts[cText];
	t.text = text;
	t.fontId = fontId;
	t.shadow = shadow;
	t.x = x;
	t.y = y;
	t.r = r;
	t.g = g;
	t.b = b;
	t.a = a;
	t.renderLayer = renderLayer;

	return cText++;
}

void SpriteManager::removeText(unsigned int textId) {
	texts.erase(textId);
}

int SpriteManager::getTextWidth(unsigned int textId) {
	Text& t = texts[textId];
	float width = 0;
	wchar_t previousLetter = *(t.text.begin());
	for (std::wstring::iterator it = t.text.begin(); it != t.text.end(); ++it) {
		Glyph g = renderer->getFontManager()->getGlyph(t.fontId, *it);
		float kerning = 0;
		if (it != t.text.begin() && g.kerning.find(previousLetter) != g.kerning.end()) {
			kerning = (*(g.kerning.find(previousLetter))).second;
		}
		width += kerning;
		width += g.advanceX;
		previousLetter = *it;
	}
	return width;
}

int SpriteManager::calculateTextWidth(wstring text, unsigned int fontId) {
	float width = 0;
	wchar_t previousLetter = *(text.begin());
	for (std::wstring::iterator it = text.begin(); it != text.end(); ++it) {
		Glyph g = renderer->getFontManager()->getGlyph(fontId, *it);
		float kerning = 0;
		if (it != text.begin() && g.kerning.find(previousLetter) != g.kerning.end()) {
			kerning = (*(g.kerning.find(previousLetter))).second;
		}
		width += kerning;
		width += g.advanceX;
		previousLetter = *it;
	}
	return width;
}

void SpriteManager::setTextPosition(unsigned int textId, float posX, float posY) {
	Text& t = texts[textId];
	t.x = posX;
	t.y = posY;
}

unsigned int SpriteManager::addLine(float x1, float y1, float x2, float y2, char r1, char g1, char b1, char a1, char r2, char g2, char b2, char a2, RenderLayer* renderLayer) {
	Line& l = lines[cLine];
	l.v1 = renderer->addColorVertex(x1, y1, r1, g1, b1, a1);
	l.v2 = renderer->addColorVertex(x2, y2, r2, g2, b2, a2);
	l.renderLayer = renderLayer;

	return cLine++;
}

void SpriteManager::removeLine(unsigned int lineId) {
	Line& l = lines[lineId];
	renderer->removeColorVertex(l.v1);
	renderer->removeColorVertex(l.v2);
	lines.erase(lineId);
}

void SpriteManager::uploadTextures() {
	renderer->getTextureManager()->upload();
	//spriteRenderer->getTextureManager()->writeToPng();
}

void SpriteManager::flagSpriteForRender(unsigned int spriteId) {
	Sprite& s = sprites[spriteId];
	s.renderLayer->addSpriteTriangle(s.topLeftId, s.bottomLeftId, s.topRightId);
	s.renderLayer->addSpriteTriangle(s.topRightId, s.bottomLeftId, s.bottomRightId);
}

void SpriteManager::flagTextForRender(unsigned int textId) {
	Text& t = texts[textId];
	float cX = t.x;
	wchar_t previousLetter = *(t.text.begin());
	for (std::wstring::iterator it = t.text.begin(); it != t.text.end(); ++it) {
		Glyph g = renderer->getFontManager()->getGlyph(t.fontId, *it);
		float kerning = 0;
		/*for (auto& it2 : g.kerning) {
			printf("%d %f\n", it2.first, it2.second);
		}*/
		//printf("%f\n", g.kerning[45]);
		if (it != t.text.begin() && g.kerning.find(previousLetter) != g.kerning.end()) {
			kerning = (*(g.kerning.find(previousLetter))).second;
			//printf("%f\n", kerning);
		}
		cX += kerning;
		if (t.shadow)
			t.renderLayer->addLetter(*it, t.fontId, cX, t.y - 1, 0, 0, 0, t.a);
		t.renderLayer->addLetter(*it, t.fontId, cX, t.y, t.r, t.g, t.b, t.a);
		cX += g.advanceX;
		previousLetter = *it;
	}
}

void SpriteManager::flagLineForRender(unsigned int lineId) {
	Line& l = lines[lineId];
	l.renderLayer->addLine(l.v1, l.v2);
}

void SpriteManager::flagAllForRender() {
	//vector<Sprite>::iterator it;
	for (auto& it : sprites) {
		//Sprite s = (*it);
		Sprite& s = it.second;
		/*int spriteLayer = s.renderLayer->getSpriteLayer();
		spriteRenderer->flagTriangleForRender(spriteLayer, s.topLeftId, s.bottomLeftId, s.topRightId);
		spriteRenderer->flagTriangleForRender(spriteLayer, s.topRightId, s.bottomLeftId, s.bottomRightId);*/
		s.renderLayer->addSpriteTriangle(s.topLeftId, s.bottomLeftId, s.topRightId);
		s.renderLayer->addSpriteTriangle(s.topRightId, s.bottomLeftId, s.bottomRightId);
	}

	/*for (auto& it : texts) {
		Text& t = it.second;
		float cX = t.x;
		wchar_t previousLetter = *(t.text.begin());
		for (std::wstring::iterator it = t.text.begin(); it != t.text.end(); ++it) {
			Glyph g = renderer->getFontManager()->getGlyph(t.fontId, *it);
			float kerning = 0;
			if (it != t.text.begin() && g.kerning.find(previousLetter) != g.kerning.end()) {
				kerning = (*(g.kerning.find(previousLetter))).second;
				//printf("%f\n", kerning);
			}
			cX += kerning;
			t.renderLayer->addLetter(*it, t.fontId, cX, t.y, t.r, t.g, t.b, t.a);
			cX += g.advanceX;
			previousLetter = *it;
		}
	}*/
}

}
