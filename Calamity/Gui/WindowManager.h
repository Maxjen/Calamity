#pragma once

#include <memory>
#include <unordered_map>
#include <map>
#include <list>

#include "math.h"

#include "../../ActionHandler.h"

#include "../SpriteManager.h"
#include "Widget.h"
#include "Window.h"

using std::unique_ptr;
using std::unordered_map;
using std::map;
using std::list;

namespace clm {

//using std::unordered_map;

class WindowManager /*: public ActionHandler*/ {
private:
	SpriteManager* spriteManager;
	RenderLayer bottomLevelShadowLayer;

	unordered_map<unsigned int, unique_ptr<Window>> windows;
	unsigned int cWindow;

	map<unsigned int, Window*> windowsByZOrder;
public:
	explicit WindowManager(SpriteManager* spriteManager);

	unsigned int createWindow(int posX, int posY, int width, int height, int flags);
	void deleteWindow(unsigned int windowId);

	Window* getWindow(unsigned int windowId);

	void setChild(Widget* child);

	bool contains(int x, int y);

	ActionHandler* createActionHandler(int x, int y, SDL_Event* event);

	void update();
	void draw();
};

}
