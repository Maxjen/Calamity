#pragma once

#include "../../Calamity/Math/Vec2.h"
#include "../../Calamity/Action.h"
#include "../BoneSelection.h"

using namespace clm;

class ScaleBones : public Action {
private:
	BoneSelection* selection;
	unordered_map<unsigned int, int> bonesToScale;
	list<Vec2> startingPositions;
	Vec2 scaleCenter;
	float scaleFactor;
public:
	explicit ScaleBones(BoneSelection* selection);
	void setScaleFactor(float scaleFactor);
	Vec2 getScaleCenter();
	list<Vec2> getStartingPositions();
    void apply();
    void revert();
};
