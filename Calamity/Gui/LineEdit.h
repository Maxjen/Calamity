#pragma once

#include <memory>
#include <vector>

#include "Widget.h"
#include "../SpriteManager.h"
#include "ActionHandler/LineEditHandler.h"
#include "../Properties.h"

using std::unique_ptr;
using std::vector;

namespace clm {

class LineEdit : public Widget {
private:
	RenderLayer* renderLayer;
	unique_ptr<RenderLayer> backgroundLayer;
	unique_ptr<RenderLayer> borderLayer;

	SpriteManager* spriteManager;
	int minWidth, minHeight;
	bool fillHorizontal, fillVertical;

	unsigned int topLeftId, topId, topRightId, leftId, centerId, rightId, bottomLeftId, bottomId, bottomRightId;
	unsigned int labelId;
	unsigned int labelBg, labelBorder;

	unsigned int textId;
	unsigned int cursorId;
	unsigned int cursorPos;
	wstring label;
	wstring value;
	Properties* properties;
	Property* property;

	bool editMode;

	unsigned int fontId;
	/*int scrollAmount;
	int maxScrollAmount;*/
public:
	explicit LineEdit(SpriteManager* spriteManager, int width, wstring label, unsigned int fontId, Properties* properties, Property* property, bool fillHorizontal = false);
	~LineEdit();

	void setRenderLayer(RenderLayer* renderLayer);
	WidgetSize calculateMinSize();
	void setSize(int width, int height);
	WidgetSize getSize();
	void computeLayout();

	Properties* getProperties();
	Property* getProperty();
	void setEditMode(bool editMode);
	void setCursorPos(unsigned int cursorPos);
	wstring getValue();
	void setValue(wstring value);

	//void setChild(Widget* child);
	PriorityWidget getHighestPriorityWidget(int x, int y, SDL_Event* event);
	ActionHandler* createActionHandler(int x, int y, SDL_Event* event);

	void draw();
};

}
