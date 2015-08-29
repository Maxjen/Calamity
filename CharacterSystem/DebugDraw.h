#pragma once

#include <vector>
#include <GL/glew.h>

#include "LinearMath/btIDebugDraw.h"
#include "../Calamity/Shader/Color3DShader.h"

using std::vector;

using namespace clm;

class DebugDraw : public btIDebugDraw {
private:
	struct Color3DVertex {
		float x, y, z;
		char r, g, b, a;
	};

	int debugMode;
	Color3DShader* color3DShader;

	vector<Color3DVertex> vertices;
	GLuint vertexBufferId;
	GLuint vaoId;

	vector<Color3DVertex> points;
	GLuint vertexBuffer2Id;
	GLuint vao2Id;
public:
	explicit DebugDraw(Color3DShader* color3DShader);
	~DebugDraw();

	void drawPoint(float x, float y, float z, char r, char g, char b, char a);
	void drawLine(const btVector3& from, const btVector3& to, const btVector3& color);
	void drawLine(const btVector3& from, const btVector3& to, const btVector3& fromColor, const btVector3& toColor);
	void drawContactPoint(const btVector3& PointOnB, const btVector3& normalOnB,btScalar distance, int lifeTime, const btVector3& color);
	void reportErrorWarning(const char* warningString);
	void draw3dText(const btVector3& location, const char* textString);
	void setDebugMode(int debugMode);
	int getDebugMode() const { return debugMode; }

	void render();
};
