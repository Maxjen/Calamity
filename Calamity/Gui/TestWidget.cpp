#include "TestWidget.h"

namespace clm {

TestWidget::TestWidget(SpriteManager* spriteManager, int width, int height, bool fillHorizontal, bool fillVertical) :
	renderLayer(nullptr),
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
	bottomRightId(-1)
{}

TestWidget::~TestWidget() {
	spriteManager->removeSprite(topLeftId);
	spriteManager->removeSprite(topId);
	spriteManager->removeSprite(topRightId);
	spriteManager->removeSprite(leftId);
	spriteManager->removeSprite(centerId);
	spriteManager->removeSprite(rightId);
	spriteManager->removeSprite(bottomLeftId);
	spriteManager->removeSprite(bottomId);
	spriteManager->removeSprite(bottomRightId);
}

void TestWidget::setRenderLayer(RenderLayer* renderLayer) {
	this->renderLayer = renderLayer;

	topLeftId = spriteManager->addSprite(0, 0, "Calamity/Gui/Images/window_top_left.png", renderLayer);
	topId = spriteManager->addSprite(0, 0, "Calamity/Gui/Images/window_top.png", renderLayer);
	topRightId = spriteManager->addSprite(0, 0, "Calamity/Gui/Images/window_top_right.png", renderLayer);

	leftId = spriteManager->addSprite(0, 0, "Calamity/Gui/Images/window_left.png", renderLayer);
	centerId = spriteManager->addSprite(0, 0, "Calamity/Gui/Images/window_center.png", renderLayer);
	rightId = spriteManager->addSprite(0, 0, "Calamity/Gui/Images/window_right.png", renderLayer);

	bottomLeftId = spriteManager->addSprite(0, 0, "Calamity/Gui/Images/window_bottom_left.png", renderLayer);
	bottomId = spriteManager->addSprite(0, 0, "Calamity/Gui/Images/window_bottom.png", renderLayer);
	bottomRightId = spriteManager->addSprite(0, 0, "Calamity/Gui/Images/window_bottom_right.png", renderLayer);
}

WidgetSize TestWidget::calculateMinSize() {
	WidgetSize result;
	result.width = minWidth;
	result.height = minHeight;
	result.fillHorizontal = fillHorizontal;
	result.fillVertical = fillVertical;
	return result;
}

void TestWidget::setSize(int width, int height) {
	this->width = width;
	this->height = height;
}

WidgetSize TestWidget::getSize() {
	WidgetSize result;
	result.width = minWidth;
	result.height = minHeight;
	result.fillHorizontal = fillHorizontal;
	result.fillVertical = fillVertical;
	return result;
}

void TestWidget::computeLayout() {
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

	spriteManager->setSpritePosition(topLeftId, posX - 10, posY + 10);
	spriteManager->setSpritePositionAndSize(topId, posX + 5, posY + 10, width - 10, 15);
	spriteManager->setSpritePosition(topRightId, posX + width - 5, posY + 10);

	spriteManager->setSpritePositionAndSize(leftId, posX - 10, posY - 5, 15, height - 10);
	spriteManager->setSpritePositionAndSize(centerId, posX + 5, posY - 5, width - 10, height - 10);
	spriteManager->setSpritePositionAndSize(rightId, posX + width - 5, posY - 5, 15, height - 10);

	spriteManager->setSpritePosition(bottomLeftId, posX - 10, posY - height + 5);
	spriteManager->setSpritePositionAndSize(bottomId, posX + 5, posY - height + 5, width - 10, 15);
	spriteManager->setSpritePosition(bottomRightId, posX + width - 5, posY - height + 5);

	spriteManager->flagSpriteForRender(topLeftId);
	spriteManager->flagSpriteForRender(topId);
	spriteManager->flagSpriteForRender(topRightId);
	spriteManager->flagSpriteForRender(leftId);
	spriteManager->flagSpriteForRender(centerId);
	spriteManager->flagSpriteForRender(rightId);
	spriteManager->flagSpriteForRender(bottomLeftId);
	spriteManager->flagSpriteForRender(bottomId);
	spriteManager->flagSpriteForRender(bottomRightId);
}

PriorityWidget TestWidget::getHighestPriorityWidget(int x, int y, SDL_Event* event) {
	PriorityWidget result;
	result.priority = 1;
	result.widget = this;
	return result;
}

void TestWidget::draw() {

}

}
