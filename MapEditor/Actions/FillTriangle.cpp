#include "FillTriangle.h"

FillTriangle::FillTriangle(MapEditorSelection* selection, const char* textureName) :
	selection(selection),
	triangleId(-1),
	textureName(textureName),
	verticesToReselect(),
	firstApply(true)
{
	// size must be 3 and they must be on the same layer
	verticesToReselect = selection->getSelectedVertices();
}

void FillTriangle::apply() {
	GameManager* gameManager = selection->getGameManager();

    if (firstApply) {
		SharedTriangleData* sharedTriangleData = selection->getSharedTriangleData();

		for (auto& it : verticesToReselect)
			selection->selectVertex(it.first, it.second);

		unsigned int v1, v2, v3;
		unsigned int layerId;
		unordered_map<unsigned int, unsigned int>::iterator it = verticesToReselect.begin();
		layerId = it->second;
		v1 = it->first;
		++it;
		v2 = it->first;
		++it;
		v3 = it->first;

		TriangleObject* triangle = new TriangleObject(sharedTriangleData, v1, v2, v3, textureName.c_str());
		triangleId = gameManager->addGameObject(triangle, layerId);

		for (auto& it : verticesToReselect)
			selection->selectVertex(it.first, it.second);

		//spriteManager->uploadTextures();

        firstApply = false;
    }
    else {
		for (auto& it : verticesToReselect)
			selection->selectVertex(it.first, it.second);

		gameManager->restoreGameObject(triangleId);

		for (auto& it : verticesToReselect)
			selection->selectVertex(it.first, it.second);
    }

	gameManager->getRenderer()->getTextureManager()->upload();
}

void FillTriangle::revert() {
	GameManager* gameManager = selection->getGameManager();

	for (auto& it : verticesToReselect)
		selection->selectVertex(it.first, it.second);

	gameManager->removeGameObject(triangleId);

	for (auto& it : verticesToReselect)
		selection->selectVertex(it.first, it.second);
}
