#pragma once

#include "../../Calamity/Math/Vec2.h"
#include "../../Calamity/Action.h"
#include "../BoneSelection.h"

using namespace clm;

class MoveBones : public Action {
private:
	BoneSelection* selection;
	unordered_map<unsigned int, int> bonesToMove;
	list<Vec2> startingPositions;
	Vec2 deltaPosition;
public:
	explicit MoveBones(BoneSelection* selection);
	void setDeltaPosition(Vec2 deltaPosition);
	list<Vec2> getStartingPositions();
    void apply();
    void revert();
};
