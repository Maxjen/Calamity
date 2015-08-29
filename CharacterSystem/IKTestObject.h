#pragma once

#include <cmath>

#include "GameObject3D.h"
#include "DebugDraw.h"

using namespace clm;

class IKTestObject : public GameObject3D {
private:
	struct IKLink {
		IKLink() { child = nullptr; }
		~IKLink() { if (child) {delete child;} }
		IKLink* child;

		//glm::vec3 rotation;
		glm::quat rotation;
		float angle;
		glm::vec3 offset;
		glm::mat4 globalTransformation;
	};

	void updateLinks(IKLink* link, glm::mat4 parentTransform);
	void renderLink(IKLink* link, IKLink* parent);

	DebugDraw* debugDraw;

	IKLink* linkHierarchy;
	glm::vec3 endEffector;
	glm::vec3 targetPosition;
public:
	explicit IKTestObject(DebugDraw* debugDraw);
	~IKTestObject();

	int getObjectType();
	void setPosition(float posX, float posY, float posZ);

	void ikStep();

	void render();
};
