#include "RemoveBones.h"

RemoveBones::RemoveBones(BoneManager* boneManager, BoneSelection* selection, LoadBoneListFunctor* loadBoneListFunctor) :
	boneManager(boneManager),
	selection(selection),
	loadBoneListFunctor(loadBoneListFunctor),
	bonesToRemove(),
	oldMainSelectedBone()
{
	bonesToRemove = selection->getSelectedBones();
	oldMainSelectedBone = selection->getMainSelectedBone();
}

void RemoveBones::apply() {
	for (auto& it : bonesToRemove) {
		selection->selectBone(it.first, it.second);
		boneManager->removeBone(it.first);
		selection->getGameManager()->removeGameObject(it.first);
	}
	//boneList->reload();
	if (loadBoneListFunctor != nullptr)
		(*loadBoneListFunctor)();
}

void RemoveBones::revert() {
	for (auto& it : bonesToRemove) {
		selection->getGameManager()->restoreGameObject(it.first);
		boneManager->restoreBone(it.first);
		selection->selectBone(it.first, it.second);
	}
	//boneList->reload();
	if (loadBoneListFunctor != nullptr)
		(*loadBoneListFunctor)();

	selection->setMainSelectedBone(oldMainSelectedBone);
}
