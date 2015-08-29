#include "ScaleWindowHandler.h"

ScaleWindowHandler::ScaleWindowHandler(int mouseX, int mouseY, int minWidth, int minHeight, int* width, int* height,
									   int* windowPosX, int* windowPosY, bool scaleWidth, bool scaleHeight, bool left, bool top) :
	width(width),
	height(height),
	minWidth(minWidth),
	minHeight(minHeight),
	initialWidth(*width),
	initialHeight(*height),

	windowPosX(windowPosX),
	windowPosY(windowPosY),
	initialWindowPosX(*windowPosX),
	initialWindowPosY(*windowPosY),

	mouseStartX(mouseX),
	mouseStartY(mouseY),
	scaleWidth(scaleWidth),
	scaleHeight(scaleHeight),
	left(left),
	top(top)
{}

bool ScaleWindowHandler::handleEvent(SDL_Event* event) {
	switch (event->type) {
		case SDL_MOUSEMOTION: {
			if (scaleWidth) {
				int deltaX = event->button.x - mouseStartX;
				if (left) {
					int newWidth = std::max(minWidth, initialWidth - deltaX);
					deltaX = newWidth - initialWidth;
					*windowPosX = initialWindowPosX - deltaX;
					*width = newWidth;
				}
				else {
					*width = std::max(minWidth, initialWidth + deltaX);
				}
			}
			if (scaleHeight) {
				int deltaY = event->button.y - mouseStartY;
				if (top) {
					int newHeight = std::max(minHeight, initialHeight - deltaY);
					deltaY = newHeight - initialHeight;
					*windowPosY = initialWindowPosY + deltaY;
					*height = newHeight;
				}
				else {
					*height = std::max(minHeight, initialHeight + deltaY);
				}
			}
			break;
		}
		case SDL_MOUSEBUTTONUP:
			if (event->button.button == SDL_BUTTON_LEFT)
				return true;
			break;
		default:
			break;
	}

	return false;
}
