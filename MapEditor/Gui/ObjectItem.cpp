#include "ObjectItem.h"

namespace clm {

ObjectItem::ObjectItem(SpriteManager* spriteManager, string* cObjectFileName, const char* fileName, unsigned int fontId) :
	spriteManager(spriteManager),
	fontId(fontId),
	cObjectFileName(cObjectFileName),
	fileName(fileName),
	color(false),
	isSelected(false),

	spriteId(-1),
	textId(-1)
{}

ObjectItem::~ObjectItem() {
	spriteManager->removeSprite(spriteId);
	spriteManager->removeText(textId);
}

void ObjectItem::initialize(RenderLayer* renderLayer, bool color) {
	this->color = color;
	if (color)
		spriteId = spriteManager->addSprite(0, 0, "Calamity/Gui/Images/object_item_1.png", renderLayer);
	else
		spriteId = spriteManager->addSprite(0, 0, "Calamity/Gui/Images/object_item_2.png", renderLayer);

	wstring label(fileName.begin(), fileName.end());
	textId = spriteManager->addText(label, fontId, 0, 0, 255, 255, 255, 255, renderLayer, true);
}

int ObjectItem::getHeight() {
	return 20;
}

void ObjectItem::handleEvent(int xRel, int yRel, RenderLayer* renderLayer, SDL_Event* event) {
	if (event->type == SDL_MOUSEBUTTONDOWN && event->button.button == SDL_BUTTON_LEFT) {
		*cObjectFileName = fileName;
		/*if (!isSelected) {
			spriteManager->removeSprite(spriteId);
			spriteId = spriteManager->addSprite(0, 0, "Calamity/Gui/Images/object_item_selected.png", renderLayer);
			spriteManager->getRenderer()->getTextureManager()->upload();
			isSelected =true;
		}*/
	}
	else if (event->type == SDL_KEYDOWN && event->key.keysym.sym == SDLK_a) {
		*cObjectFileName = "";
	}
}

void ObjectItem::draw(RenderLayer* renderLayer, int posX, int posY, int width) {
	/*if (fileName = selectedFilename && !isSelected) {
		spriteManager->removeSprite(spriteId);
		spriteId = spriteManager->addSprite(0, 0, "Calamity/Gui/Images/object_item_selected.png", renderLayer);
		isSelected = true;
	}
	else*/
	if (fileName != *cObjectFileName && isSelected) {
		spriteManager->removeSprite(spriteId);
		if (color)
			spriteId = spriteManager->addSprite(0, 0, "Calamity/Gui/Images/object_item_1.png", renderLayer);
		else
			spriteId = spriteManager->addSprite(0, 0, "Calamity/Gui/Images/object_item_2.png", renderLayer);
		isSelected = false;
	}
	else if (fileName == *cObjectFileName && !isSelected) {
		spriteManager->removeSprite(spriteId);
		spriteId = spriteManager->addSprite(0, 0, "Calamity/Gui/Images/object_item_selected.png", renderLayer);
		spriteManager->getRenderer()->getTextureManager()->upload();
		isSelected = true;
	}

	spriteManager->setSpritePositionAndSize(spriteId, posX, posY, width, 20);
	spriteManager->flagSpriteForRender(spriteId);

	spriteManager->setTextPosition(textId, posX + 5, posY - 14);
	spriteManager->flagTextForRender(textId);
}

}
