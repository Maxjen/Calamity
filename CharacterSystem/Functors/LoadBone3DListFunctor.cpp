#include "LoadBone3DListFunctor.h"

LoadBone3DListFunctor::LoadBone3DListFunctor(SpriteManager* spriteManager, unsigned int fontId, CharacterObject* character) {
	this->boneList = nullptr;
	this->spriteManager = spriteManager;
	this->fontId = fontId;
	this->character = character;
	//this->functor = functor;
}

void LoadBone3DListFunctor::setBoneList(TreeList* boneList) {
	this->boneList = boneList;
}

void LoadBone3DListFunctor::operator()() {
	if (boneList != nullptr) {
		boneList->clear();
		//list<unsigned int> boneIds = boneManager->getBoneIds();
		/*for (unsigned int i = 0; i < 10; ++i) {
			wstring label = L"Item";
			Bone3DItem* item = new Bone3DItem(spriteManager, label, i, fontId);
			//item->setFunctor(functor);
			boneList->addItem(item);
		}*/
		/*Bone3DItem* item1 = new Bone3DItem(spriteManager, L"Item1", 0, fontId);
		unsigned int item1Id = boneList->addItem(item1);
		Bone3DItem* item2 = new Bone3DItem(spriteManager, L"Item2", 0, fontId);
		unsigned int item2Id = boneList->addItemAsChild(item2, item1Id);
		Bone3DItem* item3 = new Bone3DItem(spriteManager, L"Item3", 0, fontId);
		boneList->addItemAsChild(item3, item1Id);
		Bone3DItem* item4 = new Bone3DItem(spriteManager, L"Item4", 0, fontId);
		boneList->addItemAsChild(item4, item2Id);*/
		/*Bone3D* rootBone = character->getBoneHierarchy();
		wstring label(rootBone->boneName.begin(), rootBone->boneName.end());
		Bone3DItem* item = new Bone3DItem(spriteManager, label, 0, fontId);
		boneList->addItem(item);*/
		loadItems(character->getBoneHierarchy(), 0);
	}
}

void LoadBone3DListFunctor::loadItems(Bone3D* bone, unsigned int parentItemId) {
	if (bone) {
		wstring label(bone->boneName.begin(), bone->boneName.end());
		Bone3DItem* item = new Bone3DItem(spriteManager, label, 0, fontId);
		unsigned int itemId;

		if (!bone->parent)
			itemId = boneList->addItem(item);
		else
			itemId = boneList->addItemAsChild(item, parentItemId);

		for (auto& it : bone->children)
			loadItems(it, itemId);
	}
}
