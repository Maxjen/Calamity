#pragma once

#include <list>
#include <map>

#include "ActionHandler/ScaleVerticalBoxHandler.h"

#include "Widget.h"
#include "../SpriteManager.h"

using std::list;
using std::map;

namespace clm {

/*struct BoxData {
	Widget* widget;
	int height;
	int minHeight;

	int seperatorId;
};*/

class VerticalBoxLayout : public Widget {
private:
	RenderLayer* renderLayer;
	SpriteManager* spriteManager;
	int minWidth, minHeight;
	bool fillHorizontal, fillVertical;
	int verticalSpaceToDistribute;

	int padding;
	int seperatorHeight;

	map<unsigned int, BoxData> boxes;
	unsigned int cBox;
	//list<BoxData> boxes;
	//list<Widget*> children;
public:
	explicit VerticalBoxLayout(SpriteManager* spriteManager, int width, int height, bool fillHorizontal = false, bool fillVertical = false);
	~VerticalBoxLayout();

	void initialize();

	void setRenderLayer(RenderLayer* renderLayer);
	WidgetSize calculateMinSize();
	void setSize(int width, int height);
	WidgetSize getSize();
	void computeLayout();

	//void addChild(Widget* child);
	unsigned int addBox();
	void setBoxWidget(unsigned int boxId, Widget* widget);
	void clearBox(unsigned int boxId);

	PriorityWidget getHighestPriorityWidget(int x, int y, SDL_Event* event);
	ActionHandler* createActionHandler(int x, int y, SDL_Event* event);

	void draw();
};

}
