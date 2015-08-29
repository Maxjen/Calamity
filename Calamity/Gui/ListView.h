#pragma once

#include <memory>
#include <vector>

#include "Widget.h"
#include "../SpriteManager.h"
#include "../Calamity/Functor.h"

using std::unique_ptr;
using std::vector;

namespace clm {

class ListViewItem {
public:
	virtual ~ListViewItem() {}
	virtual void initialize(RenderLayer* renderLayer, bool color) = 0;
	virtual int getHeight() = 0;
	virtual void handleEvent(int xRel, int yRel, RenderLayer* renderLayer, SDL_Event* event) = 0;
	virtual void draw(RenderLayer* renderLayer, int posX, int posY, int width) = 0;
};

class ListView : public Widget {
private:
	RenderLayer* renderLayer;
	//RenderLayer* itemLayer;
	//RenderLayer* borderLayer;
	unique_ptr<RenderLayer> itemLayer;
	unique_ptr<RenderLayer> borderLayer;

	SpriteManager* spriteManager;
	int minWidth, minHeight;
	bool fillHorizontal, fillVertical;

	unsigned int topLeftId, topId, topRightId, leftId, centerId, rightId, bottomLeftId, bottomId, bottomRightId;

	vector<ListViewItem*> items;
	bool cItemColor;
	int scrollAmount;
	int maxScrollAmount;

	Functor* loadFunctor;
public:
	explicit ListView(SpriteManager* spriteManager, int width, int height, bool fillHorizontal = false, bool fillVertical = false);
	~ListView();

	void setLoadFunctor(Functor* loadFunctor);
	void reload();

	void setRenderLayer(RenderLayer* renderLayer);
	WidgetSize calculateMinSize();
	void setSize(int width, int height);
	WidgetSize getSize();
	void computeLayout();

	//void setChild(Widget* child);
	PriorityWidget getHighestPriorityWidget(int x, int y, SDL_Event* event);
	ActionHandler* createActionHandler(int x, int y, SDL_Event* event);

	void addItem(ListViewItem* item);
	void clear();

	void draw();
};

}
