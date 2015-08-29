#pragma once

//#include <glm/glm.hpp>
#include <map>

#include "Math/Vec2.h"

using std::map;

using namespace clm;

class SharedTriangleData {
private:
	struct SharedVertex {
		float x, y;
		char r, g, b, a;
	};

	map<unsigned int, SharedVertex> sharedVertexData;
	unsigned int cSharedVertex;
public:
	explicit SharedTriangleData();

	bool vertexExists(unsigned int sharedVertexId);

	unsigned int addSharedVertex(float x, float y, char r, char g, char b, char a);

	Vec2 getSharedVertexPosition(unsigned int sharedVertexId);
	char getSharedVertexR(unsigned int sharedVertexId);
	char getSharedVertexG(unsigned int sharedVertexId);
	char getSharedVertexB(unsigned int sharedVertexId);
	char getSharedVertexA(unsigned int sharedVertexId);

	void setSharedVertexPosition(unsigned int sharedVertexId, float x, float y);
};
