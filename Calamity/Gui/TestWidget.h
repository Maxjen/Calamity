#pragma once

#include "Widget.h"
#include "../SpriteManager.h"

namespace clm {

//using std::unordered_map;

class TestWidget : public Widget {
private:
	RenderLayer* renderLayer;
	SpriteManager* spriteManager;
	int minWidth, minHeight;
	bool fillHorizontal, fillVertical;

	unsigned int topLeftId, topId, topRightId, leftId, centerId, rightId, bottomLeftId, bottomId, bottomRightId;
public:
	explicit TestWidget(SpriteManager* spriteManager, int width, int height, bool fillHorizontal = false, bool fillVertical = false);
	~TestWidget();

	void setRenderLayer(RenderLayer* renderLayer);
	WidgetSize calculateMinSize();
	void setSize(int width, int height);
	WidgetSize getSize();
	void computeLayout();

	//void setChild(Widget* child);
	PriorityWidget getHighestPriorityWidget(int x, int y, SDL_Event* event);

	void draw();
};

}
