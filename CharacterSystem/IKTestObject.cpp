#include "IKTestObject.h"

IKTestObject::IKTestObject(DebugDraw* debugDraw) :
	debugDraw(debugDraw)
{
	linkHierarchy = new IKLink();
	IKLink* link1 = new IKLink();
	IKLink* link2 = new IKLink();
	IKLink* link3 = new IKLink();

	linkHierarchy->child = link1;
	link1->child = link2;
	link2->child = link3;

	//linkHierarchy->rotation = glm::vec3(0, 0, 0);
	linkHierarchy->rotation = glm::quat(glm::vec3(0, 0, 0));
	linkHierarchy->angle = 0;
	linkHierarchy->offset = glm::vec3(0, 0, 0);

	//link1->rotation = glm::vec3(0, 0, 0);
	link1->rotation = glm::quat(glm::vec3(0, 0, 0));
	link1->angle = 0;
	link1->offset = glm::vec3(0, 1, 0);

	//link2->rotation = glm::vec3(0, 0, 0);
	link2->rotation = glm::quat(glm::vec3(0, 0, 0));
	link2->angle = 0;
	link2->offset = glm::vec3(0, 2, 0);

	//link3->rotation = glm::vec3(0, 0, 0);
	link3->rotation = glm::quat(glm::vec3(0, 0, 0));
	link3->angle = 0;
	link3->offset = glm::vec3(0, 1, 0);
}

IKTestObject::~IKTestObject() {
	delete linkHierarchy;
}

int IKTestObject::getObjectType() {
	return TEST_OBJECT;
}

void IKTestObject::setPosition(float posX, float posY, float posZ) {
	position = glm::vec3(posX, posY, posZ);
	targetPosition = position + glm::vec3(2, 1, 2);
}

void IKTestObject::ikStep() {
	IKLink* currentLink = linkHierarchy;
	vector<glm::vec3> J;
	while (currentLink) {
		glm::vec3 jointPos(currentLink->globalTransformation * glm::vec4(0, 0, 0, 1));

		//glm::vec3 j = glm::cross(glm::vec3(0, 0, 1), endEffector - jointPos);
		//J.push_back(j);

		glm::vec3 j;
		j = glm::vec3(currentLink->globalTransformation * glm::vec4(1, 0, 0, 0));
		j = glm::cross(j, endEffector - jointPos);
		J.push_back(j);
		j = glm::vec3(currentLink->globalTransformation * glm::vec4(0, 0, 1, 0));
		j = glm::cross(j, endEffector - jointPos);
		J.push_back(j);

		currentLink = currentLink->child;
	}

	// TODO: change to mat3
	vector<glm::vec3> J_JT;
	for (unsigned int i = 0; i < 3; ++i) {
		glm::vec3 rowI;
		for (unsigned int j = 0; j < 3; ++j) {
			float c = 0;
			for (unsigned int k = 0; k < J.size(); ++k)
				c += J[k][i] * J[k][j];
			rowI[j] = c;
		}
		J_JT.push_back(rowI);
	}

	glm::vec3 e = targetPosition - endEffector;

	glm::vec3 J_JT_e;
	for (unsigned int i = 0; i < 3; ++i)
		J_JT_e[i] = glm::dot(J_JT[i], e);

	float alpha = glm::dot(e, J_JT_e) / glm::dot(J_JT_e, J_JT_e);

	currentLink = linkHierarchy;
	unsigned int cRowJT = 0;
	while (currentLink) {
		//currentLink->angle += alpha * glm::dot(J[cRowJT], e);
		//++cRowJT;

		float xRot = alpha * glm::dot(J[cRowJT], e);
		++cRowJT;
		float zRot = alpha * glm::dot(J[cRowJT], e);
		++cRowJT;
		glm::quat rotDelta(glm::vec3(xRot, 0, zRot));
		currentLink->rotation = currentLink->rotation * rotDelta;

		currentLink = currentLink->child;
	}
}

void IKTestObject::render() {
	updateLinks(linkHierarchy, glm::translate(glm::mat4(), position));
	renderLink(linkHierarchy, nullptr);
}

void IKTestObject::updateLinks(IKLink* link, glm::mat4 parentTransform) {
	glm::mat4 translation = glm::translate(glm::mat4(), link->offset);
	glm::mat4 rotationM = glm::mat4_cast(link->rotation);
	//glm::mat4 rotation = glm::rotate(glm::mat4(), (float)(link->angle * 180 / M_PI), glm::vec3(0, 0, 1));
	link->globalTransformation = parentTransform * translation * rotationM;

	if (link->child)
		updateLinks(link->child, link->globalTransformation);
	else {
		endEffector = glm::vec3(link->globalTransformation * glm::vec4(0, 0, 0, 1));
		/*printf("endEffector %f %f %f\n", endEffector.x, endEffector.y, endEffector.z);
		printf("targetPosition %f %f %f\n", targetPosition.x, targetPosition.y, targetPosition.z);*/
	}
}

void IKTestObject::renderLink(IKLink* link, IKLink* parent) {
	/*float s = sin(link->angle);
	float c = cos(link->angle);

	printf("\ns %f c %f\n", s, c);

	glm::vec3 toTmp(0, link->length, 0);

	printf("%f %f %f\n", toTmp.x, toTmp.y, toTmp.z);
	{
		glm::vec3 toTmp2 = toTmp;
		toTmp.x = toTmp2.x * c - toTmp2.y * s;
		toTmp.y = toTmp2.x * s + toTmp2.y * c;
	}
	printf("%f %f %f\n", toTmp.x, toTmp.y, toTmp.z);*/

	if (parent) {
		glm::vec4 start(0, 0, 0, 1);
		glm::vec4 end(0, 0, 0, 1);

		start = parent->globalTransformation * start;
		end = link->globalTransformation * end;

		debugDraw->drawPoint(start.x, start.y, start.z, 255, 255, 255, 255);
		debugDraw->drawPoint(end.x, end.y, end.z, 255, 255, 255, 255);
		debugDraw->drawLine(btVector3(start.x, start.y, start.z), btVector3(end.x, end.y, end.z), btVector3(1, 1, 1));
	}

	if (link->child)
		renderLink(link->child, link);
	else {
		debugDraw->drawPoint(endEffector.x, endEffector.y, endEffector.z, 255, 255, 0, 255);
		debugDraw->drawPoint(targetPosition.x, targetPosition.y, targetPosition.z, 255, 0, 0, 255);
	}
}
