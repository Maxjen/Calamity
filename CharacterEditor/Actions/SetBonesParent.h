#pragma once

#include "../../Calamity/Math/Vec2.h"
#include "../../Calamity/Action.h"
#include "../BoneSelection.h"

using namespace clm;

class SetBonesParent : public Action {
private:
	BoneSelection* selection;
	unordered_map<unsigned int, int> bonesToSetParent;
	list<BoneObject*> startingParents;
	BoneObject* newParent;
public:
	explicit SetBonesParent(BoneSelection* selection, BoneObject* newParent);
    void apply();
    void revert();
};
