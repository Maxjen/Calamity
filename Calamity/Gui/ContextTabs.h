#pragma once

#include <vector>

#include "Widget.h"
#include "../SpriteManager.h"
#include "../ContextManager.h"

using std::vector;

namespace clm {

class ContextTab {
private:
	SpriteManager* spriteManager;
	unsigned int contextId;
	bool isActive;
	int posX;
	int width;
	wstring label;
	unsigned int fontId;

	unsigned int left, middle, right;
	unsigned int textId;
public:
	explicit ContextTab(SpriteManager* spriteManager, unsigned int contextId, bool isActive, wstring label, unsigned int fontId);
	~ContextTab();
	void initialize(RenderLayer* renderLayer);
	unsigned int getContextId();
	bool getIsActive();
	void setPosX(int posX);
	int getPosX();
	int getWidth();
	void draw(int posX, int posY);
};

class ContextTabs : public Widget {
private:
	RenderLayer* renderLayer;

	SpriteManager* spriteManager;
	ContextManager* contextManager;
	unsigned int activeContextId;
	int minWidth, minHeight;
	bool fillHorizontal, fillVertical;

	unsigned int bg, left, right;

	vector<ContextTab*> tabs;
public:
	explicit ContextTabs(SpriteManager* spriteManager, ContextManager* contextManager, unsigned int activeContextId, int width, bool fillHorizontal = false, bool fillVertical = false);
	~ContextTabs();

	void setRenderLayer(RenderLayer* renderLayer);
	WidgetSize calculateMinSize();
	void setSize(int width, int height);
	WidgetSize getSize();
	void computeLayout();

	PriorityWidget getHighestPriorityWidget(int x, int y, SDL_Event* event);
	ActionHandler* createActionHandler(int x, int y, SDL_Event* event);

	void addTab(ContextTab* tab);

	void draw();
};

}
