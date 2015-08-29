#include "RemoveLayer.h"

RemoveLayer::RemoveLayer(LayerManager* layerManager, MapEditorSelection* selection, ListView* layerList, unsigned int listViewFont, unsigned int layerId, Functor* functor) :
	layerManager(layerManager),
	selection(selection),
	layerList(layerList),
	listViewFont(listViewFont),
	layerId(layerId),
	newLayerId(-1),

	spritesToDeselect(),
	verticesToDeselect(),

	functor(functor),
	firstApply(true)
{
	GameManager* gameManager = selection->getGameManager();
	for (auto& it : selection->getSelectedSprites()) {
		if (gameManager->getObject(it)->layerId == layerId)
			spritesToDeselect.push_back(it);
	}

	for (auto& it : selection->getSelectedVertices()) {
		if (it.second == layerId)
			verticesToDeselect[it.first] = layerId;
	}
}

void RemoveLayer::apply() {
	if (firstApply) {
		layerManager->selectLowerLayer();
		newLayerId = layerManager->getSelectedLayerId();

		firstApply = false;
	}

	for (auto& it : spritesToDeselect)
		selection->selectSprite(it);

	for (auto& it : verticesToDeselect)
		selection->selectVertex(it.first, it.second);

	layerManager->removeLayer(layerId);

	//reloadLayerList();
	layerList->reload();
	layerManager->setSelectedLayerId(newLayerId);
	(*functor)();
}

void RemoveLayer::revert() {
	layerManager->restoreLayer(layerId);

	for (auto& it : spritesToDeselect)
		selection->selectSprite(it);

	for (auto& it : verticesToDeselect)
		selection->selectVertex(it.first, it.second);

	//reloadLayerList();
	layerList->reload();
	layerManager->setSelectedLayerId(layerId);
	(*functor)();
}

/*void RemoveLayer::reloadLayerList() {
	layerManager->setSelectedLayerId(-1);
	layerList->reload();
}*/
