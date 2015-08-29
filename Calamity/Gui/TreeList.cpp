#include "TreeList.h"

namespace clm {

void TreeListItem::addNonCollapsedChildren(unordered_map<unsigned int, TreeListItem*> items,
										   vector<unsigned int> &nonCollapsedItems) {
	if (!bCollapsed) {
		for (auto& it : children) {
			nonCollapsedItems.push_back(it);
			items[it]->addNonCollapsedChildren(items, nonCollapsedItems);
		}
	}
}

TreeList::TreeList(SpriteManager* spriteManager, int width, int height, bool fillHorizontal, bool fillVertical) :
	renderLayer(nullptr),
	itemLayer(),
	borderLayer(),

	spriteManager(spriteManager),
	minWidth(width),
	minHeight(height),
	fillHorizontal(fillHorizontal),
	fillVertical(fillVertical),

	topLeftId(-1),
	topId(-1),
	topRightId(-1),
	leftId(-1),
	centerId(-1),
	rightId(-1),
	bottomLeftId(-1),
	bottomId(-1),
	bottomRightId(-1),

	items(),
	cItem(0),

	topLevelItems(),

	nonCollapsedItems(),

	scrollAmount(0),
	maxScrollAmount(0),

	loadFunctor(nullptr)
{
	itemLayer.reset(new RenderLayer(spriteManager->getRenderer()));
	borderLayer.reset(new RenderLayer(spriteManager->getRenderer()));
}

TreeList::~TreeList() {
	for (auto& it : items)
		delete it.second;

	spriteManager->removeSprite(topLeftId);
	spriteManager->removeSprite(topId);
	spriteManager->removeSprite(topRightId);
	spriteManager->removeSprite(leftId);
	spriteManager->removeSprite(centerId);
	spriteManager->removeSprite(rightId);
	spriteManager->removeSprite(bottomLeftId);
	spriteManager->removeSprite(bottomId);
	spriteManager->removeSprite(bottomRightId);

	//delete itemLayer;
	//delete borderLayer;
}

void TreeList::setLoadFunctor(Functor* loadFunctor) {
	this->loadFunctor = loadFunctor;
}

void TreeList::reload() {
	if (loadFunctor)
		(*loadFunctor)();
}

void TreeList::setRenderLayer(RenderLayer* renderLayer) {
	this->renderLayer = renderLayer;

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

	centerId = spriteManager->addSprite(0, 0, "Calamity/Gui/Images/list_view_center.png", itemLayer.get());

	topLeftId = spriteManager->addSprite(0, 0, "Calamity/Gui/Images/list_view_top_left.png", borderLayer.get());
	topId = spriteManager->addSprite(0, 0, "Calamity/Gui/Images/list_view_top.png", borderLayer.get());
	topRightId = spriteManager->addSprite(0, 0, "Calamity/Gui/Images/list_view_top_right.png", borderLayer.get());

	leftId = spriteManager->addSprite(0, 0, "Calamity/Gui/Images/list_view_left.png", borderLayer.get());
	//centerId = spriteManager->addSprite(posX + 5, posY - 5, width - 10, height - 10, "Calamity/Gui/Images/list_view_center.png", borderLayer);
	rightId = spriteManager->addSprite(0, 0, "Calamity/Gui/Images/list_view_right.png", borderLayer.get());

	bottomLeftId = spriteManager->addSprite(0, 0, "Calamity/Gui/Images/list_view_bottom_left.png", borderLayer.get());
	bottomId = spriteManager->addSprite(0, 0, "Calamity/Gui/Images/list_view_bottom.png", borderLayer.get());
	bottomRightId = spriteManager->addSprite(0, 0, "Calamity/Gui/Images/list_view_bottom_right.png", borderLayer.get());
}

WidgetSize TreeList::calculateMinSize() {
	WidgetSize result;
	result.width = minWidth;
	result.height = minHeight;
	result.fillHorizontal = fillHorizontal;
	result.fillVertical = fillVertical;
	return result;
}

void TreeList::setSize(int width, int height) {
	this->width = width;
	this->height = height;
}

WidgetSize TreeList::getSize() {
	WidgetSize result;
	result.width = minWidth;
	result.height = minHeight;
	result.fillHorizontal = fillHorizontal;
	result.fillVertical = fillVertical;
	return result;
}

void TreeList::computeLayout() {
	itemLayer->clearLayers();
	borderLayer->clearLayers();

	nonCollapsedItems.clear();

	for (auto& it : topLevelItems) {
		nonCollapsedItems.push_back(it);
		items[it]->addNonCollapsedChildren(items, nonCollapsedItems);
	}

	for (unsigned int i = 0; i < nonCollapsedItems.size(); ++i)
		items[nonCollapsedItems[i]]->setColor((i + 1) % 2);

	if (!nonCollapsedItems.empty()) {
		maxScrollAmount = std::max(0, (int)nonCollapsedItems.size() * items[0]->getHeight() - height + 2);
		scrollAmount = std::min(scrollAmount, maxScrollAmount);
		scrollAmount = std::max(0, scrollAmount);
	}

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


	if (!nonCollapsedItems.empty()) {
		int itemHeight = items[nonCollapsedItems[0]]->getHeight();
		unsigned int iStart = scrollAmount / itemHeight;
		unsigned int iEnd = std::min((int)(iStart + (height - 2) / itemHeight + 1), (int)nonCollapsedItems.size());
		for (unsigned int i = iStart; i < iEnd; ++i)
			items[nonCollapsedItems[i]]->draw(posX + 1, scrollAmount + posY - i * itemHeight - 1, width - 2);
	}

	/*if ((int)items.size() * items[0]->getHeight() > height) {
		maxScrollAmount = (int)items.size() * items[0]->getHeight() - height;
		printf("scrollable\n");
	}*/
}

PriorityWidget TreeList::getHighestPriorityWidget(int x, int y, SDL_Event* event) {
	PriorityWidget result;
	result.priority = 2;
	result.widget = this;
	return result;
}

ActionHandler* TreeList::createActionHandler(int x, int y, SDL_Event* event) {
	if (event->type == SDL_MOUSEWHEEL)
		scrollAmount -= event->wheel.y * 10;
	else if (event->type == SDL_MOUSEBUTTONDOWN && event->button.button == SDL_BUTTON_LEFT && (!items.empty()) &&
			 x > posX && x < posX + width && y < posY && y > posY - height) { // can't click on border
		int itemHeight = items[nonCollapsedItems[0]]->getHeight();
		unsigned int item = ((posY - 1) - y + scrollAmount) / itemHeight;
		int relY = itemHeight - (((posY - 1) - y + scrollAmount) % itemHeight);
		if (item < nonCollapsedItems.size())
			items[nonCollapsedItems[item]]->handleEvent(x - posX - 1, relY, event);
	}
	else if (event->type == SDL_KEYDOWN && event->key.keysym.sym == SDLK_a && (!items.empty())) {
		items[nonCollapsedItems[0]]->handleEvent(0, 0, event);
	}

	return nullptr;
}

unsigned int TreeList::addItem(TreeListItem* item) {
	/*items.push_back(item);
	item->initialize(itemLayer.get(), cItemColor);
	cItemColor = !cItemColor;*/
	items[cItem] = item;
	topLevelItems.push_back(cItem);
	item->initialize(itemLayer.get(), 0);
	return cItem++;
}

unsigned int TreeList::addItemAsChild(TreeListItem* item, unsigned int parent) {
	items[cItem] = item;
	items[parent]->addChild(cItem);
	item->initialize(itemLayer.get(), items[parent]->getDepth() + 1);
	return cItem++;
}

void TreeList::clear() {
	for (auto& it : items)
		delete it.second;
	items.clear();

	cItem = 0;

	topLevelItems.clear();
}

void TreeList::draw() {
	glEnable(GL_SCISSOR_TEST);
	glScissor(posX + 1, posY - height + 1, width - 2, height - 2);
	itemLayer->uploadLayerBuffers();
	itemLayer->draw();

	glDisable(GL_SCISSOR_TEST);

	borderLayer->uploadLayerBuffers();
	borderLayer->draw();
}

}
