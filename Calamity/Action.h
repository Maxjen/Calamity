#pragma once

namespace clm {

class Action {
public:
    //virtual Action* clone() = 0;

    //virtual bool apply() = 0; //return true if undoable
	virtual ~Action() {}

    virtual void apply() = 0;
    virtual void revert() = 0;
};

}
