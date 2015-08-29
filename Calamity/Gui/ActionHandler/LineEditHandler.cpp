#include "LineEditHandler.h"

LineEditHandler::LineEditHandler(LineEdit* lineEdit, int screenHeight) :
	lineEdit(lineEdit),
	screenHeight(screenHeight),
	oldValue(lineEdit->getValue()),
	newValue(oldValue),
	cursorPos(0)
{
	cursorPos = newValue.length();
	lineEdit->setEditMode(true);
	lineEdit->setCursorPos(cursorPos);
}

void LineEditHandler::setNewValue() {
	Property* property = lineEdit->getProperty();
	if (property->isValid(newValue)) {
		wstring oldValue = property->getValueAsString();

		newValue = property->correctValue(newValue);
		lineEdit->setValue(newValue);

		Properties* properties = lineEdit->getProperties();
		properties->pushPropertyChange(property, oldValue, newValue);
	}
	else {
		lineEdit->setValue(oldValue);
	}
}

bool LineEditHandler::handleEvent(SDL_Event* event) {
	switch (event->type) {
		case SDL_TEXTINPUT:
		{
			string stext(event->text.text);
			wstring wtext(stext.begin(), stext.end());
			newValue.insert(cursorPos, wtext);
			++cursorPos;
			lineEdit->setValue(newValue);
			lineEdit->setCursorPos(cursorPos);
			break;
		}
		case SDL_KEYDOWN:
			if (event->key.keysym.sym == SDLK_RETURN) {
				lineEdit->setEditMode(false);
				setNewValue();
				return true;
			}
			else if (event->key.keysym.sym == SDLK_LEFT) {
				if (cursorPos > 0) {
					--cursorPos;
					lineEdit->setCursorPos(cursorPos);
				}
			}
			else if (event->key.keysym.sym == SDLK_RIGHT) {
				if (cursorPos < newValue.size()) {
					++cursorPos;
					lineEdit->setCursorPos(cursorPos);
				}
			}
			else if (event->key.keysym.sym == SDLK_BACKSPACE) {
				if (cursorPos > 0) {
					newValue.erase(--cursorPos, 1);
					lineEdit->setValue(newValue);
					lineEdit->setCursorPos(cursorPos);
				}
			}
			break;
		case SDL_MOUSEBUTTONDOWN:
			if (event->button.button == SDL_BUTTON_LEFT && !lineEdit->contains(event->button.x, screenHeight - event->button.y)) {
				lineEdit->setEditMode(false);
				setNewValue();
				return true;
			}
			break;
		default:
			break;
	}

	return false;
}
