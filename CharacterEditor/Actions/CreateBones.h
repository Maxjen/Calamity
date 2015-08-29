#pragma once

#include "Math/Vec2.h"
#include "Action.h"
#include "../../Common/BoneObject.h"
#include "../BoneSelection.h"
#include "../../Common/BoneManager.h"
#include "../Functors/LoadBoneListFunctor.h"

using namespace clm;

class CreateBones : public Action {
private:
	SpriteManager* spriteManager;
	BoneManager* boneManager;
	BoneSelection* selection;
	LoadBoneListFunctor* loadBoneListFunctor;
	unsigned int layerId;
	list<Vec2> startingPositions;
	Vec2 boneVector;
	unordered_map<unsigned int, int> oldSelection;
	unsigned int oldMainSelectedBone;
	list<unsigned int> boneIds;
	unsigned int newMainSelectedBone;

    bool firstApply;
public:
	explicit CreateBones(SpriteManager* spriteManager, BoneManager* boneManager, BoneSelection* selection,
				LoadBoneListFunctor* loadBoneListFunctor, list<Vec2> startingPositions, unsigned int layerId);
	void setBoneVector(Vec2 boneVector);
    void apply();
    void revert();
};
