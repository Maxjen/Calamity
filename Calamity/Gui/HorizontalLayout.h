#pragma once

#include <list>

#include "Widget.h"
#include "../SpriteManager.h"

using std::list;

namespace clm {

//using std::unordered_map;

class HorizontalLayout : public Widget
{
private:
	RenderLayer* renderLayer;
	SpriteManager* spriteManager;
	bool fillHorizontal, fillVertical;
	int horizontalSpaceToDistribute;
	int horizontalFillChildCount;

	int padding;
	int marginLeft, marginRight, marginTop, marginBottom;

	list<Widget*> children;
public:
	explicit HorizontalLayout(SpriteManager* spriteManager, int width, int height, bool fillHorizontal = false, bool fillVertical = false);
	~HorizontalLayout();

	void initialize();
	void setMargin(int left, int right, int top, int bottom);

	void setRenderLayer(RenderLayer* renderLayer);
	WidgetSize calculateMinSize();
	void setSize(int width, int height);
	WidgetSize getSize();
	void computeLayout();

	void addChild(Widget* child);

	PriorityWidget getHighestPriorityWidget(int x, int y, SDL_Event* event);

	void draw();
};

}
