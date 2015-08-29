#include "CharacterComponentItem.h"

namespace clm {

CharacterComponentItem::CharacterComponentItem(SpriteManager* spriteManager, CharacterObject* character, const char* fileName, unsigned int fontId, bool bClothComponent) :
	spriteManager(spriteManager),
	fontId(fontId),
	character(character),
	fileName(fileName),
	bClothComponent(bClothComponent),
	bComponentLoaded(false),
	color(false),
	isSelected(false),

	spriteId(-1),
	checkboxId(-1),
	checkId(-1),
	textId(-1)
{
	string path;
	if (bClothComponent)
		path.append("Data/ClothComponents/");
	else
		path.append("Data/MeshComponents/");
	path.append(fileName);

	if (bClothComponent)
		bComponentLoaded = character->clothComponentLoaded(path);
	else
		bComponentLoaded = character->meshComponentLoaded(path);
}

CharacterComponentItem::~CharacterComponentItem() {
	spriteManager->removeSprite(spriteId);
	spriteManager->removeSprite(checkboxId);
	spriteManager->removeSprite(checkId);
	spriteManager->removeText(textId);
}

void CharacterComponentItem::initialize(RenderLayer* renderLayer, bool color) {
	this->color = color;
	if (color)
		spriteId = spriteManager->addSprite(0, 0, "Calamity/Gui/Images/object_item_1.png", renderLayer);
	else
		spriteId = spriteManager->addSprite(0, 0, "Calamity/Gui/Images/object_item_2.png", renderLayer);

	checkboxId = spriteManager->addSprite(0, 0, "Calamity/Gui/Images/checkbox.png", renderLayer);
	checkId = spriteManager->addSprite(0, 0, "Calamity/Gui/Images/checkbox_check.png", renderLayer);

	wstring label(fileName.begin(), fileName.end());
	textId = spriteManager->addText(label, fontId, 0, 0, 255, 255, 255, 255, renderLayer, true);
}

int CharacterComponentItem::getHeight() {
	return 20;
}

void CharacterComponentItem::handleEvent(int xRel, int yRel, RenderLayer* renderLayer, SDL_Event* event) {
	if (event->type == SDL_MOUSEBUTTONDOWN && event->button.button == SDL_BUTTON_LEFT) {
		//*cObjectFileName = fileName;

		string path;
		if (bClothComponent)
			path.append("Data/ClothComponents/");
		else
			path.append("Data/MeshComponents/");
		path.append(fileName);

		if (!bComponentLoaded) {
			if (bClothComponent)
				character->addClothComponent(path);
			else
				character->addMeshComponent(path);
			bComponentLoaded = true;
		}
		else {
			if (bClothComponent)
				character->removeClothComponent(path);
			else
				character->removeMeshComponent(path);
			bComponentLoaded = false;
		}

		/*if (!isSelected) {
			spriteManager->removeSprite(spriteId);
			spriteId = spriteManager->addSprite(0, 0, "Calamity/Gui/Images/object_item_selected.png", renderLayer);
			spriteManager->getRenderer()->getTextureManager()->upload();
			isSelected =true;
		}*/
	}
	/*else if (event->type == SDL_KEYDOWN && event->key.keysym.sym == SDLK_a) {
		*cObjectFileName = "";
	}*/
}

void CharacterComponentItem::draw(RenderLayer* renderLayer, int posX, int posY, int width) {
	/*if (fileName = selectedFilename && !isSelected) {
		spriteManager->removeSprite(spriteId);
		spriteId = spriteManager->addSprite(0, 0, "Calamity/Gui/Images/object_item_selected.png", renderLayer);
		isSelected = true;
	}
	else*/
	/*if (fileName != *cObjectFileName && isSelected) {
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
	}*/

	spriteManager->setSpritePositionAndSize(spriteId, posX, posY, width, 20);
	spriteManager->flagSpriteForRender(spriteId);

	spriteManager->setSpritePosition(checkboxId, posX + 8, posY - 3);
	spriteManager->setSpritePosition(checkId, posX + 8, posY - 3);
	spriteManager->flagSpriteForRender(checkboxId);
	if (bComponentLoaded)
		spriteManager->flagSpriteForRender(checkId);

	spriteManager->setTextPosition(textId, posX + 30, posY - 14);
	spriteManager->flagTextForRender(textId);
}

}
