#pragma once

#include "../../Calamity/Action.h"
#include "../../Common/SpriteObject.h"
#include "../MapEditorSelection.h"

using namespace clm;

class CreateSpriteObject : public Action {
private:
	SpriteManager* spriteManager;
	MapEditorSelection* selection;
	unsigned int layerId;
	unsigned int objectId;
	float posX, posY;
	string textureName;

	list<unsigned int> oldSprites;
	unsigned int oldMainSelectedSprite;
	unordered_map<unsigned int, unsigned int> oldVertices;

    bool firstApply;
public:
	explicit CreateSpriteObject(SpriteManager* spriteManager, MapEditorSelection* selection, float posX, float posY, const char* textureName, unsigned int layerId);
    void apply();
    void revert();
};
