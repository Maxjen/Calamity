#pragma once

#include "Functor.h"
#include "../../Common/BoneManager.h"
#include "../Gui/BoneItem.h"
//#include "SelectLayerFunctor.h"

using namespace clm;

class LoadBoneListFunctor : public Functor {
private:
	ListView* boneList;
	BoneManager* boneManager;
	BoneSelection* selection;
	ActionManager* actionManager;
	SpriteManager* spriteManager;
	unsigned int fontId;
	//SelectLayerFunctor* functor;
public:
	explicit LoadBoneListFunctor(BoneManager* boneManager, BoneSelection* selection,
						ActionManager* actionManager, SpriteManager* spriteManager);
	void setFontId(unsigned int fontId);
	void setBoneList(ListView* boneList);
	void operator()();
};
