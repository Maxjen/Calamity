#pragma once

#include <string>

#include "GameManager.h"
#include "Math/Vec2.h"

#include "Box2D/Collision/b2DynamicTree.h"

using std::string;

using namespace clm;

class SpriteObject : public GameObject {
private:
	SpriteManager* spriteManager;
	RenderLayer* renderLayer;
	RenderLayer* overlayLayer;
	unsigned int spriteId;
	float posX, posY;
	float scale;
	float rotation;
	string textureName;

	int state;
	//bool isSelected;
	unsigned int topLeft, bottomLeft, bottomRight, topRight;
public:
	explicit SpriteObject(SpriteManager* spriteManager, float posX, float posY, const char* textureName);
	void setRenderLayers(RenderLayer* renderLayer, RenderLayer* overlayLayer);
	b2AABB getAABB();
	void remove();
	void restore();
	int getObjectType();
	bool containsPoint(float x, float y);
	void flagForRender();

	Vec2 getPosition();
	void setPosition(float x, float y);
	float getScale();
	void setScale(float scale);
	float getRotation();
	void setRotation(float rotation);

	//void toggleSelection();
	void setSelectionState(int newState);
	void updateSelection();
};
