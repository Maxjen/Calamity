#pragma once

#include "Action.h"
#include "../BoneSelection.h"
#include "../../Common/BoneManager.h"
#include "../Functors/LoadBoneListFunctor.h"

using namespace clm;

class RemoveBones : public Action {
private:
	BoneManager* boneManager;
	BoneSelection* selection;
	LoadBoneListFunctor* loadBoneListFunctor;
	unordered_map<unsigned int, int> bonesToRemove;
	unsigned int oldMainSelectedBone;
public:
	explicit RemoveBones(BoneManager* boneManager, BoneSelection* selection, LoadBoneListFunctor* loadBoneListFunctor);
    void apply();
    void revert();
};
