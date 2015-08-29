#include "CreateTriangleObject.h"

CreateTriangleObject::CreateTriangleObject(MapEditorSelection* selection, unsigned int layerId, const char* textureName) :
	selection(selection),
	triangleId(-1),
	textureName(textureName),
	oldSprites(),
	oldMainSelectedSprite(-1),
	oldVertices(),
	layerId(layerId),
	v1(-1),
	v2(-1),
	v3(-1),
	firstApply(true)
{
	oldSprites = selection->getSelectedSprites();
	oldMainSelectedSprite = selection->getMainSelectedSprite();
	oldVertices = selection->getSelectedVertices();
}

void CreateTriangleObject::setVertices(unsigned int v1, unsigned int v2, unsigned int v3) {
	this->v1 = v1;
	this->v2 = v2;
	this->v3 = v3;
}

void CreateTriangleObject::apply() {
	GameManager* gameManager = selection->getGameManager();

    if (firstApply) {
		for (auto& it : oldSprites)
			selection->selectSprite(it);
		for (auto& it : oldVertices)
			selection->selectVertex(it.first, it.second);

		SharedTriangleData* sharedTriangleData = selection->getSharedTriangleData();
		TriangleObject* triangle = new TriangleObject(sharedTriangleData, v1, v2, v3, textureName.c_str());
		triangleId = gameManager->addGameObject(triangle, layerId);

        firstApply = false;
    }
    else {
		for (auto& it : oldSprites)
			selection->selectSprite(it);
		for (auto& it : oldVertices)
			selection->selectVertex(it.first, it.second);

		gameManager->restoreGameObject(triangleId);
    }

	selection->selectVertex(v1, layerId);
	selection->selectVertex(v2, layerId);
	selection->selectVertex(v3, layerId);

	gameManager->getRenderer()->getTextureManager()->upload();
}

void CreateTriangleObject::revert() {
	GameManager* gameManager = selection->getGameManager();

	selection->selectVertex(v1, layerId);
	selection->selectVertex(v2, layerId);
	selection->selectVertex(v3, layerId);

	gameManager->removeGameObject(triangleId);

	for (auto& it : oldSprites)
		selection->selectSprite(it);
	for (auto& it : oldVertices)
		selection->selectVertex(it.first, it.second);

	selection->setMainSelectedSprite(oldMainSelectedSprite);
}
