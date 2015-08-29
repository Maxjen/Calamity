#pragma once

#include <SDL2/SDL.h>

#include <string>

using std::string;

namespace clm {

class Context {
protected:
	Context() : contextId(0) {}

	unsigned int contextId;
public:
	virtual ~Context() {};
	virtual void setContextId(unsigned int contextId) { this->contextId = contextId; };
	virtual void activate() {};
	virtual void deactivate() {};
	virtual string getContextName() = 0;
	virtual bool handleEvent(SDL_Event* event) = 0;
	virtual void update(float deltaTime) {}
	virtual void frameRender() = 0;
};

}
