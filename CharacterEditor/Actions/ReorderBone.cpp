#include "ReorderBone.h"

ReorderBone::ReorderBone(BoneManager* boneManager, unsigned int boneId, Functor* functor, bool bRaise) :
	boneManager(boneManager),
	boneId(boneId),
	functor(functor),
	bRaise(bRaise)
{
	//this->boneList = boneList;
}

void ReorderBone::apply() {
	if (bRaise)
		boneManager->moveBoneUp(boneId);
	else
		boneManager->moveBoneDown(boneId);

	//reloadLayerList();
	//boneList->reload();
	boneManager->setSelectedBoneId(boneId);
	(*functor)();
}

void ReorderBone::revert() {
	if (bRaise)
		boneManager->moveBoneDown(boneId);
	else
		boneManager->moveBoneUp(boneId);

	//reloadLayerList();
	//boneList->reload();
	boneManager->setSelectedBoneId(boneId);
	(*functor)();
}
