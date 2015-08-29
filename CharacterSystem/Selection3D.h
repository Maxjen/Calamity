#pragma once

#include <set>
#include <list>

#include "MeshObject.h"

using std::set;
using std::list;

using namespace clm;

class Selection3D {
private:
	set<MeshObject*> meshObjects;
public:
	explicit Selection3D();

	void selectMeshObject(MeshObject* meshObject);
	list<MeshObject*> getSelectedMeshObjects();

	//void updateSelection();
};
