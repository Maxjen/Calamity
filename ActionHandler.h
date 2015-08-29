#pragma once

#include <SDL2/SDL.h>

namespace clm {

class ActionHandler {
public:
	virtual ~ActionHandler() {};
	virtual void update() {};
	virtual void render() {};
	virtual bool handleEvent(SDL_Event* event) = 0;
};

}
