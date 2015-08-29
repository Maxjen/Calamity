#include "InputGroup.h"

namespace clm {

Input::Input(SpriteManager* spriteManager, RenderLayer* renderLayer, wstring label, unsigned int fontId,
			 Properties* properties, Property* property) :
	spriteManager(spriteManager),
	renderLayer(renderLayer),
	fontId(fontId),
	x(0),
	y(0),
	width(0),
	verticalSeperator(false),
	horizontalSeperators(false),
	vertical(-1),
	top(-1),
	bottom(-1),
	textId(-1),
	cursorId(spriteManager->addSprite(0, 0, "Calamity/Gui/Images/input_cursor.png", renderLayer)),
	cursorPos(0),
	label(label),
	value(),
	properties(properties),
	property(property),

	editMode(false)
{
	this->label.append(L": ");
	this->value.assign(L"");

	textId = spriteManager->addText(this->label, fontId, 0, 0, 255, 255, 255, 255, renderLayer, true);

	spriteManager->getRenderer()->getTextureManager()->upload();
}

Input::~Input() {
	if (verticalSeperator)
		spriteManager->removeSprite(vertical);
	if ((horizontalSeperators & 1) == 1)
		spriteManager->removeSprite(top);
	if ((horizontalSeperators & 2) == 2)
		spriteManager->removeSprite(bottom);
	spriteManager->removeSprite(cursorId);
	spriteManager->removeText(textId);
}

bool Input::contains(int x, int y) {
	int relX = x - this->x;
	int relY = this->y - y;
	return (relX > 0 && relX < width - 1 && relY > 0 && relY < 19);
}

Properties* Input::getProperties() {
	return properties;
}

Property* Input::getProperty() {
	return property;
}

void Input::setEditMode(bool editMode) {
	this->editMode = editMode;
	spriteManager->removeText(textId);
	if (editMode)
		textId = spriteManager->addText(value, fontId, 0, 0, 255, 255, 255, 255, renderLayer, true);
	else {
		wstring text = label;
		text.append(value);
		textId = spriteManager->addText(text, fontId, 0, 0, 255, 255, 255, 255, renderLayer, true);
	}
}

void Input::setCursorPos(unsigned int cursorPos) {
	this->cursorPos = cursorPos;
}

wstring Input::getValue() {
	return value;
}

void Input::setValue(wstring value) {
	this->value = value;
	spriteManager->removeText(textId);
	if (editMode)
		textId = spriteManager->addText(value, fontId, 0, 0, 255, 255, 255, 255, renderLayer, true);
	else {
		wstring text = label;
		text.append(value);
		textId = spriteManager->addText(text, fontId, 0, 0, 255, 255, 255, 255, renderLayer, true);
	}
}

void Input::setPosition(float x, float y) {
	this->x = x;
	this->y = y;
}

void Input::setSeperators(bool verticalSeperator, int horizontalSeperators) {
	this->verticalSeperator = verticalSeperator;
	this->horizontalSeperators = horizontalSeperators;
}

void Input::setSprites() {
	if (verticalSeperator)
		vertical = spriteManager->addSprite(0, 0, "Calamity/Gui/Images/input_seperator_vertical.png", renderLayer);
	if ((horizontalSeperators & 1) == 1)
		top = spriteManager->addSprite(0, 0, "Calamity/Gui/Images/input_seperator_horizontal_s.png", renderLayer);
	if ((horizontalSeperators & 2) == 2)
		bottom = spriteManager->addSprite(0, 0, "Calamity/Gui/Images/input_seperator_horizontal_l.png", renderLayer);
}

void Input::setWidth(float width) {
	this->width = width;
}

void Input::updateSprites() {
	if (verticalSeperator)
		spriteManager->setSpritePositionAndSize(vertical, x + width - 2, y, 3, 20);
	if ((horizontalSeperators & 1) == 1)
		spriteManager->setSpritePositionAndSize(top, x + 1, y - 1, width - 2, 1);
	if ((horizontalSeperators & 2) == 2)
		spriteManager->setSpritePositionAndSize(bottom, x + 1, y - 18, width - 2, 1);

	if (editMode) {
		spriteManager->setTextPosition(textId, x + 8, y - 14);
		wstring textBeforeCursor = value.substr(0, cursorPos);
		int textWidth = spriteManager->calculateTextWidth(textBeforeCursor, fontId);
		spriteManager->setSpritePositionAndSize(cursorId, x + 9 + textWidth, y - 5, 1, 10);
	}
	else {
		if (property) {
			wstring newValue = property->getValueAsString();
			if (value != newValue) {
				value = newValue;
				spriteManager->removeText(textId);
				wstring text = label;
				text.append(value);
				textId = spriteManager->addText(text, fontId, 0, 0, 255, 255, 255, 255, renderLayer, true);
			}
		}
		int textWidth = spriteManager->getTextWidth(textId);
		spriteManager->setTextPosition(textId, floor(x + width / 2 - textWidth / 2), y - 14);
	}
}

void Input::prepare() {
	if (verticalSeperator)
		spriteManager->flagSpriteForRender(vertical);
	if ((horizontalSeperators & 1) == 1)
		spriteManager->flagSpriteForRender(top);
	if ((horizontalSeperators & 2) == 2)
		spriteManager->flagSpriteForRender(bottom);

	spriteManager->flagTextForRender(textId);

	if (editMode)
		spriteManager->flagSpriteForRender(cursorId);
}

InputGroup::InputGroup(SpriteManager* spriteManager, unsigned int fontId, int width, bool fillHorizontal) :
	renderLayer(nullptr),
	inputLayer(),
	borderLayer(),

	spriteManager(spriteManager),
	minWidth(width),
	minHeight(0),
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

	fontId(fontId),
	rows(),
	horizontalSeperatorIds()
{
	this->width = width;
	inputLayer.reset(new RenderLayer(spriteManager->getRenderer()));
	borderLayer.reset(new RenderLayer(spriteManager->getRenderer()));
}

InputGroup::~InputGroup() {
	spriteManager->removeSprite(topLeftId);
	spriteManager->removeSprite(topId);
	spriteManager->removeSprite(topRightId);
	spriteManager->removeSprite(leftId);
	spriteManager->removeSprite(centerId);
	spriteManager->removeSprite(rightId);
	spriteManager->removeSprite(bottomLeftId);
	spriteManager->removeSprite(bottomId);
	spriteManager->removeSprite(bottomRightId);

	for (auto& itRow : rows) {
		for (auto& itInput : itRow)
			delete itInput;
	}
	/*vector<vector<Button*>>::iterator itRow;
	for (itRow = rows.begin(); itRow != rows.end(); ++itRow) {
		vector<Button*>::iterator itButton;
		for (itButton = (*itRow).begin(); itButton != (*itRow).end(); ++itButton)
			delete (*itButton);
	}*/
}

void InputGroup::initialize() {
	for (unsigned int i = 0; i < rows.size(); ++i) {
		for (unsigned int j = 0; j < rows[i].size(); ++j) {
			bool verticalSeperator = j < rows[i].size() - 1;
			int top = (i == 0) ? 0 : 1;
			int bottom = (i == rows.size() - 1) ? 0 : 2;
			rows[i][j]->setSeperators(verticalSeperator, top | bottom);
			rows[i][j]->setSprites();


			/*int top = (i == 0) ? 1 : 0;
			int bottom = (i == rows.size() - 1) ? 2 : 0;

			int topLeft = (j == 0) ? 1 : 0;
			topLeft &= top;
			int bottomLeft = (j == 0) ? 2 : 0;
			bottomLeft &= bottom;
			int left = topLeft | bottomLeft;

			int topRight = (j == rows[i].size() - 1) ? 1 : 0;
			topRight &= top;
			int bottomRight = (j == rows[i].size() - 1) ? 2 : 0;
			bottomRight &= bottom;
			int right = topRight | bottomRight;


			rows[i][j]->setLeft(left);
			rows[i][j]->setRight(right);
			rows[i][j]->setSprites();*/
		}
	}
}

void InputGroup::setRenderLayer(RenderLayer* renderLayer) {
	this->renderLayer = renderLayer;

	centerId = spriteManager->addSprite(0, 0, "Calamity/Gui/Images/list_view_center.png", inputLayer.get());

	topLeftId = spriteManager->addSprite(0, 0, "Calamity/Gui/Images/list_view_top_left.png", borderLayer.get());
	topId = spriteManager->addSprite(0, 0, "Calamity/Gui/Images/list_view_top.png", borderLayer.get());
	topRightId = spriteManager->addSprite(0, 0, "Calamity/Gui/Images/list_view_top_right.png", borderLayer.get());

	leftId = spriteManager->addSprite(0, 0, "Calamity/Gui/Images/list_view_left.png", borderLayer.get());
	rightId = spriteManager->addSprite(0, 0, "Calamity/Gui/Images/list_view_right.png", borderLayer.get());

	bottomLeftId = spriteManager->addSprite(0, 0, "Calamity/Gui/Images/list_view_bottom_left.png", borderLayer.get());
	bottomId = spriteManager->addSprite(0, 0, "Calamity/Gui/Images/list_view_bottom.png", borderLayer.get());
	bottomRightId = spriteManager->addSprite(0, 0, "Calamity/Gui/Images/list_view_bottom_right.png", borderLayer.get());

	/*topLeftId = spriteManager->addSprite(0, 0, "Calamity/Gui/Images/buttons_sh_top_left.png", renderLayer);
	topId = spriteManager->addSprite(0, 0, "Calamity/Gui/Images/buttons_sh_top.png", renderLayer);
	topRightId = spriteManager->addSprite(posX + width - 5, posY + 10, "Calamity/Gui/Images/buttons_sh_top_right.png", renderLayer);

	leftId = spriteManager->addSprite(0, 0, "Calamity/Gui/Images/buttons_sh_left.png", renderLayer);
	rightId = spriteManager->addSprite(0, 0, "Calamity/Gui/Images/buttons_sh_right.png", renderLayer);

	bottomLeftId = spriteManager->addSprite(0, 0, "Calamity/Gui/Images/buttons_sh_bottom_left.png", renderLayer);
	bottomId = spriteManager->addSprite(0, 0, "Calamity/Gui/Images/buttons_sh_bottom.png", renderLayer);
	bottomRightId = spriteManager->addSprite(0, 0, "Calamity/Gui/Images/buttons_sh_bottom_right.png", renderLayer);*/
}

WidgetSize InputGroup::calculateMinSize() {
	WidgetSize result;
	result.width = minWidth;
	result.height = minHeight;
	result.fillHorizontal = fillHorizontal;
	result.fillVertical = fillVertical;
	return result;
}

void InputGroup::setSize(int width, int height) {
	this->width = width;
	this->height = height;
}

WidgetSize InputGroup::getSize() {
	WidgetSize result;
	result.width = minWidth;
	result.height = minHeight;
	result.fillHorizontal = fillHorizontal;
	result.fillVertical = fillVertical;
	return result;
}

void InputGroup::computeLayout() {
	inputLayer->clearLayers();
	borderLayer->clearLayers();

	/*spriteManager->setSpritePosition(topLeftId, posX - 5, posY + 4);
	spriteManager->setSpritePositionAndSize(topId, posX + 9, posY + 4, width - 18, 4);
	spriteManager->setSpritePosition(topRightId, posX + width - 9, posY + 4);

	spriteManager->setSpritePositionAndSize(leftId, posX - 5, posY - 9, 5, height - 18);
	spriteManager->setSpritePositionAndSize(rightId, posX + width, posY - 9, 5, height - 18);

	spriteManager->setSpritePosition(bottomLeftId, posX - 5, posY - height + 9);
	spriteManager->setSpritePositionAndSize(bottomId, posX + 9, posY - height, width - 18, 6);
	spriteManager->setSpritePosition(bottomRightId, posX + width - 9, posY - height + 9);

	spriteManager->flagSpriteForRender(topLeftId);
	spriteManager->flagSpriteForRender(topId);
	spriteManager->flagSpriteForRender(topRightId);
	spriteManager->flagSpriteForRender(leftId);
	spriteManager->flagSpriteForRender(rightId);
	spriteManager->flagSpriteForRender(bottomLeftId);
	spriteManager->flagSpriteForRender(bottomId);
	spriteManager->flagSpriteForRender(bottomRightId);*/

	spriteManager->setSpritePositionAndSize(centerId, posX + 1, posY - 1, width - 2, height - 2);
	spriteManager->flagSpriteForRender(centerId);

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


	for (unsigned int i = 0; i < rows.size(); ++i) {
		for (unsigned int j = 0; j < rows[i].size(); ++j) {
			int cWidth = width / rows[i].size() + 1;
			int offsetX = posX + j * (cWidth - 1);
			if (j == rows[i].size() - 1) {
				int rest = width % rows[i].size();
				cWidth += rest - 1;
			}
			rows[i][j]->setPosition(offsetX, posY - i * 19);
			rows[i][j]->setWidth(cWidth);
			rows[i][j]->updateSprites();
			rows[i][j]->prepare();
		}
	}

	for (unsigned int i = 0; i < horizontalSeperatorIds.size(); ++i) {
		spriteManager->setSpritePositionAndSize(horizontalSeperatorIds[i], posX, posY - (i + 1) * 19, width, 1);
		spriteManager->flagSpriteForRender(horizontalSeperatorIds[i]);
	}
}

PriorityWidget InputGroup::getHighestPriorityWidget(int x, int y, SDL_Event* event) {
	PriorityWidget result;
	result.priority = 1;
	result.widget = this;
	return result;
}

ActionHandler* InputGroup::createActionHandler(int x, int y, SDL_Event* event) {
	if (event->type == SDL_MOUSEBUTTONDOWN && event->button.button == SDL_BUTTON_LEFT &&
			 x > posX && x < posX + width && y < posY && y > posY - height) { // can't click on border
		for (unsigned int i = 0; i < rows.size(); ++i) {
			for (unsigned int j = 0; j < rows[i].size(); ++j) {
				if (rows[i][j]->contains(x, y)) {
					return new InputHandler(rows[i][j], spriteManager->getScreenHeight());
				}
			}
		}
	}
	return nullptr;
}

void InputGroup::addRow() {
	if (rows.size() == 0)
		minHeight += 20;
	else {
		minHeight += 19;
		unsigned int seperator = spriteManager->addSprite(0, 0, "Calamity/Gui/Images/input_seperator_middle.png", inputLayer.get());
		horizontalSeperatorIds.push_back(seperator);
	}
	height = minHeight;
	vector<Input*> newRow;
	rows.push_back(newRow);
}

Input* InputGroup::addInput(unsigned int row, wstring label, Properties* properties, Property* property) {
	/*int top = 1 ? (row == 0) : 0;
	int bottom = 2 ? (row == rows.size() - 1) : 0;

	int topLeft = top & (rows[row].size == 0);
	int bottomLeft = bottom & (rows[row].size == 0);
	int left = topLeft | bottomLeft;

	int right = top | bottom;*/

	Input* newInput = new Input(spriteManager, inputLayer.get(), label, fontId, properties, property);
	rows[row].push_back(newInput);
	return newInput;
}

void InputGroup::draw() {
	glEnable(GL_SCISSOR_TEST);
	glScissor(posX + 1, posY - height + 1, width - 2, height - 2);
	inputLayer->uploadLayerBuffers();
	inputLayer->draw();
	glDisable(GL_SCISSOR_TEST);

	borderLayer->uploadLayerBuffers();
	borderLayer->draw();
}

}
