#pragma once

#include "../../Calamity/Action.h"
#include "../../Common/BoneManager.h"
#include "../../Calamity/Functor.h"

using namespace clm;

class ReorderBone : public Action {
private:
	BoneManager* boneManager;
	unsigned int boneId;
	Functor* functor;
	bool bRaise;
public:
	explicit ReorderBone(BoneManager* boneManager, unsigned int boneId, Functor* functor, bool bRaise);
    void apply();
    void revert();
};
