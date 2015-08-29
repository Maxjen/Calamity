#include "Bone3DItem.h"

namespace clm {

Bone3DItem::Bone3DItem(SpriteManager* spriteManager, /*BoneManager* boneManager, BoneSelection* selection, ActionManager* actionManager, */wstring label, unsigned int boneId, unsigned int fontId) :
	spriteManager(spriteManager),
	/*boneManager(boneManager),
	selection(selection),
	actionManager(actionManager),*/
	fontId(fontId),
	label(label),
	boneId(boneId),
	color(false),
	isSelected(false),

	spriteId(-1),
	textId(-1)
{}

Bone3DItem::~Bone3DItem() {
	spriteManager->removeSprite(spriteId);
	spriteManager->removeText(textId);
}

void Bone3DItem::addChild(unsigned int child) {
	children.push_back(child);
	if (children.size() == 1) {
		spriteManager->removeText(textId);
		wstring text(L"▼ ");
		text.append(label);
		textId = spriteManager->addText(text, fontId, 0, 0, 255, 255, 255, 255, renderLayer, true);
	}
}

/*void Bone3DItem::setFunctor(Functor *functor) {
	this->functor = functor;
}*/

void Bone3DItem::initialize(RenderLayer* renderLayer, int depth) {
	this->renderLayer = renderLayer;
	this->depth = depth;

	spriteId = spriteManager->addSprite(0, 0, "Calamity/Gui/Images/object_item_2.png", renderLayer);
	textId = spriteManager->addText(label, fontId, 0, 0, 255, 255, 255, 255, renderLayer, true);
}

int Bone3DItem::getHeight() {
	return 20;
}

void Bone3DItem::setColor(bool bColor) {
	if (color != bColor) {
		this->color = bColor;
		spriteManager->removeSprite(spriteId);
		if (bColor)
			spriteId = spriteManager->addSprite(0, 0, "Calamity/Gui/Images/object_item_1.png", renderLayer);
		else
			spriteId = spriteManager->addSprite(0, 0, "Calamity/Gui/Images/object_item_2.png", renderLayer);
	}
}

void Bone3DItem::handleEvent(int xRel, int yRel, SDL_Event* event) {
	if (event->type == SDL_MOUSEBUTTONDOWN && event->button.button == SDL_BUTTON_LEFT) {
		if (!children.empty()) {
			if (xRel >= depth * 10 + 5 && xRel < depth * 10 + 15 && yRel >= 5 && yRel <= 15) {
				bCollapsed = !bCollapsed;
				spriteManager->removeText(textId);
				wstring text;
				if (bCollapsed)
					text.assign(L"▶ ");
				else
					text.assign(L"▼ ");
				text.append(label);
				textId = spriteManager->addText(text, fontId, 0, 0, 255, 255, 255, 255, renderLayer, true);
			}
		}
		/*unordered_map<unsigned int, int> newSelection;
		newSelection[boneId] = 3;*/
		/*SelectBones* action = new SelectBones(selection, newSelection, true);
		action->setAbsoluteSelection(true);
		actionManager->pushAction(action);*/
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
		/*unordered_map<unsigned int, int> selectedBones = selection->getSelectedBones();
		if (!selectedBones.empty()) {
			SelectBones* action = new SelectBones(selection, selectedBones);
			actionManager->pushAction(action);
		}*/

		/*unordered_map<unsigned int, int> newSelection;
		SelectBones* action = new SelectBones(selection, newSelection, true);
		action->setAbsoluteSelection(true);
		actionManager->pushAction(action);
		boneManager->setSelectedBoneId(-1);*/
		/*if (functor)
			(*functor)();*/
	}
}

void Bone3DItem::draw(int posX, int posY, int width) {
	/*if (boneId != boneManager->getSelectedBoneId() && isSelected) {
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
	}*/

	int offsetX = depth * 10;

	spriteManager->setSpritePositionAndSize(spriteId, posX, posY, width, 20);
	spriteManager->flagSpriteForRender(spriteId);

	spriteManager->setTextPosition(textId, posX + 5 + offsetX, posY - 14);
	spriteManager->flagTextForRender(textId);
}

}
