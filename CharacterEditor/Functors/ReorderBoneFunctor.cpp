#include "ReorderBoneFunctor.h"

ReorderBoneFunctor::ReorderBoneFunctor(ActionManager* actionManager, BoneManager* boneManager, Functor* functor, bool bRaise) {
	this->actionManager = actionManager;
	this->boneManager = boneManager;
	this->functor = functor;
	this->bRaise = bRaise;
}

void ReorderBoneFunctor::operator()() {
	unsigned int boneId = boneManager->getSelectedBoneId();
	if (boneId != (unsigned int)-1) {
		action = new ReorderBone(boneManager, boneId, functor, bRaise);
		actionManager->pushAction(action);
	}
}
