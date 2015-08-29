#pragma once

#include "../../ActionHandler.h"
#include "../SegmentControl.h"

using namespace clm;

namespace clm {
	class Segment;
	class Properties;
	class IntProperty;
}

class SegmentControlHandler : public ActionHandler {
private:
	Segment* segment;
	Properties* properties;
	IntProperty* property;
	int screenHeight;
public:
	explicit SegmentControlHandler(Segment* segment, Properties* properties, IntProperty* property, int screenHeight);
	bool handleEvent(SDL_Event* event);
};
