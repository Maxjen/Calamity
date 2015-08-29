#pragma once

#include "../../ActionHandler.h"

using namespace clm;

class MoveWindowHandler : public ActionHandler {
private:
	int* windowPosX;
	int* windowPosY;
public:
	explicit MoveWindowHandler(int* windowPosX, int* windowPosY);
	bool handleEvent(SDL_Event* event);
};
