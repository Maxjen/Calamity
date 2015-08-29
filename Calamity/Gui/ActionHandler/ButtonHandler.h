#pragma once

#include "../../ActionHandler.h"
#include "../ButtonGroup.h"
#include "../../Functor.h"

using namespace clm;

namespace clm {
	class Button;
}

class ButtonHandler : public ActionHandler {
private:
	Button* button;
	Functor* functor;
	int screenHeight;
public:
	explicit ButtonHandler(Button* button, int screenHeight);
	bool handleEvent(SDL_Event* event);
};
