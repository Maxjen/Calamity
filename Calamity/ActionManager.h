#pragma once

#include <stack>
#include "Action.h"

using std::stack;

namespace clm {

class ActionManager {
private:
    stack<Action*> undoStack;
    stack<Action*> redoStack;
public:
	explicit ActionManager();
    ~ActionManager();
    void clear();
    void pushAction(Action* newAction);
    void undoAction();
    void redoAction();
};

}
