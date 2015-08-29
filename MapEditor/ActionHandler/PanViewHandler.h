#pragma once

#include "../../ActionHandler.h"

using namespace clm;

class PanViewHandler : public ActionHandler {
private:
	float* viewPosX;
	float* viewPosY;
public:
	explicit PanViewHandler(float* viewPosX, float* viewPosY);
	bool handleEvent(SDL_Event* event);
};
