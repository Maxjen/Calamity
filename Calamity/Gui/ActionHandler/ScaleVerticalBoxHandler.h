#pragma once

#include "../../ActionHandler.h"
//#include "../VerticalBoxLayout.h"
#include "../Widget.h"
//#include <list>
#include <map>

using namespace clm;

//using std::list;
using std::map;

struct BoxData {
	Widget* widget;
	int height;
	int minHeight;

	unsigned int seperatorId;
};

class ScaleVerticalBoxHandler : public ActionHandler {
private:
	int lastMouseY;
	map<unsigned int, BoxData>* boxes;
	int seperatorPosition;
public:
	explicit ScaleVerticalBoxHandler(int mouseY, map<unsigned int, BoxData>* boxes, int seperatorPosition);
	bool handleEvent(SDL_Event* event);
};
