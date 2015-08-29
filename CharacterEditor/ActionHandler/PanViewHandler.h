#ifndef PANVIEWHANDLER_H
#define PANVIEWHANDLER_H

#include "../../ActionHandler.h"

using namespace clm;

class PanViewHandler : public ActionHandler {
private:
	float* viewPosX;
	float* viewPosY;
public:
	PanViewHandler(float* viewPosX, float* viewPosY);
	bool handleEvent(SDL_Event* event);
};

#endif // PANVIEWHANDLER_H
