#include "ContextManager.h"

namespace clm {

ContextManager::ContextManager() :
	contexts(),
	cContext(0),
	activeContext(nullptr)
{}

ContextManager::~ContextManager() {
	for (auto& it : contexts)
		delete it.second;
}

unsigned int ContextManager::addContext(Context* context) {
	context->setContextId(cContext);
	contexts[cContext] = context;
	return cContext++;
}

Context* ContextManager::getActiveContext() {
	return activeContext;
}

void ContextManager::setActiveContext(unsigned int contextId) {
	if (contexts.find(contextId) != contexts.end()) {
		if (activeContext)
			activeContext->deactivate();
		activeContext = contexts[contextId];
		activeContext->activate();
	}
}

string ContextManager::getContextName(unsigned int contextId) {
	string result = "";
	if (contexts.find(contextId) != contexts.end())
		result = contexts[contextId]->getContextName();
	return result;
}

list<unsigned int> ContextManager::getContextIds() {
	list<unsigned int> result;
	for (auto& it : contexts)
		result.push_back(it.first);
	return result;
}

}
