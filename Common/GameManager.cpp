#include "GameManager.h"

bool compareGameObjects(GameObject* object1, GameObject* object2) {
	return object1->zOrder < object2->zOrder;
}

bool QueryWrapper::QueryCallback(int32 proxyId) {
	result.push_back(((GameObject*)tree->GetUserData(proxyId))->objectId);
	return true;
}

GameLayer::GameLayer(SpriteManager* spriteManager, float scrollSpeed) :
	renderLayer(),
	overlayLayer(),
	spriteManager(spriteManager),
	tree(),
	objectsToRender(),

	objects(),
	objectCount(0),

	scrollSpeed(scrollSpeed),
	viewPosX(0),
	viewPosY(0)
{
	renderLayer.reset(new RenderLayer(spriteManager->getRenderer()));
	overlayLayer.reset(new RenderLayer(spriteManager->getRenderer()));
}

/*void GameLayer::addGameObject(GameObject* object, float posX, float posY, const char* textureName) {
	object->spriteId = spriteManager->addSprite(posX, posY, textureName, renderLayer);

	b2AABB aabb;
	aabb.lowerBound = b2Vec2(posX, posY - spriteManager->getSpriteHeight(object->spriteId));
	aabb.upperBound = b2Vec2(posX + spriteManager->getSpriteWidth(object->spriteId), posY);
	object->proxyId = tree.CreateProxy(aabb, (void*)object);

	object->zOrder = objectCount;
	objects[objectCount] = object;
	++objectCount;
}*/

void GameLayer::addGameObject(GameObject* object) {
	object->setRenderLayers(renderLayer.get(), overlayLayer.get());
	b2AABB aabb = object->getAABB();
	object->proxyId = tree.CreateProxy(aabb, (void*)object);
	object->zOrder = objectCount;
	objects[objectCount] = object;
	++objectCount;
}

void GameLayer::removeGameObject(unsigned int zOrder) {
	GameObject* object = objects[zOrder];
	tree.DestroyProxy(object->proxyId);
	object->remove();
	objects.erase(zOrder);
}

void GameLayer::restoreGameObject(GameObject *object) {
	object->restore();
	b2AABB aabb = object->getAABB();
	object->proxyId = tree.CreateProxy(aabb, (void*)object);
	objects[object->zOrder] = object;
}

bool GameLayer::QueryCallback(int32 proxyId) {
	GameObject* object = (GameObject*)tree.GetUserData(proxyId);
	objectsToRender.push_back(object);
	return true;
}

void GameLayer::setProxyAABB(int proxyId, b2AABB aabb) {
	tree.MoveProxy(proxyId, aabb, b2Vec2(0, 0));
}

void GameLayer::prepare(float viewPosX, float viewPosY, float eyeOffset) {
	b2AABB query;
	query.lowerBound = b2Vec2((viewPosX - eyeOffset) * scrollSpeed, viewPosY * scrollSpeed);
	query.upperBound = b2Vec2((viewPosX + eyeOffset) * scrollSpeed + spriteManager->getScreenWidth(), viewPosY * scrollSpeed + spriteManager->getScreenHeight());
	/*query.lowerBound = b2Vec2(viewPosX * scrollSpeed + 200, viewPosY * scrollSpeed + 100);
	query.upperBound = b2Vec2(viewPosX * scrollSpeed + 600, viewPosY * scrollSpeed + 500);*/
	tree.Query(this, query);

	objectsToRender.sort(compareGameObjects);

	this->viewPosX = viewPosX;
	this->viewPosY = viewPosY;

	for (auto& it : objectsToRender)
		it->flagForRender();

	renderLayer->uploadLayerBuffers();
	overlayLayer->uploadLayerBuffers();
}

void GameLayer::draw(float eyeOffset) {
	renderLayer->getRenderer()->setSpriteShaderViewPos((viewPosX + eyeOffset) * scrollSpeed, viewPosY * scrollSpeed);
	renderLayer->getRenderer()->setTextureShaderViewPos((viewPosX + eyeOffset) * scrollSpeed, viewPosY * scrollSpeed);
	renderLayer->draw();
}

void GameLayer::drawOverlay(float eyeOffset) {
	overlayLayer->getRenderer()->setColorShaderViewPos((viewPosX + eyeOffset) * scrollSpeed, viewPosY * scrollSpeed);
	overlayLayer->draw();
}

void GameLayer::clear() {
	objectsToRender.clear();
	renderLayer->clearLayers();
	overlayLayer->clearLayers();
}

/*string GameLayer::getLayerName() {
	return layerName;
}*/

float GameLayer::getScrollSpeed() {
	return scrollSpeed;
}

void GameLayer::setScrollSpeed(float scrollSpeed) {
	this->scrollSpeed = scrollSpeed;
}

list<unsigned int> GameLayer::getObjectsAtPosition(float posX, float posY) {
	b2AABB query;
	query.lowerBound = b2Vec2(posX, posY);
	query.upperBound = b2Vec2(posX, posY);

	QueryWrapper wrapper;
	wrapper.tree = &tree;

	tree.Query(&wrapper, query);
	return wrapper.result;
}

RenderLayer* GameLayer::getOverlayLayer() {
	return overlayLayer.get();
}


GameManager::GameManager(SpriteManager* spriteManager) :
	spriteManager(spriteManager),

	layers(),
	layerCount(0),

	objects(),
	objectCount(0)
{}

GameManager::~GameManager() {
	for (auto& it : layers)
		delete it.second;
	for (auto& it : objects)
		delete it.second;
}

unsigned int GameManager::addGameLayer(float scrollSpeed) {
	layers[layerCount] = new GameLayer(spriteManager, scrollSpeed);
	return layerCount++;
}

void GameManager::removeGameLayer(unsigned int layerId) {

}

void GameManager::restoreGameLayer(unsigned int layerId) {

}

void GameManager::prepareLayer(unsigned int layerId, float viewPosX, float viewPosY, float eyeOffset) {
	layers[layerId]->prepare(viewPosX, viewPosY, eyeOffset);
}

void GameManager::drawLayer(unsigned int layerId, float eyeOffset) {
	layers[layerId]->draw(eyeOffset);
}

void GameManager::drawOverlay(unsigned int layerId, float eyeOffset) {
	layers[layerId]->drawOverlay(eyeOffset);
}

void GameManager::clearLayer(unsigned int layerId) {
	layers[layerId]->clear();
}

list<unsigned int> GameManager::getLayerIds() {
	list<unsigned int> result;
	for (auto& it : layers)
		result.push_back(it.first);
	return result;
}

/*string GameManager::getLayerName(unsigned int layerId) {
	return layers[layerId]->getLayerName();
}*/

float GameManager::getLayerScrollSpeed(unsigned int layerId) {
	return layers[layerId]->getScrollSpeed();
}

void GameManager::setLayerScrollSpeed(unsigned int layerId, float scrollSpeed) {
	layers[layerId]->setScrollSpeed(scrollSpeed);
}

list<unsigned int> GameManager::getObjectsAtPosition(unsigned int layerId, float posX, float posY) {
	return layers[layerId]->getObjectsAtPosition(posX, posY);
}

GameObject* GameManager::getObject(unsigned int objectId) {
	return objects[objectId];
}

RenderLayer* GameManager::getOverlayLayer(unsigned int layerId) {
	return layers[layerId]->getOverlayLayer();
}

unsigned int GameManager::addGameObject(GameObject* object, unsigned int layerId) {
	object->objectId = objectCount;
	object->layerId = layerId;
	objects[objectCount] = object;
	layers[layerId]->addGameObject(object);
	return objectCount++;
}

void GameManager::removeGameObject(unsigned int objectId) {
	layers[objects[objectId]->layerId]->removeGameObject(objects[objectId]->zOrder);
}

void GameManager::restoreGameObject(unsigned int objectId) {
	layers[objects[objectId]->layerId]->restoreGameObject(objects[objectId]);
}

void GameManager::deleteGameObject(unsigned int objectId) {
	layers[objects[objectId]->layerId]->removeGameObject(objects[objectId]->zOrder);
	GameObject* object = objects[objectId];
	objects.erase(objectId);
	delete object;
}

void GameManager::updateObjectAABB(unsigned int objectId) {
	layers[objects[objectId]->layerId]->setProxyAABB(objects[objectId]->proxyId, objects[objectId]->getAABB());
}

Renderer* GameManager::getRenderer() {
	return spriteManager->getRenderer();
}
