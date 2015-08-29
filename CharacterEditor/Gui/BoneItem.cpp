#include "BoneItem.h"

namespace clm {

BoneItem::BoneItem(SpriteManager* spriteManager, BoneManager* boneManager, BoneSelection* selection, ActionManager* actionManager, wstring label, unsigned int boneId, unsigned int fontId) :
	spriteManager(spriteManager),
	boneManager(boneManager),
	selection(selection),
	actionManager(actionManager),
	fontId(fontId),
	label(label),
	boneId(boneId),
	color(false),
	isSelected(false),

	spriteId(-1),
	textId(-1)
{}

BoneItem::~BoneItem() {
	spriteManager->removeSprite(spriteId);
	spriteManager->removeText(textId);
}

/*void BoneItem::setFunctor(Functor *functor) {
	this->functor = functor;
}*/

void BoneItem::initialize(RenderLayer* renderLayer, bool color) {
	this->color = color;
	if (color)
		spriteId = spriteManager->addSprite(0, 0, "Calamity/Gui/Images/object_item_1.png", renderLayer);
	else
		spriteId = spriteManager->addSprite(0, 0, "Calamity/Gui/Images/object_item_2.png", renderLayer);

	textId = spriteManager->addText(label, fontId, 0, 0, 255, 255, 255, 255, renderLayer, true);
}

int BoneItem::getHeight() {
	return 20;
}

void BoneItem::handleEvent(int xRel, int yRel, RenderLayer* renderLayer, SDL_Event* event) {
	if (event->type == SDL_MOUSEBUTTONDOWN && event->button.button == SDL_BUTTON_LEFT) {
		unordered_map<unsigned int, int> newSelection;
		newSelection[boneId] = 3;
		SelectBones* action = new SelectBones(selection, newSelection, true);
		action->setAbsoluteSelection(true);
		actionManager->pushAction(action);
		/*if (!isSelected) {
			spriteManager->removeSprite(spriteId);
			spriteId = spriteManager->addSprite(0, 0, "Calamity/Gui/Images/object_item_selected.png", renderLayer);
			spriteManager->getRenderer()->getTextureManager()->upload();
			isSelected =true;
		}*/
		/*if (functor)
			(*functor)();*/
	}
	else if (event->type == SDL_KEYDOWN && event->key.keysym.sym == SDLK_a) {
		unordered_map<unsigned int, int> selectedBones = selection->getSelectedBones();
		if (!selectedBones.empty()) {
			SelectBones* action = new SelectBones(selection, selectedBones);
			actionManager->pushAction(action);
		}

		/*unordered_map<unsigned int, int> newSelection;
		SelectBones* action = new SelectBones(selection, newSelection, true);
		action->setAbsoluteSelection(true);
		actionManager->pushAction(action);
		boneManager->setSelectedBoneId(-1);*/
		/*if (functor)
			(*functor)();*/
	}
}

void BoneItem::draw(RenderLayer* renderLayer, int posX, int posY, int width) {
	if (boneId != boneManager->getSelectedBoneId() && isSelected) {
		spriteManager->removeSprite(spriteId);
		if (color)
			spriteId = spriteManager->addSprite(0, 0, "Calamity/Gui/Images/object_item_1.png", renderLayer);
		else
			spriteId = spriteManager->addSprite(0, 0, "Calamity/Gui/Images/object_item_2.png", renderLayer);
		isSelected = false;
	}
	else if (boneId == boneManager->getSelectedBoneId() && !isSelected) {
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
