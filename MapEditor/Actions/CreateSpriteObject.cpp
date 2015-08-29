#include "CreateSpriteObject.h"

CreateSpriteObject::CreateSpriteObject(SpriteManager* spriteManager, MapEditorSelection* selection, float posX, float posY, const char* textureName, unsigned int layerId) :
	spriteManager(spriteManager),
	selection(selection),
	layerId(layerId),
	objectId(-1),
	posX(posX),
	posY(posY),
	textureName(textureName),
	oldSprites(),
	oldMainSelectedSprite(-1),
	oldVertices(),
	firstApply(true)
{
	oldSprites = selection->getSelectedSprites();
	oldMainSelectedSprite = selection->getMainSelectedSprite();
	oldVertices = selection->getSelectedVertices();
}

void CreateSpriteObject::apply() {
	GameManager* gameManager = selection->getGameManager();
    if (firstApply) {
		for (auto& it : oldSprites)
			selection->selectSprite(it);
		for (auto& it : oldVertices)
			selection->selectVertex(it.first, it.second);

		SpriteObject* object = new SpriteObject(spriteManager, posX, posY, textureName.c_str());
		objectId = gameManager->addGameObject(object, layerId);

        firstApply = false;
    }
    else {
		for (auto& it : oldSprites)
			selection->selectSprite(it);
		for (auto& it : oldVertices)
			selection->selectVertex(it.first, it.second);

		gameManager->restoreGameObject(objectId);
    }

	selection->selectSprite(objectId);
	selection->setMainSelectedSprite(objectId);
}

void CreateSpriteObject::revert() {
	GameManager* gameManager = selection->getGameManager();
	selection->selectSprite(objectId);
	gameManager->removeGameObject(objectId);

	for (auto& it : oldSprites)
		selection->selectSprite(it);
	for (auto& it : oldVertices)
		selection->selectVertex(it.first, it.second);

	selection->setMainSelectedSprite(oldMainSelectedSprite);
}
