#pragma once

#include "Widget.h"
#include "../SpriteManager.h"
#include "ActionHandler/SegmentControlHandler.h"
#include "../Properties.h"

namespace clm {

//using std::unordered_map;

class Segment {
private:
	SpriteManager* spriteManager;
	RenderLayer* renderLayer;
	unsigned int fontId;
	float x, y;
	float width;
	int left, right;
	bool bPressed;
	unsigned int leftId, middleId, rightId;
	unsigned int textId;
	int value;

	bool bIgnoreValue;
public:
	explicit Segment(SpriteManager* spriteManager, RenderLayer* renderLayer, wstring label, unsigned int fontId, int value);
	~Segment();
	bool contains(int x, int y);

	void setIgnoreValue(bool bIgnoreValue);
	int getValue();

	void setLeft(int left);
	void setRight(int right);
	void setSprites();
	void setPressed(bool bPressed);
	void setPosition(float x, float y);

	void setWidth(float width);
	void updateSprites(int scValue);
	void prepare();
};

class SegmentControl : public Widget {
private:
	RenderLayer* renderLayer;
	SpriteManager* spriteManager;
	int minWidth, minHeight;
	bool fillHorizontal, fillVertical;

	unsigned int topLeftId, topId, topRightId, leftId, rightId, bottomLeftId, bottomId, bottomRightId;

	unsigned int fontId;
	vector<vector<Segment*>> rows;

	Properties* properties;
	IntProperty* property;
public:
	explicit SegmentControl(SpriteManager* spriteManager, unsigned int fontId, int width, Properties* properties, IntProperty* property, bool fillHorizontal = false);
	~SegmentControl();

	void initialize();

	void setRenderLayer(RenderLayer* renderLayer);
	WidgetSize calculateMinSize();
	void setSize(int width, int height);
	WidgetSize getSize();
	void computeLayout();

	//void setChild(Widget* child);
	PriorityWidget getHighestPriorityWidget(int x, int y, SDL_Event* event);
	ActionHandler* createActionHandler(int x, int y, SDL_Event* event);

	void addRow();
	void addSegment(unsigned int row, wstring label, int value = 0);

	void draw();
};

}
