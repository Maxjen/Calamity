#include "LoadBoneListFunctor.h"

LoadBoneListFunctor::LoadBoneListFunctor(BoneManager* boneManager, BoneSelection* selection,
										 ActionManager* actionManager, SpriteManager* spriteManager) :
	boneList(nullptr),
	boneManager(boneManager),
	selection(selection),
	actionManager(actionManager),
	spriteManager(spriteManager),
	fontId(-1)
{
	//this->functor = functor;
}

void LoadBoneListFunctor::setFontId(unsigned int fontId) {
	this->fontId = fontId;
}

void LoadBoneListFunctor::setBoneList(ListView* boneList) {
	this->boneList = boneList;
}

void LoadBoneListFunctor::operator()() {
	if (boneList && fontId != (unsigned int)-1) {
		boneList->clear();
		list<unsigned int> boneIds = boneManager->getBoneIds();
		for (auto& it : boneIds) {
			wstring label = boneManager->getBoneName(it);
			BoneItem* item = new BoneItem(spriteManager, boneManager, selection, actionManager, label, it, fontId);
			//item->setFunctor(functor);
			boneList->addItem(item);
		}
	}
}
