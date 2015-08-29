#pragma once

#include <memory>

#include "Widget.h"
#include "../SpriteManager.h"
#include "ActionHandler/InputHandler.h"
#include "../Properties.h"

using std::unique_ptr;

namespace clm {

//using std::unordered_map;

class Input {
private:
	SpriteManager* spriteManager;
	RenderLayer* renderLayer;
	unsigned int fontId;
	float x, y;
	float width;
	bool verticalSeperator;
	int horizontalSeperators;
	unsigned int vertical, top, bottom;
	unsigned int textId;
	unsigned int cursorId;
	unsigned int cursorPos;
	wstring label;
	wstring value;
	Properties* properties;
	Property* property;

	bool editMode;
public:
	//Button(SpriteManager* spriteManager, RenderLayer* renderLayer, int left, int right, float x, float y, float width);
	explicit Input(SpriteManager* spriteManager, RenderLayer* renderLayer, wstring label, unsigned int fontId,
		  Properties* properties, Property* property);
	~Input();

	bool contains(int x, int y);

	Properties* getProperties();
	Property* getProperty();
	void setEditMode(bool editMode);
	void setCursorPos(unsigned int cursorPos);
	wstring getValue();
	void setValue(wstring value);

	void setPosition(float x, float y);
	void setSeperators(bool verticalSeperator, int horizontalSeperators);
	void setSprites();

	void setWidth(float width);
	void updateSprites();
	void prepare();
};

class InputGroup : public Widget {
private:
	RenderLayer* renderLayer;
	//RenderLayer* inputLayer;
	//RenderLayer* borderLayer;
	unique_ptr<RenderLayer> inputLayer;
	unique_ptr<RenderLayer> borderLayer;

	SpriteManager* spriteManager;
	int minWidth, minHeight;
	bool fillHorizontal, fillVertical;

	unsigned int topLeftId, topId, topRightId, leftId, centerId, rightId, bottomLeftId, bottomId, bottomRightId;

	unsigned int fontId;
	vector<vector<Input*>> rows;
	vector<unsigned int> horizontalSeperatorIds;
public:
	explicit InputGroup(SpriteManager* spriteManager, unsigned int fontId, int width, bool fillHorizontal = false);
	~InputGroup();

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
	Input* addInput(unsigned int row, wstring label, Properties* properties, Property* property);

	void draw();
};

}
