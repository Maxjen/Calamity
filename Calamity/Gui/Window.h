#pragma once

#include <memory>

#include "math.h"

#include "../../ActionHandler.h"
#include "ActionHandler/ScaleWindowHandler.h"
#include "ActionHandler/MoveWindowHandler.h"

#include "../SpriteManager.h"
#include "Widget.h"

using std::unique_ptr;

namespace clm {

enum WindowFlags {
	CLM_RESIZE_LEFT = 0x00000001,
	CLM_RESIZE_RIGHT = 0x00000002,
	CLM_RESIZE_TOP = 0x00000004,
	CLM_RESIZE_BOTTOM = 0x00000008,
	CLM_RESIZE_NORMAL = 0x0000000F,
	CLM_BOTTOM_LEVEL = 0x00000010,
	CLM_NO_MOVE = 0x00000020
};

class Window /*: public ActionHandler*/ {
private:
	unsigned int windowId;
	unsigned int zOrder;

	//RenderLayer* renderLayer;
	RenderLayer renderLayer;
	RenderLayer* shadowLayer;

	SpriteManager* spriteManager;
	int screenHeight;
	int posX, posY;
	int width, height;
	int minWidth, minHeight;
	int flags;

	unsigned int shadowTopLeft, shadowTop, shadowTopRight, shadowLeft, shadowRight, shadowBottomLeft, shadowBottom, shadowBottomRight;
	unsigned int topId, leftId, centerId, rightId, bottomId;

	bool hasBeenInitialized;
	int padding;

	Widget* child;
public:
	explicit Window(unsigned int windowId, SpriteManager* spriteManager, int posX, int posY, int width, int height, int flags);
	~Window();

	void initialize();

	unsigned int getWindowId();
	int getFlags();
	unsigned int getZOrder();

	void setShadowLayer(RenderLayer* shadowLayer);
	void setZOrder(unsigned int zOrder);
	void setPadding(int padding);
	void setChild(Widget* child);
	void clear();

	bool contains(int x, int y);

	//PriorityWidget getHighestPriorityWidget(int x, int y);
	ActionHandler* createActionHandler(int x, int y, SDL_Event* event);

	void update();
	void draw();
};

}
