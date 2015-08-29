#pragma once

#include "../../ActionHandler.h"
#include "../LineEdit.h"
#include "../../Functor.h"

using namespace clm;

namespace clm {
	class LineEdit;
}

class LineEditHandler : public ActionHandler {
private:
	LineEdit* lineEdit;
	int screenHeight;
	wstring oldValue;
	wstring newValue;
	unsigned int cursorPos;
public:
	explicit LineEditHandler(LineEdit* lineEdit, int screenHeight);
	void setNewValue();
	bool handleEvent(SDL_Event* event);
};
