#include "CreateBones.h"

bool compareVec2(const Vec2& first, const Vec2& second) {
	if (first.x != second.x)
		return first.x < second.x;
	else
		return first.y < second.y;
}

CreateBones::CreateBones(SpriteManager* spriteManager, BoneManager* boneManager, BoneSelection* selection,
						 LoadBoneListFunctor* loadBoneListFunctor, list<Vec2> startingPositions, unsigned int layerId) :
	spriteManager(spriteManager),
	boneManager(boneManager),
	selection(selection),
	loadBoneListFunctor(loadBoneListFunctor),
	layerId(layerId),
	startingPositions(startingPositions),
	boneVector(),
	oldSelection(),
	oldMainSelectedBone(),
	boneIds(),
	newMainSelectedBone(-1),

	firstApply(true)
{
	oldSelection = selection->getSelectedBones();
	oldMainSelectedBone = selection->getMainSelectedBone();
}

void CreateBones::setBoneVector(Vec2 boneVector) {
	this->boneVector = boneVector;
}

void CreateBones::apply() {
	GameManager* gameManager = selection->getGameManager();
    if (firstApply) {
		if (!oldSelection.empty()) {
			for (auto& it : oldSelection)
				selection->selectBone(it.first, it.second);
		}

		startingPositions.sort(compareVec2);
		startingPositions.unique();
		for (auto& it : startingPositions) {
			BoneObject* object = new BoneObject(spriteManager, it.x, it.y, it.x + boneVector.x, it.y + boneVector.y);
			unsigned int boneId = gameManager->addGameObject(object, layerId);
			boneIds.push_back(boneId);
			boneManager->addBone(L"new Bone", boneId);
			selection->selectBone(boneId, 2);
		}
		if (boneIds.size() == 1)
			newMainSelectedBone = *(boneIds.begin());

        firstApply = false;
    }
    else {
		if (!oldSelection.empty()) {
			for (auto& it : oldSelection)
				selection->selectBone(it.first, it.second);
		}

		for (auto& it : boneIds) {
			gameManager->restoreGameObject(it);
			boneManager->restoreBone(it);
			selection->selectBone(it, 2);
		}
    }
	//boneList->reload();
	if (loadBoneListFunctor != nullptr)
		(*loadBoneListFunctor)();

	selection->setMainSelectedBone(newMainSelectedBone);
}

void CreateBones::revert() {
	GameManager* gameManager = selection->getGameManager();
	for (auto& it : boneIds) {
		selection->selectBone(it, 2);
		boneManager->removeBone(it);
		gameManager->removeGameObject(it);
	}

	if (!oldSelection.empty()) {
		for (auto& it : oldSelection)
			selection->selectBone(it.first, it.second);
	}
	//boneList->reload();
	if (loadBoneListFunctor != nullptr)
		(*loadBoneListFunctor)();

	selection->setMainSelectedBone(oldMainSelectedBone);
}
