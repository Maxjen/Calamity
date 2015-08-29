#pragma once

#include <memory>
#include <vector>

#include "Widget.h"
#include "../SpriteManager.h"
#include "../Properties.h"

using std::unique_ptr;
using std::vector;

namespace clm {

class Checkbox : public Widget {
private:
	RenderLayer* renderLayer;

	SpriteManager* spriteManager;
	int minWidth, minHeight;
	bool fillHorizontal, fillVertical;

	unsigned int checkboxId, checkId;

	unsigned int labelId;
	wstring label;

	Properties* properties;
	Property* property;

	unsigned int fontId;

	bool bChecked;
public:
	explicit Checkbox(SpriteManager* spriteManager, int width, wstring label, unsigned int fontId, Properties* properties, Property* property, bool fillHorizontal = false);
	~Checkbox();

	void setRenderLayer(RenderLayer* renderLayer);
	WidgetSize calculateMinSize();
	void setSize(int width, int height);
	WidgetSize getSize();
	void computeLayout();

	Properties* getProperties();
	Property* getProperty();

	//void setChild(Widget* child);
	PriorityWidget getHighestPriorityWidget(int x, int y, SDL_Event* event);
	ActionHandler* createActionHandler(int x, int y, SDL_Event* event);

	void draw();
};

}
