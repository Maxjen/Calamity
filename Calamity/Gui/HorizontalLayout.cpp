#include "HorizontalLayout.h"

namespace clm {

HorizontalLayout::HorizontalLayout(SpriteManager* spriteManager, int width, int height, bool fillHorizontal, bool fillVertical) :
	renderLayer(nullptr),
	spriteManager(spriteManager),
	fillHorizontal(fillHorizontal),
	fillVertical(fillVertical),
	horizontalSpaceToDistribute(0),
	horizontalFillChildCount(0),

	padding(6),
	marginLeft(0),
	marginRight(0),
	marginTop(0),
	marginBottom(0),

	children()
{
	// TODO: actually use width and height
	/*this->width = width;
	this->height = height;*/
}

HorizontalLayout::~HorizontalLayout() {
	for (auto& it : children)
		delete it;
}

void HorizontalLayout::initialize() {
	for (auto& it : children)
		it->initialize();
}

void HorizontalLayout::setMargin(int left, int right, int top, int bottom) {
	marginLeft = left;
	marginRight = right;
	marginTop = top;
	marginBottom = bottom;
}

void HorizontalLayout::setRenderLayer(RenderLayer* renderLayer) {
	this->renderLayer = renderLayer;
}

WidgetSize HorizontalLayout::calculateMinSize() {
	width = 0;
	height = 0;
	horizontalFillChildCount = 0;

	WidgetSize cWidget;
	for (auto& it : children) {
		cWidget = it->calculateMinSize();
		if (cWidget.fillHorizontal)
			++horizontalFillChildCount;
		height = std::max(height, cWidget.height);
		width += cWidget.width + padding;
	}
	width -= padding;
	width += marginLeft + marginRight;
	width = std::max(width, 0);

	height += marginTop + marginBottom;

	WidgetSize result;
	result.width = width;
	result.height = height;
	result.fillHorizontal = fillHorizontal;
	result.fillVertical = fillVertical;
	return result;
}

void HorizontalLayout::setSize(int width, int height) {
	horizontalSpaceToDistribute = width - this->width;
	this->width = width;
	this->height = height;
}

WidgetSize HorizontalLayout::getSize() {
	WidgetSize result;
	result.width = width;
	result.height = height;
	result.fillHorizontal = fillHorizontal;
	result.fillVertical = fillVertical;
	return result;
}

void HorizontalLayout::computeLayout() {
	WidgetSize cWidget;
	int cPosX = posX + marginLeft, cPosY = posY - marginTop;
	bool restAdded = false;
	for (auto& it : children) {
		it->setPosition(cPosX, cPosY);
		cWidget = it->getSize();
		if (cWidget.fillHorizontal || cWidget.fillVertical) {
			int cWidth = cWidget.width;
			int cHeight = cWidget.height;
			if (cWidget.fillHorizontal) {
				cWidth = cWidget.width + horizontalSpaceToDistribute / horizontalFillChildCount;
				if (!restAdded) {
					cWidth += horizontalSpaceToDistribute % horizontalFillChildCount;
					restAdded = true;
				}
			}
			if (cWidget.fillVertical)
				cHeight = height - marginTop - marginBottom;
			it->setSize(cWidth, cHeight);
			cPosX += cWidth + padding;
		}
		else {
			cPosX += cWidget.width + padding;
		}
		it->computeLayout();
	}
}

void HorizontalLayout::addChild(Widget* child) {
	if (renderLayer) {
		child->setRenderLayer(renderLayer);
		children.push_back(child);
	}
	else
		printf("Can't add child to HorizontalLayout because it has no RenderLayer!\n");

}

PriorityWidget HorizontalLayout::getHighestPriorityWidget(int x, int y, SDL_Event* event) {
	PriorityWidget result;
	result.priority = 0;
	result.widget = this;
	for (auto& it : children) {
		if (it->contains(x, y))
			return it->getHighestPriorityWidget(x, y, event);
	}
	return result;
}

void HorizontalLayout::draw() {
	for (auto& it : children)
		it->draw();
}

}
