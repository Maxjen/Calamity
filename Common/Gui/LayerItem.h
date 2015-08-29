#pragma once

//#include "Widget.h"
#include "Gui/ListView.h"
#include "SpriteManager.h"
#include "Functor.h"
#include "../LayerManager.h"

using namespace clm;

//using std::unordered_map;

class LayerItem : public ListViewItem {
private:
	SpriteManager* spriteManager;
	LayerManager* layerManager;
	unsigned int fontId;
	wstring label;
	unsigned int layerId;
	bool color;
	bool isSelected;

	unsigned int spriteId;
	unsigned int textId;

	Functor* functor;
public:
	explicit LayerItem(SpriteManager* spriteManager, LayerManager* layerManager, wstring label, unsigned int layerId, unsigned int fontId);
	~LayerItem();
	void setFunctor(Functor* functor);
	void initialize(RenderLayer* renderLayer, bool color);
	int getHeight();
	void handleEvent(int xRel, int yRel, RenderLayer* renderLayer, SDL_Event* event);
	void draw(RenderLayer* renderLayer, int posX, int posY, int width);
};
