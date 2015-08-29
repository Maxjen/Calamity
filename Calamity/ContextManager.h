#pragma once

#include <map>
#include <list>

#include "Context.h"

using std::map;
using std::list;

namespace clm {

class ContextManager {
private:
	map<unsigned int, Context*> contexts;
	unsigned int cContext;

	Context* activeContext;
public:
	explicit ContextManager();
	~ContextManager();
	unsigned int addContext(Context* context);

	Context* getActiveContext();
	void setActiveContext(unsigned int contextId);

	string getContextName(unsigned int contextId);

	list<unsigned int> getContextIds();
};

}
