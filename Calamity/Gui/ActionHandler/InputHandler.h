#pragma once

#include "../../ActionHandler.h"
#include "../InputGroup.h"
#include "../../Functor.h"

using namespace clm;

namespace clm {
	class Input;
}

class InputHandler : public ActionHandler {
private:
	Input* input;
	int screenHeight;
	wstring oldValue;
	wstring newValue;
	unsigned int cursorPos;
public:
	explicit InputHandler(Input* input, int screenHeight);
	void setNewValue();
	bool handleEvent(SDL_Event* event);
};
