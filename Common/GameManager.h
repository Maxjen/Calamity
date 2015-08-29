#pragma once

#include <memory>
#include <unordered_map>
#include <list>
#include <string>

#include "Renderer/Renderer.h"
#include "Renderer/RenderLayer.h"
#include "SpriteManager.h"

/*#include <set>
using std::set;
*/

using std::unique_ptr;
using std::unordered_map;
using std::list;
using std::string;

#include "Box2D/Collision/b2DynamicTree.h"

using namespace clm;

enum ObjectType {
	SPRITE_OBJECT,
	TRIANGLE_OBJECT,
	BONE_OBJECT,
};

class GameObject {
public:
	virtual ~GameObject() {}
	virtual void setRenderLayers(RenderLayer* renderLayer, RenderLayer* overlayLayer) = 0;
	virtual b2AABB getAABB() = 0;
	virtual void remove() = 0;
	virtual void restore() = 0;
	virtual int getObjectType() = 0;
	virtual bool containsPoint(float x, float y) { return true; };
	virtual void flagForRender() = 0;

	unsigned int objectId;
	unsigned int layerId;
	unsigned int zOrder;
	int proxyId;
};

class QueryWrapper {
public:
	bool QueryCallback(int32 proxyId);

	b2DynamicTree* tree;
	list<unsigned int> result;
};

class GameLayer {
private:
	unique_ptr<RenderLayer> renderLayer;
	unique_ptr<RenderLayer> overlayLayer;
	SpriteManager* spriteManager;
	b2DynamicTree tree;
	list<GameObject*> objectsToRender;

	// here depth is mapped to objects (to allow easier access to neighboring objects for reordering)
	unordered_map<unsigned int, GameObject*> objects;
	unsigned int objectCount;

	float scrollSpeed;
	float viewPosX;
	float viewPosY;
public:
	explicit GameLayer(SpriteManager* spriteManager, float scrollSpeed);

	//void addGameObject(GameObject* object, float posX, float posY, const char* textureName);
	void addGameObject(GameObject* object);
	void removeGameObject(unsigned int zOrder);
	void restoreGameObject(GameObject* object);
	bool QueryCallback(int32 proxyId);

	void setProxyAABB(int proxyId, b2AABB aabb);

	void prepare(float viewPosX, float viewPosY, float eyeOffset);
	void draw(float eyeOffset);
	void drawOverlay(float eyeOffset);
	void clear();

	float getScrollSpeed();
	void setScrollSpeed(float scrollSpeed);
	list<unsigned int> getObjectsAtPosition(float posX, float posY);
	RenderLayer* getOverlayLayer();
};

class GameManager {
private:
	SpriteManager* spriteManager;

	unordered_map<unsigned int, GameLayer*> layers;
	unsigned int layerCount;

	unordered_map<unsigned int, GameObject*> objects;
	unsigned int objectCount;
public:
	explicit GameManager(SpriteManager* spriteManager);
	~GameManager();

	unsigned int addGameLayer(float scrollSpeed);
	void removeGameLayer(unsigned int layerId);
	void restoreGameLayer(unsigned int layerId);
	void prepareLayer(unsigned int layerId, float viewPosX, float viewPosY, float eyeOffset);
	void drawLayer(unsigned int layerId, float eyeOffset);
	void drawOverlay(unsigned int layerId, float eyeOffset);
	void clearLayer(unsigned int layerId);

	//unordered_map<unsigned int, GameLayer*>* getLayers();
	list<unsigned int> getLayerIds();
	// TODO: move to layerManager
	//string getLayerName(unsigned int layerId);
	float getLayerScrollSpeed(unsigned int layerId);
	void setLayerScrollSpeed(unsigned int layerId, float scrollSpeed);
	list<unsigned int> getObjectsAtPosition(unsigned int layerId, float posX, float posY);
	GameObject* getObject(unsigned int objectId);
	RenderLayer* getOverlayLayer(unsigned int layerId);

	//unsigned int addGameObject(float posX, float posY, const char* textureName, unsigned int layerId);
	unsigned int addGameObject(GameObject* object, unsigned int layerId);
	void removeGameObject(unsigned int objectId);
	void restoreGameObject(unsigned int objectId);
	void deleteGameObject(unsigned int objectId);
	void updateObjectAABB(unsigned int objectId);

	Renderer* getRenderer();
};
