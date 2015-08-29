#pragma once

#include <string>

#include "GameManager.h"
#include "Math/Vec2.h"

#include "Box2D/Collision/b2DynamicTree.h"

using std::string;

using namespace clm;

struct BoneVertexDistance {
	int boneVertex;
	float distance;
};

class BoneObject : public GameObject {
private:
	SpriteManager* spriteManager;
	RenderLayer* renderLayer;
	RenderLayer* overlayLayer;

	bool isMainSelectedObject;
	bool rootSelected, tipSelected;

	BoneObject* parent;
	set<BoneObject*> children;

	float x1, y1, x2, y2;
	float angle, length;

	unsigned int root, tip, left, right;
	unsigned int rootT, tipT, leftT, rightT;

	unsigned int lineToParentStart, lineToParentEnd;
public:
	explicit BoneObject(SpriteManager* spriteManager, float x1, float y1, float x2, float y2);
	void setRenderLayers(RenderLayer* renderLayer, RenderLayer* overlayLayer);
	b2AABB getAABB();
	void remove();
	void restore();
	int getObjectType();
	void flagForRender();

	void addBoneOverlay();
	void updateBoneOverlay();

	BoneObject* getParent();
	void setParent(BoneObject* newParent);
	void addChild(BoneObject* child);
	void removeChild(BoneObject* child);

	Vec2 getRootPosition();
	void setRootPosition(float x, float y);
	Vec2 getTipPosition();
	void setTipPosition(float x, float y);

	BoneVertexDistance getClosestBoneVertex(float x, float y);

	void toggleRootSelection();
	void toggleTipSelection();
	void setMainSelectedObject(bool isMainSelectedObject);
	void updateBoneOverlayColor();

	/*float getScale();
	void setScale(float scale);
	float getRotation();
	void setRotation(float rotation);*/

	/*bool containsPoint(float x, float y);
	void updateSelection();*/
};
