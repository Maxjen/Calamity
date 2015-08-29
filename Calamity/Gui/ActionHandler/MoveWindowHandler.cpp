#include "MoveWindowHandler.h"

MoveWindowHandler::MoveWindowHandler(int* windowPosX, int* windowPosY) :
	windowPosX(windowPosX),
	windowPosY(windowPosY)
{}

bool MoveWindowHandler::handleEvent(SDL_Event* event) {
	switch (event->type) {
		case SDL_MOUSEMOTION:
			*windowPosX += event->motion.xrel;
			*windowPosY -= event->motion.yrel;
			break;
		case SDL_MOUSEBUTTONUP:
			if (event->button.button == SDL_BUTTON_LEFT)
				return true;
			break;
		default:
			break;
	}

	return false;
}
