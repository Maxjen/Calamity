#include "SharedTriangleData.h"

SharedTriangleData::SharedTriangleData() :
	sharedVertexData(),
	cSharedVertex(0)
{}

bool SharedTriangleData::vertexExists(unsigned int sharedVertexId) {
	return (sharedVertexData.find(sharedVertexId) != sharedVertexData.end());
}

unsigned int SharedTriangleData::addSharedVertex(float x, float y, char r, char g, char b, char a) {
	SharedVertex& v = sharedVertexData[cSharedVertex];
	v.x = x;
	v.y = y;
	v.r = r;
	v.g = g;
	v.b = b;
	v.a = a;
	return cSharedVertex++;
}

Vec2 SharedTriangleData::getSharedVertexPosition(unsigned int sharedVertexId) {
	if (sharedVertexData.find(sharedVertexId) != sharedVertexData.end()) {
		SharedVertex& v = sharedVertexData[sharedVertexId];

		return Vec2(v.x, v.y);
	}
	else {
		printf("There is no shared vertex with id %u\n", sharedVertexId);
		return Vec2(0, 0);
	}
}

char SharedTriangleData::getSharedVertexR(unsigned int sharedVertexId) {
	return sharedVertexData[sharedVertexId].r;
}

char SharedTriangleData::getSharedVertexG(unsigned int sharedVertexId) {
	return sharedVertexData[sharedVertexId].g;
}

char SharedTriangleData::getSharedVertexB(unsigned int sharedVertexId) {
	return sharedVertexData[sharedVertexId].b;
}

char SharedTriangleData::getSharedVertexA(unsigned int sharedVertexId) {
	return sharedVertexData[sharedVertexId].a;
}

void SharedTriangleData::setSharedVertexPosition(unsigned int sharedVertexId, float x, float y) {
	SharedVertex& v = sharedVertexData[sharedVertexId];
	v.x = x;
	v.y = y;
}
