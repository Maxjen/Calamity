#include "ButtonHandler.h"

ButtonHandler::ButtonHandler(Button* button, int screenHeight) :
	button(button),
	functor(button->getFunctor()),
	screenHeight(screenHeight)
{
	button->setPressed(true);
}

bool ButtonHandler::handleEvent(SDL_Event* event) {
	switch (event->type) {
		case SDL_MOUSEBUTTONUP:
			if (event->button.button == SDL_BUTTON_LEFT) {
				button->setPressed(false);
				if (button->contains(event->button.x, screenHeight - event->button.y) && functor) {
					(*functor)();
				}
				return true;
			}
			break;
		default:
			break;
	}

	return false;
}
