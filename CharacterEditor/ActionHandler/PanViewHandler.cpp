#include "PanViewHandler.h"

PanViewHandler::PanViewHandler(float* viewPosX, float* viewPosY) {
	this->viewPosX = viewPosX;
	this->viewPosY = viewPosY;
}

bool PanViewHandler::handleEvent(SDL_Event* event) {
	switch (event->type) {
		case SDL_MOUSEMOTION:
			*viewPosX -= event->motion.xrel;
			*viewPosY += event->motion.yrel;
			break;
		case SDL_MOUSEBUTTONUP:
			if (event->button.button == SDL_BUTTON_MIDDLE)
				return true;
			break;
		default:
			break;
	}

	return false;
}
