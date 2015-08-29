#pragma once

#include <dirent.h>

//#include "Widget.h"
#include "Gui/ListView.h"
#include "Renderer/RenderLayer.h"
#include "SpriteManager.h"
#include "../CharacterObject.h"

class CharacterObject;

namespace clm {

class CharacterComponentItem : public ListViewItem {
private:
	SpriteManager* spriteManager;
	unsigned int fontId;

	CharacterObject* character;

	string fileName;
	bool bClothComponent;
	bool bComponentLoaded;

	bool color;
	bool isSelected;

	unsigned int spriteId;
	unsigned int checkboxId;
	unsigned int checkId;
	unsigned int textId;
public:
	explicit CharacterComponentItem(SpriteManager* spriteManager, CharacterObject* character, const char* fileName, unsigned int fontId, bool bClothComponent = false);
	~CharacterComponentItem();
	void initialize(RenderLayer* renderLayer, bool color);
	int getHeight();
	void handleEvent(int xRel, int yRel, RenderLayer* renderLayer, SDL_Event* event);
	void draw(RenderLayer* renderLayer, int posX, int posY, int width);
};

}
