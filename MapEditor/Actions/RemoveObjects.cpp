#include "RemoveObjects.h"

RemoveObjects::RemoveObjects(MapEditorSelection* selection) :
	selection(selection),
	spritesToRemove(),
	oldMainSelectedSprite(-1),
	verticesToDeselect(),
	trianglesToRemove()
{
	spritesToRemove = selection->getSelectedSprites();
	oldMainSelectedSprite = selection->getMainSelectedSprite();
	verticesToDeselect = selection->getSelectedVertices();

	GameManager* gameManager = selection->getGameManager();
	SharedTriangleData* sharedTriangleData = selection->getSharedTriangleData();

	for (auto& it : verticesToDeselect) {
		Vec2 pos = sharedTriangleData->getSharedVertexPosition(it.first);
		list<unsigned int> possibleObjects = gameManager->getObjectsAtPosition(it.second, pos.x, pos.y);
		for (auto& objectId : possibleObjects) {
			GameObject* possibleObject = gameManager->getObject(objectId);
			if (possibleObject->getObjectType() == TRIANGLE_OBJECT) {
				TriangleObject* cTriangle = (TriangleObject*)possibleObject;
				if (cTriangle->containsVertex(it.first))
					trianglesToRemove.insert(objectId);
			}
		}
	}
}

void RemoveObjects::apply() {
	GameManager* gameManager = selection->getGameManager();

	for (auto& it : spritesToRemove) {
		selection->selectSprite(it);
		gameManager->removeGameObject(it);
	}

	for (auto& it : verticesToDeselect)
		selection->selectVertex(it.first, it.second);

	for (auto& it : trianglesToRemove)
		gameManager->removeGameObject(it);
}

void RemoveObjects::revert() {
	GameManager* gameManager = selection->getGameManager();

	for (auto& it : spritesToRemove) {
		selection->getGameManager()->restoreGameObject(it);
		selection->selectSprite(it);
	}

	for (auto& it : trianglesToRemove)
		gameManager->restoreGameObject(it);

	for (auto& it : verticesToDeselect)
		selection->selectVertex(it.first, it.second);

	selection->setMainSelectedSprite(oldMainSelectedSprite);

	gameManager->getRenderer()->getTextureManager()->upload();
}
