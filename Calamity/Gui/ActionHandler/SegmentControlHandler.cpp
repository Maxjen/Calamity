#include "SegmentControlHandler.h"

SegmentControlHandler::SegmentControlHandler(Segment* segment, Properties* properties, IntProperty* property, int screenHeight) :
	segment(segment),
	properties(properties),
	property(property),
	screenHeight(screenHeight)
{
	segment->setIgnoreValue(true);
	segment->setPressed(true);
}

bool SegmentControlHandler::handleEvent(SDL_Event* event) {
	switch (event->type) {
		case SDL_MOUSEBUTTONUP:
			if (event->button.button == SDL_BUTTON_LEFT) {
				if (segment->contains(event->button.x, screenHeight - event->button.y)) {
					char buffer[3];
					snprintf(buffer, 3, "%d", segment->getValue());
					string sValue = buffer;

					wstring wValue;
					wValue.assign(sValue.begin(), sValue.end());

					if (properties)
						properties->pushPropertyChange(property, property->getValueAsString(), wValue);
				}
				segment->setIgnoreValue(false);
				return true;
			}
			break;
		default:
			break;
	}

	return false;
}
