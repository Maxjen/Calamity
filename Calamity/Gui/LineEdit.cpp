#include "LineEdit.h"

namespace clm {

LineEdit::LineEdit(SpriteManager* spriteManager, int width, wstring label, unsigned int fontId, Properties* properties, Property* property, bool fillHorizontal) :
	renderLayer(nullptr),
	backgroundLayer(),
	borderLayer(),

	spriteManager(spriteManager),
	minWidth(width),
	minHeight(20),
	fillHorizontal(fillHorizontal),
	fillVertical(false),

	topLeftId(-1),
	topId(-1),
	topRightId(-1),
	leftId(-1),
	centerId(-1),
	rightId(-1),
	bottomLeftId(-1),
	bottomId(-1),
	bottomRightId(-1),

	labelId(-1),
	labelBg(-1),
	labelBorder(-1),

	textId(-1),
	cursorId(-1),
	cursorPos(0),
	label(label),
	value(),
	properties(properties),
	property(property),

	editMode(false),

	fontId(fontId)
{
	backgroundLayer.reset(new RenderLayer(spriteManager->getRenderer()));
	borderLayer.reset(new RenderLayer(spriteManager->getRenderer()));

	this->value.assign(L"");

	//scrollAmount = 0;
}

LineEdit::~LineEdit() {
	spriteManager->removeSprite(topLeftId);
	spriteManager->removeSprite(topId);
	spriteManager->removeSprite(topRightId);
	spriteManager->removeSprite(leftId);
	spriteManager->removeSprite(centerId);
	spriteManager->removeSprite(rightId);
	spriteManager->removeSprite(bottomLeftId);
	spriteManager->removeSprite(bottomId);
	spriteManager->removeSprite(bottomRightId);

	spriteManager->removeText(labelId);
	spriteManager->removeSprite(labelBg);
	spriteManager->removeSprite(labelBorder);

	spriteManager->removeText(textId);
	spriteManager->removeSprite(cursorId);
}

void LineEdit::setRenderLayer(RenderLayer* renderLayer) {
	this->renderLayer = renderLayer;

	centerId = spriteManager->addSprite(0, 0, "Calamity/Gui/Images/list_view_center.png", backgroundLayer.get());
	labelBg = spriteManager->addSprite(0, 0, "Calamity/Gui/Images/line_edit_label_bg.png", backgroundLayer.get());
	labelBorder = spriteManager->addSprite(0, 0, "Calamity/Gui/Images/line_edit_label_border.png", backgroundLayer.get());
	labelId = spriteManager->addText(label, fontId, 0, 0, 255, 255, 255, 255, backgroundLayer.get(), true);

	textId = spriteManager->addText(value, fontId, 0, 0, 255, 255, 255, 255, backgroundLayer.get(), true);
	cursorId = spriteManager->addSprite(0, 0, "Calamity/Gui/Images/input_cursor.png", backgroundLayer.get());

	topLeftId = spriteManager->addSprite(0, 0, "Calamity/Gui/Images/list_view_top_left.png", borderLayer.get());
	topId = spriteManager->addSprite(0, 0, "Calamity/Gui/Images/list_view_top.png", borderLayer.get());
	topRightId = spriteManager->addSprite(0, 0, "Calamity/Gui/Images/list_view_top_right.png", borderLayer.get());

	leftId = spriteManager->addSprite(0, 0, "Calamity/Gui/Images/list_view_left.png", borderLayer.get());
	rightId = spriteManager->addSprite(0, 0, "Calamity/Gui/Images/list_view_right.png", borderLayer.get());

	bottomLeftId = spriteManager->addSprite(0, 0, "Calamity/Gui/Images/list_view_bottom_left.png", borderLayer.get());
	bottomId = spriteManager->addSprite(0, 0, "Calamity/Gui/Images/list_view_bottom.png", borderLayer.get());
	bottomRightId = spriteManager->addSprite(0, 0, "Calamity/Gui/Images/list_view_bottom_right.png", borderLayer.get());

	spriteManager->getRenderer()->getTextureManager()->upload();
}

WidgetSize LineEdit::calculateMinSize() {
	WidgetSize result;
	result.width = minWidth;
	result.height = minHeight;
	result.fillHorizontal = fillHorizontal;
	result.fillVertical = fillVertical;
	return result;
}

void LineEdit::setSize(int width, int height) {
	this->width = width;
	this->height = height;
}

WidgetSize LineEdit::getSize() {
	WidgetSize result;
	result.width = minWidth;
	result.height = minHeight;
	result.fillHorizontal = fillHorizontal;
	result.fillVertical = fillVertical;
	return result;
}

void LineEdit::computeLayout() {
	backgroundLayer->clearLayers();
	borderLayer->clearLayers();

	/*if (!items.empty()) {
		maxScrollAmount = std::max(0, (int)items.size() * items[0]->getHeight() - height + 2);
		scrollAmount = std::min(scrollAmount, maxScrollAmount);
		scrollAmount = std::max(0, scrollAmount);
	}*/

	spriteManager->setSpritePositionAndSize(centerId, posX + 1, posY - 1, width - 2, height - 2);
	spriteManager->flagSpriteForRender(centerId);

	int labelWidth = spriteManager->getTextWidth(labelId);
	spriteManager->setSpritePositionAndSize(labelBg, posX + 1, posY - 1, labelWidth + 14, height - 2);
	spriteManager->flagSpriteForRender(labelBg);
	spriteManager->setSpritePositionAndSize(labelBorder, posX + labelWidth + 15, posY - 1, 3, height - 2);
	spriteManager->flagSpriteForRender(labelBorder);



	spriteManager->setTextPosition(labelId, posX + 8, posY - 14);
	spriteManager->flagTextForRender(labelId);


	if (editMode) {
		wstring textBeforeCursor = value.substr(0, cursorPos);
		int textWidth = spriteManager->calculateTextWidth(textBeforeCursor, fontId);
		spriteManager->setSpritePositionAndSize(cursorId, posX + labelWidth + textWidth + 22, posY - 5, 1, 10);
	}
	else {
		wstring newValue = property->getValueAsString();
		if (value != newValue) {
			value = newValue;
			spriteManager->removeText(textId);
			textId = spriteManager->addText(value, fontId, 0, 0, 255, 255, 255, 255, backgroundLayer.get(), true);
		}
	}
	spriteManager->setTextPosition(textId, posX + labelWidth + 22, posY - 14);
	spriteManager->flagTextForRender(textId);
	if (editMode)
		spriteManager->flagSpriteForRender(cursorId);


	spriteManager->setSpritePosition(topLeftId, posX, posY);
	spriteManager->setSpritePositionAndSize(topId, posX + 6, posY, width - 12, 7);
	spriteManager->setSpritePosition(topRightId, posX + width - 6, posY);

	spriteManager->setSpritePositionAndSize(leftId, posX, posY - 7, 6, height - 12);
	spriteManager->setSpritePositionAndSize(rightId, posX + width - 6, posY - 7, 6, height - 12);

	spriteManager->setSpritePosition(bottomLeftId, posX, posY - height + 5);
	spriteManager->setSpritePositionAndSize(bottomId, posX + 6, posY - height + 5, width - 12, 5);
	spriteManager->setSpritePosition(bottomRightId, posX + width - 6, posY - height + 5);


	spriteManager->flagSpriteForRender(topLeftId);
	spriteManager->flagSpriteForRender(topId);
	spriteManager->flagSpriteForRender(topRightId);
	spriteManager->flagSpriteForRender(leftId);
	spriteManager->flagSpriteForRender(rightId);
	spriteManager->flagSpriteForRender(bottomLeftId);
	spriteManager->flagSpriteForRender(bottomId);
	spriteManager->flagSpriteForRender(bottomRightId);


	/*if (!items.empty()) {
		unsigned int iStart = scrollAmount / items[0]->getHeight();
		unsigned int iEnd = std::min((int)(iStart + (height - 2) / items[0]->getHeight() + 1), (int)items.size());
		for (unsigned int i = iStart; i < iEnd; ++i)
			items[i]->draw(itemLayer, posX + 1, scrollAmount + posY - i * items[i]->getHeight() - 1, width - 2);
	}*/

	/*if ((int)items.size() * items[0]->getHeight() > height) {
		maxScrollAmount = (int)items.size() * items[0]->getHeight() - height;
		printf("scrollable\n");
	}*/
}

Properties* LineEdit::getProperties() {
	return properties;
}

Property* LineEdit::getProperty() {
	return property;
}

void LineEdit::setEditMode(bool editMode) {
	this->editMode = editMode;
}

void LineEdit::setCursorPos(unsigned int cursorPos) {
	this->cursorPos = cursorPos;
}

wstring LineEdit::getValue() {
	return value;
}

void LineEdit::setValue(wstring value) {
	this->value = value;
	spriteManager->removeText(textId);
	textId = spriteManager->addText(value, fontId, 0, 0, 255, 255, 255, 255, backgroundLayer.get(), true);
}

PriorityWidget LineEdit::getHighestPriorityWidget(int x, int y, SDL_Event* event) {
	PriorityWidget result;
	result.priority = 2;
	result.widget = this;
	return result;
}

ActionHandler* LineEdit::createActionHandler(int x, int y, SDL_Event* event) {
	if (event->type == SDL_MOUSEBUTTONDOWN && event->button.button == SDL_BUTTON_LEFT &&
			x > posX && x < posX + width && y < posY && y > posY - height) { // can't click on border
		return new LineEditHandler(this, spriteManager->getScreenHeight());
	}
	return nullptr;
}

void LineEdit::draw() {
	glEnable(GL_SCISSOR_TEST);
	glScissor(posX + 1, posY - height + 1, width - 2, height - 2);
	backgroundLayer->uploadLayerBuffers();
	backgroundLayer->draw();
	glDisable(GL_SCISSOR_TEST);

	borderLayer->uploadLayerBuffers();
	borderLayer->draw();
}

}
