#include "VerticalLayout.h"

namespace clm {

VerticalLayout::VerticalLayout(SpriteManager* spriteManager, int width, int height, bool fillHorizontal, bool fillVertical) :
	renderLayer(nullptr),
	spriteManager(spriteManager),
	fillHorizontal(fillHorizontal),
	fillVertical(fillVertical),
	verticalSpaceToDistribute(0),
	verticalFillChildCount(0),

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

VerticalLayout::~VerticalLayout() {
	for (auto& it : children)
		delete it;
}

void VerticalLayout::initialize() {
	for (auto& it : children)
			it->initialize();
}

void VerticalLayout::setMargin(int left, int right, int top, int bottom) {
	marginLeft = left;
	marginRight = right;
	marginTop = top;
	marginBottom = bottom;
}

void VerticalLayout::setRenderLayer(RenderLayer* renderLayer) {
	this->renderLayer = renderLayer;
}

WidgetSize VerticalLayout::calculateMinSize() {
	width = 0;
	height = 0;
	verticalFillChildCount = 0;

	WidgetSize cWidget;
	for (auto& it : children) {
		cWidget = it->calculateMinSize();
		if (cWidget.fillVertical)
			++verticalFillChildCount;
		width = std::max(width, cWidget.width);
		height += cWidget.height + padding;
	}
	height -= padding;
	height += marginTop + marginBottom;
	height = std::max(height, 0);

	width += marginLeft + marginRight;

	WidgetSize result;
	result.width = width;
	result.height = height;
	result.fillHorizontal = fillHorizontal;
	result.fillVertical = fillVertical;
	return result;
}

void VerticalLayout::setSize(int width, int height) {
	verticalSpaceToDistribute = height - this->height;
	this->width = width;
	this->height = height;
}

WidgetSize VerticalLayout::getSize() {
	WidgetSize result;
	result.width = width;
	result.height = height;
	result.fillHorizontal = fillHorizontal;
	result.fillVertical = fillVertical;
	return result;
}

void VerticalLayout::computeLayout() {
	WidgetSize cWidget;
	int cPosX = posX + marginLeft, cPosY = posY - marginTop;
	bool restAdded = false;
	for (auto& it : children) {
		it->setPosition(cPosX, cPosY);
		cWidget = it->getSize();
		if (cWidget.fillHorizontal || cWidget.fillVertical) {
			int cWidth = cWidget.width;
			int cHeight = cWidget.height;
			if (cWidget.fillHorizontal)
				cWidth = width - marginLeft - marginRight;
			if (cWidget.fillVertical) {
				cHeight = cWidget.height + verticalSpaceToDistribute / verticalFillChildCount;
				if (!restAdded) {
					cHeight += verticalSpaceToDistribute % verticalFillChildCount;
					restAdded = true;
				}
			}
			it->setSize(cWidth, cHeight);
			cPosY -= cHeight + padding;
		}
		else {
			cPosY -= cWidget.height + padding;
		}
		it->computeLayout();
	}
}

void VerticalLayout::addChild(Widget* child) {
	if (renderLayer) {
		child->setRenderLayer(renderLayer);
		children.push_back(child);
	}
	else
		printf("Can't add child to VerticalLayout because it has no RenderLayer!\n");
}

PriorityWidget VerticalLayout::getHighestPriorityWidget(int x, int y, SDL_Event* event) {
	PriorityWidget result;
	result.priority = 0;
	result.widget = this;
	for (auto& it : children) {
		if (it->contains(x, y))
			return it->getHighestPriorityWidget(x, y, event);
	}
	return result;
}

void VerticalLayout::draw() {
	for (auto& it : children)
		it->draw();
}

}
