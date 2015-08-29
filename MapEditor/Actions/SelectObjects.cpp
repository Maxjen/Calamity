#include "SelectObjects.h"
#include <algorithm>

SelectObjects::SelectObjects(MapEditorSelection* selection, bool replaceCurrentSelection) :
	selection(selection),
	oldSprites(),
	oldMainSelectedSprite(-1),
	oldVertices(),
	newSprites(),
	newMainSelectedSprite(-1),
	newVertices()
{
	if(replaceCurrentSelection) {
		oldSprites = selection->getSelectedSprites();
		oldVertices = selection->getSelectedVertices();
	}
	oldMainSelectedSprite = selection->getMainSelectedSprite();
	newMainSelectedSprite = oldMainSelectedSprite;
}

void SelectObjects::setSpritesToSelect(list<unsigned int> newSprites) {
	this->newSprites = newSprites;
	newMainSelectedSprite = newSprites.back();
}

void SelectObjects::setVerticesToSelect(unordered_map<unsigned int, unsigned int> newVertices) {
	this->newVertices = newVertices;
}

void SelectObjects::apply() {
	for (auto& it : oldSprites)
		selection->selectSprite(it);
	for (auto& it : oldVertices)
		selection->selectVertex(it.first, it.second);

	for (auto& it : newSprites)
		selection->selectSprite(it);
	for (auto& it : newVertices)
		selection->selectVertex(it.first, it.second);

	selection->setMainSelectedSprite(newMainSelectedSprite);
}

void SelectObjects::revert() {
	for (auto& it : newSprites)
		selection->selectSprite(it);
	for (auto& it : newVertices)
		selection->selectVertex(it.first, it.second);

	for (auto& it : oldSprites)
		selection->selectSprite(it);
	for (auto& it : oldVertices)
		selection->selectVertex(it.first, it.second);

	selection->setMainSelectedSprite(oldMainSelectedSprite);
}
