#include "ScaleVerticalBoxHandler.h"

ScaleVerticalBoxHandler::ScaleVerticalBoxHandler(int mouseY, map<unsigned int, BoxData>* boxes, int seperatorPosition) :
	lastMouseY(mouseY),
	boxes(boxes),
	seperatorPosition(seperatorPosition)
{}

bool ScaleVerticalBoxHandler::handleEvent(SDL_Event* event) {
	switch (event->type) {
		case SDL_MOUSEMOTION: {
			{
				int deltaY = event->button.y - lastMouseY;
				map<unsigned int, BoxData>::iterator box1;
				map<unsigned int, BoxData>::iterator box2;
				box1 = boxes->begin();
				for (int i = 0; i < seperatorPosition; ++i)
					++box1;
				box2 = box1;
				++box2;

				int availableSpace;
				if (deltaY > 0) {
					availableSpace = box2->second.height - box2->second.minHeight;
					int deltaSpace = std::min(deltaY, availableSpace);
					box1->second.height += deltaSpace;
					box2->second.height -= deltaSpace;
					lastMouseY += deltaSpace;
				}
				else if (deltaY < 0) {
					deltaY = -deltaY;
					availableSpace = box1->second.height - box1->second.minHeight;
					int deltaSpace = std::min(deltaY, availableSpace);
					box1->second.height -= deltaSpace;
					box2->second.height += deltaSpace;
					lastMouseY -= deltaSpace;
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
