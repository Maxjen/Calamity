#include "ActionManager.h"

namespace clm {

ActionManager::ActionManager() :
	undoStack(),
	redoStack()
{}

ActionManager::~ActionManager() {
    clear();
}

void ActionManager::clear() {
    while(!undoStack.empty()) {
        delete undoStack.top();
        undoStack.pop();
    }
    while(!redoStack.empty()) {
        delete redoStack.top();
        redoStack.pop();
    }
}

void ActionManager::pushAction(Action* newAction) {
    newAction->apply();
    undoStack.push(newAction);
    while(!redoStack.empty()) {
        delete redoStack.top();
        redoStack.pop();
    }
}

void ActionManager::undoAction() {
    if (!undoStack.empty()) {
        undoStack.top()->revert();
        redoStack.push(undoStack.top());
        undoStack.pop();
    }
}

void ActionManager::redoAction() {
    if (!redoStack.empty()) {
        redoStack.top()->apply();
        undoStack.push(redoStack.top());
        redoStack.pop();
    }
}

}
