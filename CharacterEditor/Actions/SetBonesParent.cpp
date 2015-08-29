#include "SetBonesParent.h"

SetBonesParent::SetBonesParent(BoneSelection* selection, BoneObject* newParent) :
	selection(selection),
	bonesToSetParent(),
	startingParents(),
	newParent(newParent)
{
	bonesToSetParent = selection->getSelectedBones();
	bonesToSetParent.erase(selection->getMainSelectedBone());
	GameManager* gameManager = selection->getGameManager();
	for (auto& it : bonesToSetParent) {
		BoneObject* cBone = (BoneObject*)gameManager->getObject(it.first);
		startingParents.push_back(cBone->getParent());
	}
}

void SetBonesParent::apply() {
	GameManager* gameManager = selection->getGameManager();
	for (auto& it : bonesToSetParent)
		((BoneObject*)gameManager->getObject(it.first))->setParent(newParent);
}

void SetBonesParent::revert() {
	GameManager* gameManager = selection->getGameManager();
	list<BoneObject*>::iterator parentIt = startingParents.begin();
	for (auto& it : bonesToSetParent) {
		((BoneObject*)gameManager->getObject(it.first))->setParent(*parentIt);
		++parentIt;
	}
}
