#include "VerticalBoxLayout.h"

namespace clm {

VerticalBoxLayout::VerticalBoxLayout(SpriteManager* spriteManager, int width, int height, bool fillHorizontal, bool fillVertical) :
	renderLayer(nullptr),
	spriteManager(spriteManager),
	minWidth(0),
	minHeight(0),
	fillHorizontal(fillHorizontal),
	fillVertical(fillVertical),
	verticalSpaceToDistribute(0),

	padding(6),
	seperatorHeight(2),

	boxes(),
	cBox(0)
{}

VerticalBoxLayout::~VerticalBoxLayout() {
	for (auto& it : boxes) {
		if (it.second.seperatorId != (unsigned int)-1)
			spriteManager->removeSprite(it.second.seperatorId);
		if (it.second.widget)
			delete it.second.widget;
	}
}

void VerticalBoxLayout::initialize() {
	map<unsigned int, BoxData>::iterator it;
	for (it = boxes.begin(); it != boxes.end(); ++it) {
		it->second.minHeight = 0;
		if (it->second.widget)
			it->second.minHeight = it->second.widget->calculateMinSize().height;
		if (it != --boxes.end()) {
			it->second.height = it->second.minHeight + verticalSpaceToDistribute / boxes.size();
			it->second.seperatorId = spriteManager->addSprite(posX, posY, width, 2, "Calamity/Gui/Images/seperator.png", renderLayer);
		}
		else {
			it->second.height = it->second.minHeight + verticalSpaceToDistribute / boxes.size() + verticalSpaceToDistribute % boxes.size();
			it->second.seperatorId = -1;
		}
	}
	verticalSpaceToDistribute = 0;

	for (auto& it : boxes) {
		if (it.second.widget)
			it.second.widget->initialize();
	}
}

void VerticalBoxLayout::setRenderLayer(RenderLayer* renderLayer) {
	this->renderLayer = renderLayer;
}

WidgetSize VerticalBoxLayout::calculateMinSize() {
	minWidth = 0;
	minHeight = 0;

	WidgetSize cWidget;
	for (auto& it : boxes) {
		cWidget.width = 0;
		cWidget.height = 0;
		if (it.second.widget)
			cWidget = it.second.widget->calculateMinSize();
		minWidth = std::max(minWidth, cWidget.width + 2 * padding);
		minHeight += cWidget.height + 2 * padding + seperatorHeight;
	}
	minHeight -= seperatorHeight;
	minHeight = std::max(minHeight, 0);

	height = std::max(minHeight, height);

	WidgetSize result;
	result.width = minWidth;
	result.height = minHeight;
	result.fillHorizontal = fillHorizontal;
	result.fillVertical = fillVertical;
	return result;
}

void VerticalBoxLayout::setSize(int width, int height) {
	verticalSpaceToDistribute = height - this->height;
	this->width = width;
	this->height = height;
}

WidgetSize VerticalBoxLayout::getSize() {
	WidgetSize result;
	result.width = width;
	result.height = height;
	result.fillHorizontal = fillHorizontal;
	result.fillVertical = fillVertical;
	return result;
}

void VerticalBoxLayout::computeLayout() {
	if (verticalSpaceToDistribute > 0) {
		map<unsigned int, BoxData>::iterator it;
		for (it = boxes.begin(); it != boxes.end(); ++it) {
			it->second.height += verticalSpaceToDistribute / boxes.size();
			if (it == --boxes.end())
				it->second.height += verticalSpaceToDistribute % boxes.size();
		}
	}
	else if (verticalSpaceToDistribute < 0) {
		// this section is unnecessary but maybe an optimization
		int boxesToShrink = boxes.size();
		int spacePerBox = verticalSpaceToDistribute / boxesToShrink;
		while (spacePerBox != 0) {
			int boxesToShrinkNext = 0;
			map<unsigned int, BoxData>::iterator it;
			for (it = boxes.begin(); it != boxes.end(); ++it) {
				int availableHeight = it->second.minHeight - it->second.height;
				int deltaHeight = std::max(availableHeight, spacePerBox);
				if (deltaHeight != availableHeight)
					boxesToShrinkNext++;
				it->second.height += deltaHeight;
				verticalSpaceToDistribute -= deltaHeight;
			}
			boxesToShrink = boxesToShrinkNext;
			if (boxesToShrink != 0)
				spacePerBox = verticalSpaceToDistribute / boxesToShrink;
			else
				spacePerBox = 0;
		}
		// end of unnecessary section

		// case for the rest (example: 5 boxes, -3 verticalSpaceToDistribute
		if (verticalSpaceToDistribute != 0) {
			while (verticalSpaceToDistribute != 0) {
				map<unsigned int, BoxData>::iterator it;
				for (it = boxes.begin(); it != boxes.end(); ++it) {
					int availableHeight = it->second.minHeight - it->second.height;
					int deltaHeight = std::max(availableHeight, -1);
					it->second.height += deltaHeight;
					verticalSpaceToDistribute -= deltaHeight;
					if (verticalSpaceToDistribute == 0)
						break;
				}
			}
		}
	}

	WidgetSize cWidget;
	int cPosX = posX + padding, cPosY = posY - padding;
	map<unsigned int, BoxData>::iterator it;
	for (it = boxes.begin(); it != boxes.end(); ++it) {
		cWidget.fillHorizontal = false;
		cWidget.fillVertical = false;
		if (it->second.widget) {
			it->second.widget->setPosition(cPosX, cPosY);
			cWidget = it->second.widget->getSize();
		}
		if (cWidget.fillHorizontal || cWidget.fillVertical) {
			int cWidth = cWidget.width;
			int cHeight = cWidget.height;
			if (cWidget.fillHorizontal)
				cWidth = width - 2 * padding;
			if (cWidget.fillVertical)
				cHeight = it->second.height;
			it->second.widget->setSize(cWidth, cHeight);
		}
		if (it != --boxes.end()) {
			spriteManager->setSpritePositionAndSize(it->second.seperatorId, posX, cPosY - it->second.height - padding, width, 2);
			spriteManager->flagSpriteForRender(it->second.seperatorId);
		}
		cPosY -= it->second.height + 2 * padding + seperatorHeight;
		if (it->second.widget)
			it->second.widget->computeLayout();
	}
}

unsigned int VerticalBoxLayout::addBox() {
	if (renderLayer) {
		BoxData& box = boxes[cBox];
		box.widget = nullptr;
		return cBox++;
	}
	else {
		printf("Can't add box to VerticalBoxLayout because it has no RenderLayer!\n");
		return -1;
	}
}

void VerticalBoxLayout::setBoxWidget(unsigned int boxId, Widget* widget) {
	if (boxes.find(boxId) != boxes.end()) {
		BoxData& box = boxes[boxId];
		box.widget = widget;
		box.widget->setRenderLayer(renderLayer);
	}
	else
		printf("There is no box with id %u\n", boxId);
}

void VerticalBoxLayout::clearBox(unsigned int boxId) {
	if (boxes.find(boxId) != boxes.end()) {
		BoxData& box = boxes[boxId];
		if (box.widget)
			delete box.widget;
		box.widget = nullptr;
	}
	else
		printf("There is no box with id %u\n", boxId);
}

PriorityWidget VerticalBoxLayout::getHighestPriorityWidget(int x, int y, SDL_Event* event) {
	PriorityWidget result;
	result.priority = 0;
	result.widget = this;
	int cY = posY;

	map<unsigned int, BoxData>::iterator it;
	for (it = boxes.begin(); it != boxes.end(); ++it) {
		if (it->second.widget) {
			if (it->second.widget->contains(x, y))
				return it->second.widget->getHighestPriorityWidget(x, y, event);
		}
		cY -= it->second.height + 2 * padding;
		if (it != --boxes.end()) {
			//if (y <= cY + 1 && y >= cY - 3 && event->type == SDL_MOUSEBUTTONDOWN && event->button.button == SDL_BUTTON_LEFT) {
			if (y <= cY + 4 && y >= cY - 6 && event->type == SDL_MOUSEBUTTONDOWN && event->button.button == SDL_BUTTON_LEFT) {
				result.priority = 1;
				result.widget = this;
				return result;
			}
			cY -= 2;
		}
	}
	return result;
}

ActionHandler* VerticalBoxLayout::createActionHandler(int x, int y, SDL_Event* event) {
	int i = 0;
	int cY = posY;
	map<unsigned int, BoxData>::iterator it;
	for (it = boxes.begin(); it != boxes.end(); ++it) {
		cY -= it->second.height + 2 * padding;
		if (it != --boxes.end()) {
			//if (y <= cY + 1 && y >= cY - 3)
			if (y <= cY + 4 && y >= cY - 6)
				break;
			cY -= 2;
			++i;
		}
	}
	return new ScaleVerticalBoxHandler(-(y - spriteManager->getScreenHeight()), &boxes, i);
}

void VerticalBoxLayout::draw() {
	for (auto& it : boxes) {
		if (it.second.widget)
			it.second.widget->draw();
	}
}

}
