#include "LayerItem.h"

LayerItem::LayerItem(SpriteManager* spriteManager, LayerManager* layerManager, wstring label, unsigned int layerId, unsigned int fontId) :
	spriteManager(spriteManager),
	layerManager(layerManager),
	fontId(fontId),
	label(label),
	layerId(layerId),
	color(false),
	isSelected(false),

	spriteId(-1),
	textId(-1),

	functor(nullptr)
{}

LayerItem::~LayerItem() {
	spriteManager->removeSprite(spriteId);
	spriteManager->removeText(textId);
}

void LayerItem::setFunctor(Functor *functor) {
	this->functor = functor;
}

void LayerItem::initialize(RenderLayer* renderLayer, bool color) {
	this->color = color;
	if (color)
		spriteId = spriteManager->addSprite(0, 0, "Calamity/Gui/Images/object_item_1.png", renderLayer);
	else
		spriteId = spriteManager->addSprite(0, 0, "Calamity/Gui/Images/object_item_2.png", renderLayer);

	textId = spriteManager->addText(label, fontId, 0, 0, 255, 255, 255, 255, renderLayer, true);
}

int LayerItem::getHeight() {
	return 20;
}

void LayerItem::handleEvent(int xRel, int yRel, RenderLayer* renderLayer, SDL_Event* event) {
	if (event->type == SDL_MOUSEBUTTONDOWN && event->button.button == SDL_BUTTON_LEFT) {
		layerManager->setSelectedLayerId(layerId);
		/*if (!isSelected) {
			spriteManager->removeSprite(spriteId);
			spriteId = spriteManager->addSprite(0, 0, "Calamity/Gui/Images/object_item_selected.png", renderLayer);
			spriteManager->getRenderer()->getTextureManager()->upload();
			isSelected =true;
		}*/
		if (functor)
			(*functor)();
	}
	else if (event->type == SDL_KEYDOWN && event->key.keysym.sym == SDLK_a) {
		layerManager->setSelectedLayerId(-1);
		if (functor)
			(*functor)();
	}
}

void LayerItem::draw(RenderLayer* renderLayer, int posX, int posY, int width) {
	if (layerId != layerManager->getSelectedLayerId() && isSelected) {
		spriteManager->removeSprite(spriteId);
		if (color)
			spriteId = spriteManager->addSprite(0, 0, "Calamity/Gui/Images/object_item_1.png", renderLayer);
		else
			spriteId = spriteManager->addSprite(0, 0, "Calamity/Gui/Images/object_item_2.png", renderLayer);
		isSelected = false;
	}
	else if (layerId == layerManager->getSelectedLayerId() && !isSelected) {
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
