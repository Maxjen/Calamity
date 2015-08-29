#pragma once

//#include "Widget.h"
#include "Gui/ListView.h"
#include "Renderer/RenderLayer.h"
#include "SpriteManager.h"

namespace clm {

class ObjectItem : public ListViewItem {
private:
	SpriteManager* spriteManager;
	unsigned int fontId;
	string* cObjectFileName;
	string fileName;
	bool color;
	bool isSelected;

	unsigned int spriteId;
	unsigned int textId;
public:
	explicit ObjectItem(SpriteManager* spriteManager, string* cObjectFileName, const char* fileName, unsigned int fontId);
	~ObjectItem();
	void initialize(RenderLayer* renderLayer, bool color);
	int getHeight();
	void handleEvent(int xRel, int yRel, RenderLayer* renderLayer, SDL_Event* event);
	void draw(RenderLayer* renderLayer, int posX, int posY, int width);
};

}
