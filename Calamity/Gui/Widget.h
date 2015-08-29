#pragma once

#include "../Renderer/RenderLayer.h"
#include "../../ActionHandler.h"

namespace clm {

class Widget;

struct WidgetSize {
	int width;
	int height;
	bool fillHorizontal;
	bool fillVertical;
};

struct PriorityWidget {
	int priority;
	Widget* widget;
};

class Widget {
protected:
	explicit Widget() : posX(0), posY(0), width(0), height(0) {}

	int posX, posY;
	int width, height;
public:
	virtual ~Widget() {}
	virtual void initialize() {}
	virtual void setPosition(int posX, int posY) { this->posX = posX; this->posY = posY; }
	virtual void setRenderLayer(RenderLayer* renderLayer) = 0;
	virtual WidgetSize calculateMinSize() = 0;
	virtual void setSize(int width, int height) = 0;
	virtual WidgetSize getSize() = 0;
	virtual void computeLayout() = 0;
	virtual bool contains(int x, int y) { return (x >= posX) && (x < posX + width) && (y <= posY) && (y > posY - height); }
	virtual PriorityWidget getHighestPriorityWidget(int x, int y, SDL_Event* event) = 0;
	virtual ActionHandler* createActionHandler(int x, int y, SDL_Event* event) { return nullptr; }
	virtual void draw() = 0;
};

}
