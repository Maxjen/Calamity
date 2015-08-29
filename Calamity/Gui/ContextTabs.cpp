#include "ContextTabs.h"

namespace clm {

ContextTab::ContextTab(SpriteManager* spriteManager, unsigned int contextId, bool isActive, wstring label, unsigned int fontId) :
	spriteManager(spriteManager),
	contextId(contextId),
	isActive(isActive),
	posX(0),
	width(0),
	label(label),
	fontId(fontId),

	left(-1),
	middle(-1),
	right(-1),
	textId(-1)
{}

ContextTab::~ContextTab() {
	spriteManager->removeSprite(left);
	spriteManager->removeSprite(middle);
	spriteManager->removeSprite(right);
	spriteManager->removeText(textId);
}

void ContextTab::initialize(RenderLayer* renderLayer) {
	if (isActive) {
		left = spriteManager->addSprite(0, 0, "Calamity/Gui/Images/context_tab_selected_left.png", renderLayer);
		middle = spriteManager->addSprite(0, 0, "Calamity/Gui/Images/context_tab_selected_middle.png", renderLayer);
		right = spriteManager->addSprite(0, 0, "Calamity/Gui/Images/context_tab_selected_right.png", renderLayer);
	}
	else {
		left = spriteManager->addSprite(0, 0, "Calamity/Gui/Images/context_tab_deselected_left.png", renderLayer);
		middle = spriteManager->addSprite(0, 0, "Calamity/Gui/Images/context_tab_deselected_middle.png", renderLayer);
		right = spriteManager->addSprite(0, 0, "Calamity/Gui/Images/context_tab_deselected_right.png", renderLayer);
	}
	textId = spriteManager->addText(label, fontId, 0, 0, 255, 255, 255, 255, renderLayer, true);
	width  = spriteManager->getTextWidth(textId) + 22;
}

unsigned int ContextTab::getContextId() {
	return contextId;
}

bool ContextTab::getIsActive() {
	return isActive;
}

void ContextTab::setPosX(int posX) {
	this->posX = posX;
}

int ContextTab::getPosX() {
	return posX;
}

int ContextTab::getWidth() {
	return width;
}

void ContextTab::draw(int posX, int posY) {
	if (isActive) {
		spriteManager->setSpritePosition(left, posX - 5, posY);
		spriteManager->setSpritePositionAndSize(middle, posX + 8, posY, width - 16, 25);
		spriteManager->setSpritePosition(right, posX + width - 8, posY);
	}
	else {
		spriteManager->setSpritePosition(left, posX - 5, posY);
		spriteManager->setSpritePositionAndSize(middle, posX + 8, posY, width - 16, 24);
		spriteManager->setSpritePosition(right, posX + width - 8, posY);
	}
	spriteManager->flagSpriteForRender(left);
	spriteManager->flagSpriteForRender(middle);
	spriteManager->flagSpriteForRender(right);

	spriteManager->setTextPosition(textId, posX + 10, posY - 18);
	spriteManager->flagTextForRender(textId);
}

ContextTabs::ContextTabs(SpriteManager* spriteManager, ContextManager* contextManager, unsigned int activeContextId, int width, bool fillHorizontal, bool fillVertical) :
	renderLayer(nullptr),
	spriteManager(spriteManager),
	contextManager(contextManager),
	activeContextId(activeContextId),
	minWidth(width),
	minHeight(25),
	fillHorizontal(fillHorizontal),
	fillVertical(fillVertical),

	bg(-1),
	left(-1),
	right(-1),

	tabs()
{}

ContextTabs::~ContextTabs() {
	for (auto& it : tabs)
		delete it;

	spriteManager->removeSprite(bg);
	spriteManager->removeSprite(left);
	spriteManager->removeSprite(right);
	/*spriteManager->removeSprite(topLeftId);
	spriteManager->removeSprite(topId);
	spriteManager->removeSprite(topRightId);
	spriteManager->removeSprite(leftId);
	spriteManager->removeSprite(centerId);
	spriteManager->removeSprite(rightId);
	spriteManager->removeSprite(bottomLeftId);
	spriteManager->removeSprite(bottomId);
	spriteManager->removeSprite(bottomRightId);

	delete itemLayer;
	delete borderLayer;*/
}

void ContextTabs::setRenderLayer(RenderLayer* renderLayer) {
	this->renderLayer = renderLayer;

	for (auto& it : tabs)
		it->initialize(renderLayer);

	bg = spriteManager->addSprite(0, 0, "Calamity/Gui/Images/context_tabs_bg.png", renderLayer);
	left = spriteManager->addSprite(0, 0, "Calamity/Gui/Images/context_tabs_border.png", renderLayer);
	right = spriteManager->addSprite(0, 0, "Calamity/Gui/Images/context_tabs_border.png", renderLayer);

	/*topLeftId = spriteManager->addSprite(posX - 10, posY + 10, "Calamity/Gui/Images/window_top_left.png", renderLayer);
	topId = spriteManager->addSprite(posX + 5, posY + 10, width - 10, 15, "Calamity/Gui/Images/window_top.png", renderLayer);
	topRightId = spriteManager->addSprite(posX + width - 5, posY + 10, "Calamity/Gui/Images/window_top_right.png", renderLayer);

	leftId = spriteManager->addSprite(posX - 10, posY - 5, 15, height - 10, "Calamity/Gui/Images/window_left.png", renderLayer);
	centerId = spriteManager->addSprite(posX + 5, posY - 5, width - 10, height - 10, "Calamity/Gui/Images/window_center.png", renderLayer);
	rightId = spriteManager->addSprite(posX + width - 5, posY - 5, 15, height - 10, "Calamity/Gui/Images/window_right.png", renderLayer);

	bottomLeftId = spriteManager->addSprite(posX - 10, posY - height + 5, "Calamity/Gui/Images/window_bottom_left.png", renderLayer);
	bottomId = spriteManager->addSprite(posX + 5, posY - height + 5, width - 10, 15, "Calamity/Gui/Images/window_bottom.png", renderLayer);
	bottomRightId = spriteManager->addSprite(posX + width - 5, posY - height + 5, "Calamity/Gui/Images/window_bottom_right.png", renderLayer);*/

	/*topLeftId = spriteManager->addSprite(posX - 10, posY + 10, "Calamity/Gui/Images/list_view_top_left.png", renderLayer);
	topId = spriteManager->addSprite(posX + 5, posY + 10, width - 10, 15, "Calamity/Gui/Images/list_view_top.png", renderLayer);
	topRightId = spriteManager->addSprite(posX + width - 5, posY + 10, "Calamity/Gui/Images/list_view_top_right.png", renderLayer);

	leftId = spriteManager->addSprite(posX - 10, posY - 5, 15, height - 10, "Calamity/Gui/Images/list_view_left.png", renderLayer);
	centerId = spriteManager->addSprite(posX + 5, posY - 5, width - 10, height - 10, "Calamity/Gui/Images/list_view_center.png", renderLayer);
	rightId = spriteManager->addSprite(posX + width - 5, posY - 5, 15, height - 10, "Calamity/Gui/Images/list_view_right.png", renderLayer);

	bottomLeftId = spriteManager->addSprite(posX - 10, posY - height + 5, "Calamity/Gui/Images/list_view_bottom_left.png", renderLayer);
	bottomId = spriteManager->addSprite(posX + 5, posY - height + 5, width - 10, 15, "Calamity/Gui/Images/list_view_bottom.png", renderLayer);
	bottomRightId = spriteManager->addSprite(posX + width - 5, posY - height + 5, "Calamity/Gui/Images/list_view_bottom_right.png", renderLayer);*/

	/*centerId = spriteManager->addSprite(posX + 5, posY - 5, width - 10, height - 10, "Calamity/Gui/Images/list_view_center.png", itemLayer);

	topLeftId = spriteManager->addSprite(posX - 10, posY + 10, "Calamity/Gui/Images/list_view_top_left.png", borderLayer);
	topId = spriteManager->addSprite(posX + 5, posY + 10, width - 10, 15, "Calamity/Gui/Images/list_view_top.png", borderLayer);
	topRightId = spriteManager->addSprite(posX + width - 5, posY + 10, "Calamity/Gui/Images/list_view_top_right.png", borderLayer);

	leftId = spriteManager->addSprite(posX - 10, posY - 5, 15, height - 10, "Calamity/Gui/Images/list_view_left.png", borderLayer);
	//centerId = spriteManager->addSprite(posX + 5, posY - 5, width - 10, height - 10, "Calamity/Gui/Images/list_view_center.png", borderLayer);
	rightId = spriteManager->addSprite(posX + width - 5, posY - 5, 15, height - 10, "Calamity/Gui/Images/list_view_right.png", borderLayer);

	bottomLeftId = spriteManager->addSprite(posX - 10, posY - height + 5, "Calamity/Gui/Images/list_view_bottom_left.png", borderLayer);
	bottomId = spriteManager->addSprite(posX + 5, posY - height + 5, width - 10, 15, "Calamity/Gui/Images/list_view_bottom.png", borderLayer);
	bottomRightId = spriteManager->addSprite(posX + width - 5, posY - height + 5, "Calamity/Gui/Images/list_view_bottom_right.png", borderLayer);*/
}

WidgetSize ContextTabs::calculateMinSize() {
	WidgetSize result;
	result.width = minWidth;
	result.height = minHeight;
	result.fillHorizontal = fillHorizontal;
	result.fillVertical = fillVertical;
	return result;
}

void ContextTabs::setSize(int width, int height) {
	this->width = width;
	this->height = height;
}

WidgetSize ContextTabs::getSize() {
	WidgetSize result;
	result.width = minWidth;
	result.height = minHeight;
	result.fillHorizontal = fillHorizontal;
	result.fillVertical = fillVertical;
	return result;
}

void ContextTabs::computeLayout() {
	spriteManager->setSpritePositionAndSize(bg, posX, posY, width, height);
	spriteManager->flagSpriteForRender(bg);

	int cX = posX + 10;

	int leftEnd = 0, rightStart = 0;
	for (auto& it : tabs) {
		it->setPosX(cX);
		it->draw(cX, posY);
		if (it->getIsActive()) {
			leftEnd = cX - posX - 5;
			rightStart = cX + it->getWidth() + 5;
		}
		cX += it->getWidth() + 10;
	}

	spriteManager->setSpritePositionAndSize(left, posX, posY - 20, leftEnd, 5);
	spriteManager->setSpritePositionAndSize(right, rightStart, posY - 20, width - rightStart, 5);
	spriteManager->flagSpriteForRender(left);
	spriteManager->flagSpriteForRender(right);

	/*if (!items.empty()) {
		maxScrollAmount = std::max(0, (int)items.size() * items[0]->getHeight() - height + 2);
		scrollAmount = std::min(scrollAmount, maxScrollAmount);
		scrollAmount = std::max(0, scrollAmount);
	}*/

	/*WidgetSize result;
	result.width = width;
	result.height = height;
	return result;*/

	// no shadow
	/*spriteManager->setSpritePosition(topLeftId, posX, posY);
	spriteManager->setSpritePositionAndSize(topId, posX + 3, posY, width - 6, 3);
	spriteManager->setSpritePosition(topRightId, posX + width - 3, posY);

	spriteManager->setSpritePositionAndSize(leftId, posX, posY - 3, 3, height - 6);
	spriteManager->setSpritePositionAndSize(centerId, posX + 3, posY - 3, width - 6, height - 6);
	spriteManager->setSpritePositionAndSize(rightId, posX + width - 3, posY - 3, 3, height - 6);

	spriteManager->setSpritePosition(bottomLeftId, posX, posY - height + 3);
	spriteManager->setSpritePositionAndSize(bottomId, posX + 3, posY - height + 3, width - 6, 3);
	spriteManager->setSpritePosition(bottomRightId, posX + width - 3, posY - height + 3);*/

	// shadow1
	/*spriteManager->setSpritePosition(topLeftId, posX, posY);
	spriteManager->setSpritePositionAndSize(topId, posX + 6, posY, width - 12, 7);
	spriteManager->setSpritePosition(topRightId, posX + width - 6, posY);

	spriteManager->setSpritePositionAndSize(leftId, posX, posY - 7, 6, height - 12);
	spriteManager->setSpritePositionAndSize(centerId, posX + 6, posY - 7, width - 12, height - 12);
	spriteManager->setSpritePositionAndSize(rightId, posX + width - 6, posY - 7, 6, height - 12);

	spriteManager->setSpritePosition(bottomLeftId, posX, posY - height + 5);
	spriteManager->setSpritePositionAndSize(bottomId, posX + 6, posY - height + 5, width - 12, 5);
	spriteManager->setSpritePosition(bottomRightId, posX + width - 6, posY - height + 5);*/



	/*spriteManager->setSpritePosition(topLeftId, posX, posY);
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


	spriteManager->setSpritePositionAndSize(centerId, posX + 1, posY - 1, width - 2, height - 2);
	spriteManager->flagSpriteForRender(centerId);*/

	/*if (!items.empty()) {
		unsigned int iStart = scrollAmount / items[0]->getHeight();
		unsigned int iEnd = std::min((int)(iStart + (height - 2) / items[0]->getHeight() + 1), (int)items.size());
		for (unsigned int i = iStart; i < iEnd; ++i)
			items[i]->draw(itemLayer, spriteManager, posX + 1, scrollAmount + posY - i * items[i]->getHeight() - 1, width - 2);
	}*/

	/*if ((int)items.size() * items[0]->getHeight() > height) {
		maxScrollAmount = (int)items.size() * items[0]->getHeight() - height;
		printf("scrollable\n");
	}*/
}

PriorityWidget ContextTabs::getHighestPriorityWidget(int x, int y, SDL_Event* event) {
	PriorityWidget result;
	result.priority = 0;
	result.widget = this;
	// TODO: adjust
	if (y <= posY && y >= posY - height) {
		for (auto& it : tabs) {
			int tabStart = it->getPosX();
			int tabEnd = tabStart + it->getWidth();
			if (x >= tabStart && x <= tabEnd) {
				result.priority = 2;
				break;
			}
		}
	}
	return result;
}

ActionHandler* ContextTabs::createActionHandler(int x, int y, SDL_Event* event) {
	/*if (event->type == SDL_MOUSEWHEEL)
		scrollAmount -= event->wheel.y * 10;
	else if (event->type == SDL_MOUSEBUTTONDOWN && event->button.button == SDL_BUTTON_LEFT && (!items.empty()) &&
			 x > posX && x < posX + width && y < posY && y > posY - height) { // can't click on border
		unsigned int item = ((posY - 1) - y + scrollAmount) / items[0]->getHeight();
		int relY = items[0]->getHeight() - (((posY - 1) - y + scrollAmount) % items[0]->getHeight());
		if (item < items.size())
			items[item]->handleEvent(x - posX - 1, relY, itemLayer, spriteManager, event);
	}
	else if (event->type == SDL_KEYDOWN && event->key.keysym.sym == SDLK_a && (!items.empty())) {
		items[0]->handleEvent(0, 0, itemLayer, spriteManager, event);
	}*/

	if (event->type == SDL_MOUSEBUTTONDOWN && event->button.button == SDL_BUTTON_LEFT) {
		for (auto& it : tabs) {
			int tabStart = it->getPosX();
			int tabEnd = tabStart + it->getWidth();
			if (x >= tabStart && x <= tabEnd) {
				contextManager->setActiveContext(it->getContextId());
				break;
			}
		}
	}

	return nullptr;
}

/*void ListView::addItem(ListViewItem* item) {
	items.push_back(item);
	item->initialize(itemLayer, spriteManager);
}*/

void ContextTabs::addTab(ContextTab* tab) {
	tabs.push_back(tab);
}

void ContextTabs::draw() {
	/*glEnable(GL_SCISSOR_TEST);
	glScissor(posX + 1, posY - height + 1, width - 2, height - 2);
	itemLayer->uploadLayerBuffers();
	itemLayer->draw();
	itemLayer->clearLayers();
	glDisable(GL_SCISSOR_TEST);

	borderLayer->uploadLayerBuffers();
	borderLayer->draw();
	borderLayer->clearLayers();*/
}

}
