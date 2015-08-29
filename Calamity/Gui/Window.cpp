#include "Window.h"

namespace clm {

Window::Window(unsigned int windowId, SpriteManager* spriteManager, int posX, int posY, int width, int height, int flags) :
	windowId(windowId),
	zOrder(0),

	renderLayer(spriteManager->getRenderer()),
	shadowLayer(&renderLayer),

	spriteManager(spriteManager),
	screenHeight(spriteManager->getScreenHeight()),
	posX(posX),
	posY(screenHeight - posY),
	width(width),
	height(height),
	minWidth(0),
	minHeight(0),
	flags(flags),

	shadowTopLeft(-1),
	shadowTop(-1),
	shadowTopRight(-1),
	shadowLeft(-1),
	shadowRight(-1),
	shadowBottomLeft(-1),
	shadowBottom(-1),
	shadowBottomRight(-1),

	topId(-1),
	leftId(-1),
	centerId(-1),
	rightId(-1),
	bottomId(-1),

	hasBeenInitialized(false),
	padding(6),

	child(nullptr)
{}

Window::~Window() {
	spriteManager->removeSprite(shadowTopLeft);
	spriteManager->removeSprite(shadowTop);
	spriteManager->removeSprite(shadowTopRight);
	spriteManager->removeSprite(shadowLeft);
	spriteManager->removeSprite(shadowRight);
	spriteManager->removeSprite(shadowBottomLeft);
	spriteManager->removeSprite(shadowBottom);
	spriteManager->removeSprite(shadowBottomRight);

	//spriteManager->removeSprite(topLeftId);
	spriteManager->removeSprite(topId);
	//spriteManager->removeSprite(topRightId);
	spriteManager->removeSprite(leftId);
	spriteManager->removeSprite(centerId);
	spriteManager->removeSprite(rightId);
	//spriteManager->removeSprite(bottomLeftId);
	spriteManager->removeSprite(bottomId);
	//spriteManager->removeSprite(bottomRightId);

	delete child;
}

void Window::initialize() {
	// round
	/*topLeftId = spriteManager->addSprite(0, 0, "Calamity/Gui/Images/window_top_left.png", renderLayer);
	topId = spriteManager->addSprite(0, 0, "Calamity/Gui/Images/window_top.png", renderLayer);
	topRightId = spriteManager->addSprite(0, 0, "Calamity/Gui/Images/window_top_right.png", renderLayer);

	leftId = spriteManager->addSprite(0, 0, "Calamity/Gui/Images/window_left.png", renderLayer);
	centerId = spriteManager->addSprite(0, 0, "Calamity/Gui/Images/window_center.png", renderLayer);
	rightId = spriteManager->addSprite(0, 0, "Calamity/Gui/Images/window_right.png", renderLayer);

	bottomLeftId = spriteManager->addSprite(0, 0, "Calamity/Gui/Images/window_bottom_left.png", renderLayer);
	bottomId = spriteManager->addSprite(0, 0, "Calamity/Gui/Images/window_bottom.png", renderLayer);
	bottomRightId = spriteManager->addSprite(0, 0, "Calamity/Gui/Images/window_bottom_right.png", renderLayer);*/

	// shadow
	shadowTopLeft = spriteManager->addSprite(0, 0, "Calamity/Gui/Images/window_sq_sh_top_left.png", shadowLayer);
	shadowTop = spriteManager->addSprite(0, 0, "Calamity/Gui/Images/window_sq_sh_top.png", shadowLayer);
	shadowTopRight = spriteManager->addSprite(0, 0, "Calamity/Gui/Images/window_sq_sh_top_right.png", shadowLayer);

	shadowLeft = spriteManager->addSprite(0, 0, "Calamity/Gui/Images/window_sq_sh_left.png", shadowLayer);
	shadowRight = spriteManager->addSprite(0, 0, "Calamity/Gui/Images/window_sq_sh_right.png", shadowLayer);

	shadowBottomLeft = spriteManager->addSprite(0, 0, "Calamity/Gui/Images/window_sq_sh_bottom_left.png", shadowLayer);
	shadowBottom = spriteManager->addSprite(0, 0, "Calamity/Gui/Images/window_sq_sh_bottom.png", shadowLayer);
	shadowBottomRight = spriteManager->addSprite(0, 0, "Calamity/Gui/Images/window_sq_sh_bottom_right.png", shadowLayer);

	// window
	//topLeftId = spriteManager->addSprite(0, 0, "Calamity/Gui/Images/window_sq_top_left.png", renderLayer);
	topId = spriteManager->addSprite(0, 0, "Calamity/Gui/Images/window_sq_top.png", &renderLayer);
	//topRightId = spriteManager->addSprite(0, 0, "Calamity/Gui/Images/window_sq_top_right.png", renderLayer);

	leftId = spriteManager->addSprite(0, 0, "Calamity/Gui/Images/window_sq_left_right.png", &renderLayer);
	centerId = spriteManager->addSprite(0, 0, "Calamity/Gui/Images/window_center.png", &renderLayer);
	rightId = spriteManager->addSprite(0, 0, "Calamity/Gui/Images/window_sq_left_right.png", &renderLayer);

	//bottomLeftId = spriteManager->addSprite(0, 0, "Calamity/Gui/Images/window_sq_bottom_left.png", renderLayer);
	bottomId = spriteManager->addSprite(0, 0, "Calamity/Gui/Images/window_sq_bottom.png", &renderLayer);
	//bottomRightId = spriteManager->addSprite(0, 0, "Calamity/Gui/Images/window_sq_bottom_right.png", renderLayer);


	//printf("%d %d %d %d %d %d %d %d %d", topLeftId, topId, topRightId, leftId, centerId, rightId, bottomLeftId, bottomId, bottomRightId);

	/*spriteManager->addSprite(posX - 10, posY + 10, "Calamity/Gui/Images/window_top_left.png", renderLayer);
	spriteManager->addSprite(posX + 5, posY + 10, width - 10, 15, "Calamity/Gui/Images/window_top.png", renderLayer);
	spriteManager->addSprite(posX + width - 5, posY + 10, "Calamity/Gui/Images/window_top_right.png", renderLayer);

	spriteManager->addSprite(posX - 10, posY - 5, 15, height - 10, "Calamity/Gui/Images/window_left.png", renderLayer);
	spriteManager->addSprite(posX + 5, posY - 5, width - 10, height - 10, "Calamity/Gui/Images/window_center.png", renderLayer);
	spriteManager->addSprite(posX + width - 5, posY - 5, 15, height - 10, "Calamity/Gui/Images/window_right.png", renderLayer);

	spriteManager->addSprite(posX - 10, posY - height + 5, "Calamity/Gui/Images/window_bottom_left.png", renderLayer);
	spriteManager->addSprite(posX + 5, posY - height + 5, width - 10, 15, "Calamity/Gui/Images/window_bottom.png", renderLayer);
	spriteManager->addSprite(posX + width - 5, posY - height + 5, "Calamity/Gui/Images/window_bottom_right.png", renderLayer);*/

	/*spriteManager->addSprite(posX - 10, screenHeight - posY + 10, "Calamity/Gui/Images/window_top_left.png", renderLayer);
	spriteManager->addSprite(posX + 5, screenHeight - posY + 10, width - 10, 15, "Calamity/Gui/Images/window_top.png", renderLayer);
	spriteManager->addSprite(posX + width - 5, screenHeight - posY + 10, "Calamity/Gui/Images/window_top_right.png", renderLayer);

	spriteManager->addSprite(posX - 10, screenHeight - posY - 5, 15, height - 10, "Calamity/Gui/Images/window_left.png", renderLayer);
	spriteManager->addSprite(posX + 5, screenHeight - posY - 5, width - 10, height - 10, "Calamity/Gui/Images/window_center.png", renderLayer);
	spriteManager->addSprite(posX + width - 5, screenHeight - posY - 5, 15, height - 10, "Calamity/Gui/Images/window_right.png", renderLayer);

	spriteManager->addSprite(posX - 10, screenHeight - posY - height + 5, "Calamity/Gui/Images/window_bottom_left.png", renderLayer);
	spriteManager->addSprite(posX + 5, screenHeight - posY - height + 5, width - 10, 15, "Calamity/Gui/Images/window_bottom.png", renderLayer);
	spriteManager->addSprite(posX + width - 5, screenHeight - posY - height + 5, "Calamity/Gui/Images/window_bottom_right.png", renderLayer);*/
}

unsigned int Window::getWindowId() {
	return windowId;
}

int Window::getFlags() {
	return flags;
}

unsigned int Window::getZOrder() {
	return zOrder;
}

void Window::setShadowLayer(RenderLayer* shadowLayer) {
	this->shadowLayer = shadowLayer;
}

void Window::setZOrder(unsigned int zOrder) {
	this->zOrder = zOrder;
}

void Window::setPadding(int padding) {
	this->padding = padding;
}

void Window::setChild(Widget* child) {
	child->setRenderLayer(&renderLayer);
	this->child = child;
}

void Window::clear() {
	if (child) {
		delete child;
		child = nullptr;
		hasBeenInitialized = false;
	}
}

bool Window::contains(int x, int y) {
	// not 100% sure
	return (x >= posX && x < posX + width && y <= posY && y > posY - height);
}

/*PriorityWidget Window::getHighestPriorityWidget(int x, int y) {
	PriorityWidget result = child->getHighestPriorityWidget(x, y);
	if (result.priority > 0)
		return result;
	else {
		result.priority = 0;
		result.widget = nullptr;
		return result;
	}
}*/

ActionHandler* Window::createActionHandler(int x, int y, SDL_Event* event) {
	ActionHandler* result = nullptr;
	PriorityWidget priorityWidget;
	priorityWidget.priority = 0;
	if (child && child->contains(x, y)) {
		priorityWidget = child->getHighestPriorityWidget(x, y, event);
	}
	if (priorityWidget.priority > 0) {
		result = priorityWidget.widget->createActionHandler(x, y, event);
	}
	else if (event->type == SDL_MOUSEBUTTONDOWN && event->button.button == SDL_BUTTON_LEFT) {
		if ((x <= posX + 5) ||
			(x >= posX + width - 5) ||
			(y >= posY - 5) ||
			(y <= posY - height + 5)) {
			bool left = x <= posX + 5 && (flags & CLM_RESIZE_LEFT);
			bool scaleWidth = left || ((x >= posX + width - 5) && (flags & CLM_RESIZE_RIGHT));
			bool top = y >= posY - 5 && (flags & CLM_RESIZE_TOP);
			bool scaleHeight = top || ((y <= posY - height + 5) && (flags & CLM_RESIZE_BOTTOM));
			if (scaleWidth || scaleHeight)
				result = new ScaleWindowHandler(x, -(y - screenHeight), minWidth, minHeight, &width, &height, &posX, &posY, scaleWidth, scaleHeight, left, top);
		}
		else if (!(flags & CLM_NO_MOVE)) {
			result = new MoveWindowHandler(&posX, &posY);
		}
	}
	return result;
}

void Window::update() {
	renderLayer.clearLayers();

	WidgetSize childSize;
	if (child) {
		childSize = child->calculateMinSize();
		minWidth = childSize.width + 2 * padding;
		minHeight = childSize.height + 2 * padding;
		width = std::max(width, minWidth);
		height = std::max(height, minHeight);
		/*if (childSize.fillHorizontal || childSize.fillVertical) {
			int childWidth = childSize.width;
			int childHeight = childSize.height;
			if (childSize.fillHorizontal)
				childWidth = std::max(width - 2 * padding, childSize.width);
			if (childSize.fillVertical)
				childHeight = std::max(height - 2 * padding, childSize.height);
			child->setSize(childWidth, childHeight);
			if (!hasBeenInitialized) {
				child->initialize();
				spriteManager->uploadTextures();
				hasBeenInitialized = true;
			}
		}
		child->setPosition(posX + padding, posY - padding);
		child->computeLayout();*/
		//child->draw();
	}

	// round
	/*spriteManager->setSpritePosition(topLeftId, posX - 10, posY + 10);
	spriteManager->setSpritePositionAndSize(topId, posX + 5, posY + 10, width - 10, 15);
	spriteManager->setSpritePosition(topRightId, posX + width - 5, posY + 10);

	spriteManager->setSpritePositionAndSize(leftId, posX - 10, posY - 5, 15, height - 10);
	spriteManager->setSpritePositionAndSize(centerId, posX + 5, posY - 5, width - 10, height - 10);
	spriteManager->setSpritePositionAndSize(rightId, posX + width - 5, posY - 5, 15, height - 10);

	spriteManager->setSpritePosition(bottomLeftId, posX - 10, posY - height + 5);
	spriteManager->setSpritePositionAndSize(bottomId, posX + 5, posY - height + 5, width - 10, 15);
	spriteManager->setSpritePosition(bottomRightId, posX + width - 5, posY - height + 5);*/

	// sq with shadow
	/*spriteManager->setSpritePosition(topLeftId, posX - 20, posY + 19);
	spriteManager->setSpritePositionAndSize(topId, posX + 20, posY + 19, width - 40, 20);
	spriteManager->setSpritePosition(topRightId, posX + width - 20, posY + 19);

	spriteManager->setSpritePositionAndSize(leftId, posX - 20, posY - 20, 21, height - 40);
	spriteManager->setSpritePositionAndSize(centerId, posX + 1, posY - 1, width - 2, height - 2);
	spriteManager->setSpritePositionAndSize(rightId, posX + width - 1, posY - 20, 21, height - 40);

	spriteManager->setSpritePosition(bottomLeftId, posX - 20, posY - height + 20);
	spriteManager->setSpritePositionAndSize(bottomId, posX + 20, posY - height + 1, width - 40, 22);
	spriteManager->setSpritePosition(bottomRightId, posX + width - 20, posY - height + 20);

	spriteManager->flagSpriteForRender(topLeftId);
	spriteManager->flagSpriteForRender(topId);
	spriteManager->flagSpriteForRender(topRightId);
	spriteManager->flagSpriteForRender(leftId);
	spriteManager->flagSpriteForRender(centerId);
	spriteManager->flagSpriteForRender(rightId);
	spriteManager->flagSpriteForRender(bottomLeftId);
	spriteManager->flagSpriteForRender(bottomId);
	spriteManager->flagSpriteForRender(bottomRightId);*/


	spriteManager->setSpritePosition(shadowTopLeft, posX - 20, posY + 19);
	spriteManager->setSpritePositionAndSize(shadowTop, posX + 20, posY + 19, width - 40, 20);
	spriteManager->setSpritePosition(shadowTopRight, posX + width - 20, posY + 19);

	spriteManager->setSpritePositionAndSize(shadowLeft, posX - 20, posY - 20, 21, height - 40);
	spriteManager->setSpritePositionAndSize(shadowRight, posX + width - 1, posY - 20, 21, height - 40);

	spriteManager->setSpritePosition(shadowBottomLeft, posX - 20, posY - height + 20);
	spriteManager->setSpritePositionAndSize(shadowBottom, posX + 20, posY - height + 1, width - 40, 22);
	spriteManager->setSpritePosition(shadowBottomRight, posX + width - 20, posY - height + 20);


	spriteManager->setSpritePositionAndSize(topId, posX, posY, width, 1);
	spriteManager->setSpritePositionAndSize(leftId, posX, posY - 1, 1, height - 2);
	spriteManager->setSpritePositionAndSize(centerId, posX + 1, posY - 1, width - 2, height - 2);
	spriteManager->setSpritePositionAndSize(rightId, posX + width - 1, posY - 1, 1, height - 2);
	spriteManager->setSpritePositionAndSize(bottomId, posX, posY - height + 1, width, 1);


	spriteManager->flagSpriteForRender(shadowTopLeft);
	spriteManager->flagSpriteForRender(shadowTop);
	spriteManager->flagSpriteForRender(shadowTopRight);
	spriteManager->flagSpriteForRender(shadowLeft);
	spriteManager->flagSpriteForRender(shadowRight);
	spriteManager->flagSpriteForRender(shadowBottomLeft);
	spriteManager->flagSpriteForRender(shadowBottom);
	spriteManager->flagSpriteForRender(shadowBottomRight);

	spriteManager->flagSpriteForRender(topId);
	spriteManager->flagSpriteForRender(leftId);
	spriteManager->flagSpriteForRender(centerId);
	spriteManager->flagSpriteForRender(rightId);
	spriteManager->flagSpriteForRender(bottomId);


	if (child) {
		if (childSize.fillHorizontal || childSize.fillVertical) {
			int childWidth = childSize.width;
			int childHeight = childSize.height;
			if (childSize.fillHorizontal)
				childWidth = width - 2 * padding;
			if (childSize.fillVertical)
				childHeight = height - 2 * padding;
			child->setSize(childWidth, childHeight);
			if (!hasBeenInitialized) {
				child->initialize();
				spriteManager->uploadTextures();
				hasBeenInitialized = true;
			}
		}
		child->setPosition(posX + padding, posY - padding);
		child->computeLayout();
	}
}

void Window::draw() {
	renderLayer.uploadLayerBuffers();
	renderLayer.draw();

	if (child)
		child->draw();
}

}
