#pragma once

//#include "Widget.h"
#include "Gui/ListView.h"
#include "Renderer/RenderLayer.h"
#include "SpriteManager.h"
#include "Functor.h"
#include "../../Common/BoneManager.h"
#include "ActionManager.h"
#include "../Actions/SelectBones.h"

namespace clm {

//using std::unordered_map;

class BoneItem : public ListViewItem {
private:
	SpriteManager* spriteManager;
	BoneManager* boneManager;
	BoneSelection* selection;
	ActionManager* actionManager;
	unsigned int fontId;
	wstring label;
	unsigned int boneId;
	bool color;
	bool isSelected;

	unsigned int spriteId;
	unsigned int textId;

	//Functor* functor;
public:
	explicit BoneItem(SpriteManager* spriteManager, BoneManager* boneManager, BoneSelection* selection, ActionManager* actionManager, wstring label, unsigned int boneId, unsigned int fontId);
	~BoneItem();
	//void setFunctor(Functor* functor);
	void initialize(RenderLayer* renderLayer, bool color);
	int getHeight();
	void handleEvent(int xRel, int yRel, RenderLayer* renderLayer, SDL_Event* event);
	void draw(RenderLayer* renderLayer, int posX, int posY, int width);
};

}
