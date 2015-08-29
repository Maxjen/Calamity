#pragma once

#include "../../Calamity/Math/Vec2.h"
#include "../../Calamity/Action.h"
#include "../MapEditorSelection.h"

using namespace clm;

class RotateObjects : public Action {
private:
	MapEditorSelection* selection;

	list<unsigned int> spritesToRotate;
	list<Vec2> startingSpritePositions;
	list<float> startingSpriteRotations;

	unordered_map<unsigned int, unsigned int> verticesToRotate;
	list<Vec2> startingVertexPositions;

	Vec2 rotationCenter;
	float angle;
	bool absoluteRotation;
public:
	explicit RotateObjects(MapEditorSelection* selection, bool onlyMainSprite = false);
	void setAbsoluteRotation(bool absoluteRotation);
	void setRotationAngle(float angle);
	Vec2 getRotationCenter();
	list<Vec2> getStartingSpritePositions();
	list<float> getStartingSpriteRotations();
	list<Vec2> getStartingVertexPositions();
    void apply();
    void revert();
};
