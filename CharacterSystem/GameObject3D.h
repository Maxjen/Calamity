#pragma once

#include <glm/glm.hpp>
//#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>

enum Object3DType {
	MESH_OBJECT,
	CLOTH_OBJECT,
	CHARACTER_OBJECT,
	TEST_OBJECT,
};

enum CollisionType {
	COL_ALL = -1,
	COL_NOTHING = 0,
	COL_CHARACTER = 1,
	COL_CONTROLLER = 2,
	COL_WORLD = 4,
};

class GameObject3D {
protected:
	explicit GameObject3D() : position(), scale() {}

	glm::vec3 position;
	glm::vec3 scale;
public:
	virtual int getObjectType() = 0;
	virtual glm::vec3 getPosition();
	virtual void setPosition(float posX, float posY, float posZ);
	virtual void setScale(float scaleX, float scaleY, float scaleZ);
};
