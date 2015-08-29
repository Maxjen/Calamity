#include "Selection3D.h"

Selection3D::Selection3D() :
	meshObjects()
{}

void Selection3D::selectMeshObject(MeshObject* meshObject) {
	if (meshObjects.find(meshObject) != meshObjects.end())
		meshObjects.erase(meshObject);
	else
		meshObjects.insert(meshObject);
}

list<MeshObject*> Selection3D::getSelectedMeshObjects() {
	list<MeshObject*> result;
	for (auto& it : meshObjects)
		result.push_back(it);
	return result;
}

/*void Selection3D::updateSelection() {
	Renderer* renderer = spriteManager->getRenderer();

	for (auto& it : selectedSprites) {
		gameManager->updateObjectAABB(it);
	}

	for (auto& it : selectedTriangles) {
		TriangleObject* cTriangle = (TriangleObject*)gameManager->getObject(it);
		cTriangle->updateVertices();
		gameManager->updateObjectAABB(it);
	}

	for (auto& it : selectionEdges) {
		Vec2 pos = sharedTriangleData->getSharedVertexPosition(it.first.first);
		renderer->setColorVertexPosition(it.second.v1, pos.x, pos.y);
		pos = sharedTriangleData->getSharedVertexPosition(it.first.second);
		renderer->setColorVertexPosition(it.second.v2, pos.x, pos.y);
	}
}*/
