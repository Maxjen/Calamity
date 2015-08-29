#pragma once

#include "Widget.h"
#include "../SpriteManager.h"
#include "ActionHandler/ButtonHandler.h"
#include "../Functor.h"

namespace clm {

//using std::unordered_map;

class Button {
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
	Functor* functor;
public:
	//Button(SpriteManager* spriteManager, RenderLayer* renderLayer, int left, int right, float x, float y, float width);
	explicit Button(SpriteManager* spriteManager, RenderLayer* renderLayer, wstring label, unsigned int fontId, Functor* functor);
	~Button();
	bool contains(int x, int y);
	Functor* getFunctor();
	void setLeft(int left);
	void setRight(int right);
	void setSprites();
	void setPressed(bool bPressed);
	void setPosition(float x, float y);

	void setWidth(float width);
	void updateSprites();
	void prepare();
};

class ButtonGroup : public Widget {
private:
	RenderLayer* renderLayer;
	SpriteManager* spriteManager;
	int minWidth, minHeight;
	bool fillHorizontal, fillVertical;

	unsigned int topLeftId, topId, topRightId, leftId, rightId, bottomLeftId, bottomId, bottomRightId;

	unsigned int fontId;
	vector<vector<Button*>> rows;
public:
	explicit ButtonGroup(SpriteManager* spriteManager, unsigned int fontId, int width, bool fillHorizontal = false);
	~ButtonGroup();

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
	void addButton(unsigned int row, wstring label, Functor* functor = nullptr);

	void draw();
};

}
