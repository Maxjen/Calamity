#pragma once

#include "Functor.h"
#include "ActionManager.h"
#include "../../Common/BoneManager.h"
#include "../Actions/ReorderBone.h"

using namespace clm;

class ReorderBoneFunctor : public Functor {
private:
	ActionManager* actionManager;
	BoneManager* boneManager;
	ReorderBone* action;
	Functor* functor;
	bool bRaise;
public:
	ReorderBoneFunctor(ActionManager* actionManager, BoneManager* boneManager, Functor* functor, bool bRaise);
	void operator()();
};
