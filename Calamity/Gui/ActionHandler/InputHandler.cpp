#include "InputHandler.h"

InputHandler::InputHandler(Input* input, int screenHeight) :
	input(input),
	screenHeight(screenHeight),
	oldValue(input->getValue()),
	newValue(oldValue),
	cursorPos(0)
{
	cursorPos = newValue.length();
	input->setEditMode(true);
	input->setCursorPos(cursorPos);
}

void InputHandler::setNewValue() {
	Property* property = input->getProperty();
	if (property) {
		if (property->isValid(newValue)) {
			wstring oldValue = property->getValueAsString();

			newValue = property->correctValue(newValue);
			input->setValue(newValue);

			Properties* properties = input->getProperties();
			properties->pushPropertyChange(property, oldValue, newValue);
		}
		else {
			input->setValue(oldValue);
		}
	}

	/*switch (input->getInputType()) {
		case CLM_FLOAT:
			try {
				float newValuef;
				newValuef = stof(newValue);
				char buffer[10];
				snprintf(buffer, 10, "%.3f", newValuef);
				string newValueS = buffer;
				newValue.assign(newValueS.begin(), newValueS.end());
				input->setValue(newValue);
			}
			catch(...) {
				input->setValue(oldValue);
			}
			break;
		case CLM_INT:
			try {
				int newValueI;
				newValueI = stoi(newValue);
				char buffer[10];
				snprintf(buffer, 10, "%d", newValueI);
				string newValueS = buffer;
				newValue.assign(newValueS.begin(), newValueS.end());
				input->setValue(newValue);
			}
			catch(...) {
				input->setValue(oldValue);
			}
			break;
		case CLM_UINT:
			try {
				//int newValueU;
				//newValueU = stoi(newValue);
				//newValueU = newValueU < 0 ? 0 : newValueU;
				//char buffer[10];
				//snprintf(buffer, 10, "%d", newValueU);
				//string newValueS = buffer;
				//newValue.assign(newValueS.begin(), newValueS.end());
				//input->setValue(newValue);
				int newValueU;
				newValueU = stoul(newValue);
				char buffer[10];
				snprintf(buffer, 10, "%u", newValueU);
				string newValueS = buffer;
				newValue.assign(newValueS.begin(), newValueS.end());
				input->setValue(newValue);
			}
			catch(...) {
				input->setValue(oldValue);
			}
			break;
	}*/
}

bool InputHandler::handleEvent(SDL_Event* event) {
	switch (event->type) {
		/*case SDL_MOUSEBUTTONUP:
			if (event->button.button == SDL_BUTTON_LEFT) {
				input->setPressed(false);
				if (input->contains(event->button.x, screenHeight - event->button.y) && functor) {
					(*functor)();
				}
				return true;
			}
			break;*/
		case SDL_TEXTINPUT:
		{
			string stext(event->text.text);
			wstring wtext(stext.begin(), stext.end());
			newValue.insert(cursorPos, wtext);
			++cursorPos;
			input->setValue(newValue);
			input->setCursorPos(cursorPos);
			break;
		}
		case SDL_KEYDOWN:
			if (event->key.keysym.sym == SDLK_RETURN) {
				input->setEditMode(false);
				setNewValue();
				return true;
			}
			else if (event->key.keysym.sym == SDLK_LEFT) {
				if (cursorPos > 0) {
					--cursorPos;
					input->setCursorPos(cursorPos);
				}
			}
			else if (event->key.keysym.sym == SDLK_RIGHT) {
				if (cursorPos < newValue.size()) {
					++cursorPos;
					input->setCursorPos(cursorPos);
				}
			}
			else if (event->key.keysym.sym == SDLK_BACKSPACE) {
				if (cursorPos > 0) {
					newValue.erase(--cursorPos, 1);
					input->setValue(newValue);
					input->setCursorPos(cursorPos);
				}
			}
			break;
		case SDL_MOUSEBUTTONDOWN:
			if (event->button.button == SDL_BUTTON_LEFT && !input->contains(event->button.x, screenHeight - event->button.y)) {
				input->setEditMode(false);
				setNewValue();
				return true;
			}
			break;
		default:
			break;
	}

	return false;
}
