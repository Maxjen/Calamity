#pragma once

//#include "Widget.h"
#include "Gui/TreeList.h"
#include "Renderer/RenderLayer.h"
#include "SpriteManager.h"
#include "Functor.h"
#include "ActionManager.h"

namespace clm {

//using std::unordered_map;

class Bone3DItem : public TreeListItem {
private:
	SpriteManager* spriteManager;
	RenderLayer* renderLayer;
	//ActionManager* actionManager;
	unsigned int fontId;
	wstring label;
	unsigned int boneId;
	bool color;
	bool isSelected;

	unsigned int spriteId;
	unsigned int textId;

	//Functor* functor;
public:
	explicit Bone3DItem(SpriteManager* spriteManager, /*BoneManager* boneManager, BoneSelection* selection, ActionManager* actionManager, */wstring label, unsigned int boneId, unsigned int fontId);
	~Bone3DItem();
	void addChild(unsigned int child);
	//void setFunctor(Functor* functor);
	void initialize(RenderLayer* renderLayer, int depth);
	int getHeight();
	void setColor(bool bColor);
	void handleEvent(int xRel, int yRel, SDL_Event* event);
	void draw(int posX, int posY, int width);
};

}
