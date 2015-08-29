#pragma once

#include "Functor.h"
#include "../Gui/Bone3DItem.h"
#include "../CharacterObject.h"

using namespace clm;

class LoadBone3DListFunctor : public Functor {
private:
	void loadItems(Bone3D* bone, unsigned int parentItemId);

	TreeList* boneList;
	SpriteManager* spriteManager;
	unsigned int fontId;
	//SelectLayerFunctor* functor;
	CharacterObject* character;
public:
	LoadBone3DListFunctor(SpriteManager* spriteManager, unsigned int fontId, CharacterObject* character);
	void setBoneList(TreeList* boneList);
	void operator()();
};
