#include "CreateTriangleHandler.h"

CreateTriangleHandler::CreateTriangleHandler(MapEditorSelection* selection, ActionManager* actionManager, unsigned int layerId, const char* textureName, float mouseX, float mouseY) :
	selection(selection),
	actionManager(actionManager),
	action(new CreateTriangleObject(selection, layerId, textureName)),
	layerId(layerId),
	mousePos(Vec2(mouseX, mouseY)),
	v1(-1),
	v2(-1),
	v3(-1),
	v1Tmp(-1),
	v2Tmp(-1),
	v3Tmp(-1),
	timesClicked(1)
{
	SharedTriangleData* sharedTriangleData = selection->getSharedTriangleData();
	v1 = getClosestVertex();
	if (v1 == (unsigned int)-1)
		v1 = sharedTriangleData->addSharedVertex(mousePos.x, mousePos.y, 255, 255, 255, 255);

	RenderLayer* overlayLayer = selection->getGameManager()->getOverlayLayer(layerId);
	Renderer* renderer = overlayLayer->getRenderer();

	Vec2 pos = sharedTriangleData->getSharedVertexPosition(v1);
	v1Tmp = renderer->addColorVertex(pos.x, pos.y, 255, 255, 255, 255);
	v2Tmp = renderer->addColorVertex(mousePos.x, mousePos.y, 255, 255, 255, 255);
	v3Tmp = renderer->addColorVertex(0, 0, 255, 255, 255, 255);

	//overlayLayer->addLine(v1Tmp, v2Tmp);
}

void CreateTriangleHandler::render() {
	RenderLayer* overlayLayer = selection->getGameManager()->getOverlayLayer(layerId);

	if (timesClicked == 1) {
		overlayLayer->addLine(v1Tmp, v2Tmp);
	}
	else {
		overlayLayer->addLine(v1Tmp, v2Tmp);
		overlayLayer->addLine(v2Tmp, v3Tmp);
		overlayLayer->addLine(v3Tmp, v1Tmp);
	}
}

bool CreateTriangleHandler::handleEvent(SDL_Event* event) {
	RenderLayer* overlayLayer = selection->getGameManager()->getOverlayLayer(layerId);
	Renderer* renderer = overlayLayer->getRenderer();
	GameManager* gameManager = selection->getGameManager();
	SharedTriangleData* sharedTriangleData = selection->getSharedTriangleData();

	switch (event->type) {
		case SDL_MOUSEMOTION:
		{
			mousePos.x += event->motion.xrel;
			mousePos.y -= event->motion.yrel;

			if (timesClicked == 1) {
				unsigned int closestVertex = getClosestVertex();
				if (closestVertex == (unsigned int)-1)
					renderer->setColorVertexPosition(v2Tmp, mousePos.x, mousePos.y);
				else {
					Vec2 pos = sharedTriangleData->getSharedVertexPosition(closestVertex);
					renderer->setColorVertexPosition(v2Tmp, pos.x, pos.y);
				}
			}
			else {
				unsigned int closestVertex = getClosestVertex();
				if (closestVertex == (unsigned int)-1)
					renderer->setColorVertexPosition(v3Tmp, mousePos.x, mousePos.y);
				else {
					Vec2 pos = sharedTriangleData->getSharedVertexPosition(closestVertex);
					renderer->setColorVertexPosition(v3Tmp, pos.x, pos.y);
				}
			}

			/*list<unsigned int>::iterator sprite = spritesToMove.begin();
			list<Vec2>::iterator posIt = startingSpritePositions.begin();
			for (; sprite != spritesToMove.end(); ++sprite, ++posIt) {
				Vec2 newPosition = (*posIt) + deltaPosition;
				((SpriteObject*)gameManager->getObject(*sprite))->setPosition(newPosition.x, newPosition.y);
			}

			SharedTriangleData* sharedTriangleData = selection->getSharedTriangleData();
			unordered_map<unsigned int, unsigned int>::iterator vertex = verticesToMove.begin();
			posIt = startingVertexPositions.begin();
			for (; vertex != verticesToMove.end(); ++vertex, ++posIt) {
				Vec2 newPosition = (*posIt) + deltaPosition;
				sharedTriangleData->setSharedVertexPosition(vertex->first, newPosition.x, newPosition.y);
			}

			selection->updateSelection();
			selection->updateProperties();*/

			break;
		}
		case SDL_MOUSEBUTTONDOWN:
			if (event->button.button == SDL_BUTTON_LEFT) {
				if (timesClicked == 1) {
					v2 = getClosestVertex();
					if (v2 == (unsigned int)-1)
						v2 = sharedTriangleData->addSharedVertex(mousePos.x, mousePos.y, 255, 255, 255, 255);
					renderer->setColorVertexPosition(v3Tmp, mousePos.x, mousePos.y);

					++timesClicked;
				}
				else {
					renderer->removeColorVertex(v1Tmp);
					renderer->removeColorVertex(v2Tmp);
					renderer->removeColorVertex(v3Tmp);

					v3 = getClosestVertex();
					if (v3 == (unsigned int)-1) {
						v3 = sharedTriangleData->addSharedVertex(mousePos.x, mousePos.y, 255, 255, 255, 255);
						action->setVertices(v1, v2, v3);
						actionManager->pushAction(action);
					}
					else {
						// check if triangle already exists
						bool bTriangleExists = false;
						Vec2 pos = sharedTriangleData->getSharedVertexPosition(v3);
						list<unsigned int> objects = gameManager->getObjectsAtPosition(layerId, pos.x, pos.y);
						for (auto& it : objects) {
							GameObject* cObject = gameManager->getObject(it);
							if (cObject->getObjectType() == TRIANGLE_OBJECT) {
								TriangleObject* cTriangle = (TriangleObject*)cObject;
								if (cTriangle->containsVertex(v1) && cTriangle->containsVertex(v2) && cTriangle->containsVertex(v3)) {
									bTriangleExists = true;
									break;
								}
							}
						}
						if (!bTriangleExists) {
							action->setVertices(v1, v2, v3);
							actionManager->pushAction(action);
						}
						/*else {
						 * // could get rid of newly created shared vertices here
						}*/
					}

					return true;
				}
			}
			break;
		default:
			break;
	}

	return false;
}

unsigned int CreateTriangleHandler::getClosestVertex() {
	GameManager* gameManager = selection->getGameManager();

	list<unsigned int> objectsAtMouse = gameManager->getObjectsAtPosition(layerId, mousePos.x, mousePos.y);
	unsigned int closestVertex = -1;
	float minDistanceSq = 400.0f;
	for (auto& it : objectsAtMouse) {
		GameObject* object = gameManager->getObject(it);
		if (object->getObjectType() == TRIANGLE_OBJECT) {
			pair<unsigned int, float> v = ((TriangleObject*)object)->getClosestVertex(mousePos.x, mousePos.y);
			if (v.first != (unsigned int)-1) {
				if (v.second < minDistanceSq) {
					closestVertex = v.first;
					minDistanceSq = v.second;
				}
			}
		}
	}
	return closestVertex;
}
