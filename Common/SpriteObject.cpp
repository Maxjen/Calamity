#include "SpriteObject.h"

SpriteObject::SpriteObject(SpriteManager* spriteManager, float posX, float posY, const char* textureName) :
	spriteManager(spriteManager),
	renderLayer(nullptr),
	overlayLayer(nullptr),
	spriteId(-1),
	posX(posX),
	posY(posY),
	scale(1.0f),
	rotation(0),
	textureName(textureName),

	state(0),

	topLeft(-1),
	bottomLeft(-1),
	bottomRight(-1),
	topRight(-1)
{}

void SpriteObject::setRenderLayers(RenderLayer* renderLayer, RenderLayer* overlayLayer) {
	this->renderLayer = renderLayer;
	this->overlayLayer = overlayLayer;
	spriteId = spriteManager->addSpriteCentered(posX, posY, textureName.c_str(), renderLayer);
	//spriteManager->setSpriteRotation(spriteId, 3.14159265f / 4);

	/*Renderer* renderer = renderLayer->getRenderer();
	Vec2 pos;
	pos = spriteManager->getSpriteVertexPosition(spriteId, 0);
	topLeft = renderer->addColorVertex(pos.x, pos.y, 255, 255, 255, 255);
	pos = spriteManager->getSpriteVertexPosition(spriteId, 1);
	bottomLeft = renderer->addColorVertex(pos.x, pos.y, 255, 255, 255, 255);
	pos = spriteManager->getSpriteVertexPosition(spriteId, 2);
	bottomRight = renderer->addColorVertex(pos.x, pos.y, 255, 255, 255, 255);
	pos = spriteManager->getSpriteVertexPosition(spriteId, 3);
	topRight = renderer->addColorVertex(pos.x, pos.y, 255, 255, 255, 255);*/
}

b2AABB SpriteObject::getAABB() {
	return spriteManager->getSpriteAABB(spriteId);
}

void SpriteObject::remove() {
	Renderer* renderer = renderLayer->getRenderer();

	spriteManager->removeSprite(spriteId);

	// probably not needed
	if (state != 0) {
		renderer->removeColorVertex(topLeft);
		renderer->removeColorVertex(bottomLeft);
		renderer->removeColorVertex(bottomRight);
		renderer->removeColorVertex(topRight);
	}
}

void SpriteObject::restore() {
	spriteId = spriteManager->addSpriteCentered(posX, posY, textureName.c_str(), renderLayer);
	spriteManager->setSpriteScale(spriteId, scale);
	spriteManager->setSpriteRotation(spriteId, rotation);

	// probably not needed
	if (state) {
		int stateTmp = state;
		state = 0;
		setSelectionState(stateTmp);
	}

	updateSelection();
}

int SpriteObject::getObjectType() {
	return SPRITE_OBJECT;
}

bool SpriteObject::containsPoint(float x, float y) {
	return true;
}

void SpriteObject::flagForRender() {
	spriteManager->flagSpriteForRender(spriteId);

	//if (isSelected) {
	if (state) {
		overlayLayer->addLine(topLeft, bottomLeft);
		overlayLayer->addLine(bottomLeft, bottomRight);
		overlayLayer->addLine(bottomRight, topRight);
		overlayLayer->addLine(topRight, topLeft);
	}
}

Vec2 SpriteObject::getPosition() {
	return Vec2(posX, posY);
}

void SpriteObject::setPosition(float x, float y) {
	posX = x;
	posY = y;
	spriteManager->setSpritePosition(spriteId, posX, posY);
	updateSelection();
}

float SpriteObject::getScale() {
	return scale;
}

void SpriteObject::setScale(float scale) {
	this->scale = scale;
	spriteManager->setSpriteScale(spriteId, scale);
	updateSelection();
}

float SpriteObject::getRotation() {
	return rotation;
}

void SpriteObject::setRotation(float rotation) {
	this->rotation = rotation;
	spriteManager->setSpriteRotation(spriteId, rotation);
	updateSelection();
}

void SpriteObject::setSelectionState(int newState) {
	Renderer* renderer = renderLayer->getRenderer();

	if (newState == 0 && state != 0) {
		renderer->removeColorVertex(topLeft);
		renderer->removeColorVertex(bottomLeft);
		renderer->removeColorVertex(bottomRight);
		renderer->removeColorVertex(topRight);
	}
	else if (newState == 1) {
		if (state == 0) {
			Vec2 pos;
			pos = spriteManager->getSpriteVertexPosition(spriteId, 0);
			topLeft = renderer->addColorVertex(pos.x, pos.y, 155, 155, 155, 255);
			pos = spriteManager->getSpriteVertexPosition(spriteId, 1);
			bottomLeft = renderer->addColorVertex(pos.x, pos.y, 155, 155, 155, 255);
			pos = spriteManager->getSpriteVertexPosition(spriteId, 2);
			bottomRight = renderer->addColorVertex(pos.x, pos.y, 155, 155, 155, 255);
			pos = spriteManager->getSpriteVertexPosition(spriteId, 3);
			topRight = renderer->addColorVertex(pos.x, pos.y, 155, 155, 155, 255);
		}
		else if (state == 2) {
			renderer->setColorVertexColor(topLeft, 155, 155, 155, 255);
			renderer->setColorVertexColor(bottomLeft, 155, 155, 155, 255);
			renderer->setColorVertexColor(bottomRight, 155, 155, 155, 255);
			renderer->setColorVertexColor(topRight, 155, 155, 155, 255);
		}
	}
	else if (newState == 2) {
		if (state == 0) {
			Vec2 pos;
			pos = spriteManager->getSpriteVertexPosition(spriteId, 0);
			topLeft = renderer->addColorVertex(pos.x, pos.y, 255, 255, 255, 255);
			pos = spriteManager->getSpriteVertexPosition(spriteId, 1);
			bottomLeft = renderer->addColorVertex(pos.x, pos.y, 255, 255, 255, 255);
			pos = spriteManager->getSpriteVertexPosition(spriteId, 2);
			bottomRight = renderer->addColorVertex(pos.x, pos.y, 255, 255, 255, 255);
			pos = spriteManager->getSpriteVertexPosition(spriteId, 3);
			topRight = renderer->addColorVertex(pos.x, pos.y, 255, 255, 255, 255);
		}
		else if (state == 1) {
			renderer->setColorVertexColor(topLeft, 255, 255, 255, 255);
			renderer->setColorVertexColor(bottomLeft, 255, 255, 255, 255);
			renderer->setColorVertexColor(bottomRight, 255, 255, 255, 255);
			renderer->setColorVertexColor(topRight, 255, 255, 255, 255);
		}
	}
	else {
		return;
	}
	state = newState;
}

/*void SpriteObject::toggleSelection() {
	Renderer* renderer = renderLayer->getRenderer();
	if (isSelected) {
		renderer->removeColorVertex(topLeft);
		renderer->removeColorVertex(bottomLeft);
		renderer->removeColorVertex(bottomRight);
		renderer->removeColorVertex(topRight);
		isSelected = false;
	}
	else {
		Vec2 pos;
		pos = spriteManager->getSpriteVertexPosition(spriteId, 0);
		topLeft = renderer->addColorVertex(pos.x, pos.y, 255, 255, 255, 255);
		pos = spriteManager->getSpriteVertexPosition(spriteId, 1);
		bottomLeft = renderer->addColorVertex(pos.x, pos.y, 255, 255, 255, 255);
		pos = spriteManager->getSpriteVertexPosition(spriteId, 2);
		bottomRight = renderer->addColorVertex(pos.x, pos.y, 255, 255, 255, 255);
		pos = spriteManager->getSpriteVertexPosition(spriteId, 3);
		topRight = renderer->addColorVertex(pos.x, pos.y, 255, 255, 255, 255);
		isSelected = true;
	}
}*/

void SpriteObject::updateSelection() {
	//if (isSelected) {
	if (state) {
		Renderer* renderer = renderLayer->getRenderer();
		Vec2 pos;
		pos = spriteManager->getSpriteVertexPosition(spriteId, 0);
		renderer->setColorVertexPosition(topLeft, pos.x, pos.y);
		pos = spriteManager->getSpriteVertexPosition(spriteId, 1);
		renderer->setColorVertexPosition(bottomLeft, pos.x, pos.y);
		pos = spriteManager->getSpriteVertexPosition(spriteId, 2);
		renderer->setColorVertexPosition(bottomRight, pos.x, pos.y);
		pos = spriteManager->getSpriteVertexPosition(spriteId, 3);
		renderer->setColorVertexPosition(topRight, pos.x, pos.y);
	}
}
