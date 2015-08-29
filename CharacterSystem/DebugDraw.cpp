#include "DebugDraw.h"

DebugDraw::DebugDraw(Color3DShader* color3DShader) :
	debugMode(0),
	color3DShader(color3DShader),
	vertices(),
	vertexBufferId(0),
	vaoId(0),
	points(),
	vertexBuffer2Id(0),
	vao2Id(0)
{
	glGenBuffers(1, &vertexBufferId);
	vaoId = color3DShader->createVertexArrayObject(vertexBufferId, sizeof(Color3DVertex), 0, (GLvoid*)(3 * sizeof(float)));

	glGenBuffers(1, &vertexBuffer2Id);
	vao2Id = color3DShader->createVertexArrayObject(vertexBuffer2Id, sizeof(Color3DVertex), 0, (GLvoid*)(3 * sizeof(float)));
}

DebugDraw::~DebugDraw() {
	glDeleteVertexArrays(1, &vaoId);
	glDeleteBuffers(1, &vertexBufferId);

	glDeleteVertexArrays(1, &vao2Id);
	glDeleteBuffers(1, &vertexBuffer2Id);
}

void DebugDraw::drawPoint(float x, float y, float z, char r, char g, char b, char a) {
	//printf("%f %f %f\n", x, y, z);
	Color3DVertex p;
	p.x = x;
	p.y = y;
	p.z = z;
	p.r = r;
	p.g = g;
	p.b = b;
	p.a = a;
	points.push_back(p);
}

void DebugDraw::drawLine(const btVector3& from, const btVector3& to, const btVector3& color) {
	//if (debugMode > 0) {
		Color3DVertex v1, v2;

		v1.x = from.getX();
		v1.y = from.getY();
		v1.z = from.getZ();
		v1.r = color.getX() * 255;
		v1.g = color.getY() * 255;
		v1.b = color.getZ() * 255;
		v1.a = 255;

		v2.x = to.getX();
		v2.y = to.getY();
		v2.z = to.getZ();
		v2.r = color.getX() * 255;
		v2.g = color.getY() * 255;
		v2.b = color.getZ() * 255;
		v2.a = 255;

		vertices.push_back(v1);
		vertices.push_back(v2);
	//}
}

void DebugDraw::drawLine(const btVector3& from, const btVector3& to, const btVector3& fromColor, const btVector3& toColor) {
	//if (debugMode > 0) {
		Color3DVertex v1, v2;

		v1.x = from.getX();
		v1.y = from.getY();
		v1.z = from.getZ();
		v1.r = fromColor.getX() * 255;
		v1.g = fromColor.getY() * 255;
		v1.b = fromColor.getZ() * 255;
		v1.a = 255;

		v2.x = to.getX();
		v2.y = to.getY();
		v2.z = to.getZ();
		v2.r = toColor.getX() * 255;
		v2.g = toColor.getY() * 255;
		v2.b = toColor.getZ() * 255;
		v2.a = 255;

		vertices.push_back(v1);
		vertices.push_back(v2);
	//}
}

void DebugDraw::drawContactPoint(const btVector3& PointOnB, const btVector3& normalOnB, btScalar distance, int lifeTime, const btVector3& color) {

}

void DebugDraw::reportErrorWarning(const char* warningString) {
	printf(warningString);
}

void DebugDraw::draw3dText(const btVector3& location, const char* textString) {

}

void DebugDraw::setDebugMode(int debugMode) {
	this->debugMode = debugMode;
}

void DebugDraw::render() {
	glLineWidth(1);
	glPointSize(3.0f);


	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferId);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Color3DVertex), &(vertices[0]), GL_DYNAMIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer2Id);
	glBufferData(GL_ARRAY_BUFFER, points.size() * sizeof(Color3DVertex), &(points[0]), GL_DYNAMIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);


	color3DShader->bind();
	glBindVertexArray(vaoId);
	glDrawArrays(GL_LINES, 0, vertices.size());
	glBindVertexArray(vao2Id);
	glDrawArrays(GL_POINTS, 0, points.size());
	glBindVertexArray(0);
	color3DShader->unbind();


	vertices.clear();
	points.clear();
}
