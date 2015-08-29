#pragma once

#include "../../Calamity/Math/Vec2.h"
#include "../../Calamity/Action.h"
#include "../BoneSelection.h"

using namespace clm;

class RotateBones : public Action {
private:
	BoneSelection* selection;
	unordered_map<unsigned int, int> bonesToRotate;
	list<Vec2> startingPositions;
	Vec2 rotationCenter;
	float angle;
public:
	explicit RotateBones(BoneSelection* selection);
	void setRotationAngle(float angle);
	Vec2 getRotationCenter();
	list<Vec2> getStartingPositions();
    void apply();
    void revert();
};
