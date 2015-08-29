#include "Checkbox.h"

namespace clm {

Checkbox::Checkbox(SpriteManager* spriteManager, int width, wstring label, unsigned int fontId, Properties* properties, Property* property, bool fillHorizontal) :
	renderLayer(nullptr),

	spriteManager(spriteManager),
	minWidth(width),
	minHeight(20),
	fillHorizontal(fillHorizontal),
	fillVertical(false),

	checkboxId(-1),
	checkId(-1),

	labelId(-1),
	label(label),

	properties(properties),
	property(property),

	fontId(fontId),
	bChecked(false)
{
	if (property) {
		wstring wValue = property->getValueAsString();
		if (wValue.compare(L"true") == 0)
			bChecked = true;
	}
}

Checkbox::~Checkbox() {
	spriteManager->removeSprite(checkboxId);
	spriteManager->removeSprite(checkId);

	spriteManager->removeText(labelId);
}

void Checkbox::setRenderLayer(RenderLayer* renderLayer) {
	this->renderLayer = renderLayer;

	checkboxId = spriteManager->addSprite(0, 0, "Calamity/Gui/Images/checkbox.png", renderLayer);
	checkId = spriteManager->addSprite(0, 0, "Calamity/Gui/Images/checkbox_check.png", renderLayer);

	labelId = spriteManager->addText(label, fontId, 0, 0, 255, 255, 255, 255, renderLayer, true);

	spriteManager->getRenderer()->getTextureManager()->upload();
}

WidgetSize Checkbox::calculateMinSize() {
	WidgetSize result;
	result.width = minWidth;
	result.height = minHeight;
	result.fillHorizontal = fillHorizontal;
	result.fillVertical = fillVertical;
	return result;
}

void Checkbox::setSize(int width, int height) {
	this->width = width;
	this->height = height;
}

WidgetSize Checkbox::getSize() {
	WidgetSize result;
	result.width = minWidth;
	result.height = minHeight;
	result.fillHorizontal = fillHorizontal;
	result.fillVertical = fillVertical;
	return result;
}

void Checkbox::computeLayout() {
	spriteManager->setSpritePosition(checkboxId, posX + 3, posY - 3);
	spriteManager->setSpritePosition(checkId, posX + 3, posY - 3);
	spriteManager->flagSpriteForRender(checkboxId);
	if (bChecked)
		spriteManager->flagSpriteForRender(checkId);

	/*int labelWidth = spriteManager->getTextWidth(labelId);
	spriteManager->setSpritePositionAndSize(labelBg, posX + 1, posY - 1, labelWidth + 14, height - 2);
	spriteManager->flagSpriteForRender(labelBg);
	spriteManager->setSpritePositionAndSize(labelBorder, posX + labelWidth + 15, posY - 1, 3, height - 2);
	spriteManager->flagSpriteForRender(labelBorder);*/



	spriteManager->setTextPosition(labelId, posX + 25, posY - 14);
	spriteManager->flagTextForRender(labelId);
}

Properties* Checkbox::getProperties() {
	return properties;
}

Property* Checkbox::getProperty() {
	return property;
}

PriorityWidget Checkbox::getHighestPriorityWidget(int x, int y, SDL_Event* event) {
	PriorityWidget result;
	result.priority = 2;
	result.widget = this;
	return result;
}

ActionHandler* Checkbox::createActionHandler(int x, int y, SDL_Event* event) {
	if (event->type == SDL_MOUSEBUTTONDOWN && event->button.button == SDL_BUTTON_LEFT &&
			x > posX + 3 && x < posX + 19 && y < posY - 3 && y > posY - height + 3 &&
			properties && property) {
		wstring newValue(L"false");
		wstring oldValue = property->getValueAsString();
		if (oldValue.compare(L"false") == 0) {
			newValue.assign(L"true");
			bChecked = true;
		}
		else
			bChecked = false;
		properties->pushPropertyChange(property, oldValue, newValue);
	}
	return nullptr;
}

void Checkbox::draw() {

}

}
