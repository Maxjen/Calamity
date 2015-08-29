#pragma once

#include "../../ActionHandler.h"
#include <list> // TODO: remove (used for max)

using namespace clm;

class ScaleWindowHandler : public ActionHandler {
private:
	int* width;
	int* height;
	int minWidth;
	int minHeight;
	int initialWidth;
	int initialHeight;

	int* windowPosX;
	int* windowPosY;
	int initialWindowPosX;
	int initialWindowPosY;

	int mouseStartX;
	int mouseStartY;
	bool scaleWidth;
	bool scaleHeight;
	bool left;
	bool top;
public:
	explicit ScaleWindowHandler(int mouseX, int mouseY, int minWidth, int minHeight, int* width, int* height,
					   int* windowPosX, int* windowPosY, bool scaleWidth, bool scaleHeight, bool left, bool top);
	bool handleEvent(SDL_Event* event);
};
