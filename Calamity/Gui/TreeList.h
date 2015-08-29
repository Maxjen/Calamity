#pragma once

#include <memory>
#include <unordered_map>
#include <vector>

#include "Widget.h"
#include "../SpriteManager.h"
#include "../Calamity/Functor.h"

using std::unique_ptr;
using std::unordered_map;
using std::vector;

namespace clm {

//using std::unordered_map;

class TreeListItem {
protected:
	//unsigned int parent;
	/*unsigned int next;
	unsigned int lastChild;*/
	int depth;
	std::vector<unsigned int> children;
	bool bCollapsed;
public:
	TreeListItem() : /*parent(0), next(0), lastChild(0),*/ depth(0), children(), bCollapsed(false) {}
	virtual ~TreeListItem() {}
	virtual void addChild(unsigned int child) { children.push_back(child); }
	void addNonCollapsedChildren(unordered_map<unsigned int, TreeListItem*> items,
								 vector<unsigned int>& nonCollapsedItems);

	virtual void initialize(RenderLayer* renderLayer, int depth) = 0;
	virtual int getHeight() = 0;
	int getDepth() { return depth; }
	virtual void setColor(bool bColor) = 0;
	virtual void handleEvent(int xRel, int yRel, SDL_Event* event) = 0;
	virtual void draw(int posX, int posY, int width) = 0;
};

class TreeList : public Widget {
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

	unordered_map<unsigned int, TreeListItem*> items;
	unsigned int cItem;

	vector<unsigned int> topLevelItems;

	vector<unsigned int> nonCollapsedItems;

	//vector<TreeListItem*> items;
	int scrollAmount;
	int maxScrollAmount;

	Functor* loadFunctor;
public:
	explicit TreeList(SpriteManager* spriteManager, int width, int height, bool fillHorizontal = false, bool fillVertical = false);
	~TreeList();

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

	unsigned int addItem(TreeListItem* item);
	unsigned int addItemAsChild(TreeListItem* item, unsigned int parent);
	void clear();

	void draw();
};

}
