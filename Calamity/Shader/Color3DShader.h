#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"

namespace clm {

class Color3DShader : public Shader {
private:
	GLint positionLocation;
	GLint colorLocation;
	GLint projectionMatrixLocation;
	GLint modelViewMatrixLocation;
public:
	explicit Color3DShader();
	//void loadFromFiles(const char* vertexShader, const char* fragmentShader);

	void setProjectionMatrix(glm::mat4 projectionMatrix);
	void setModelViewMatrix(glm::mat4 modelViewMatrix);

	GLuint createVertexArrayObject(GLuint vboId, GLsizei stride, const GLvoid* positionOffset, const GLvoid* colorOffset);

	void setPositionPointer(GLsizei stride, const GLvoid* offset);
	void setColorPointer(GLsizei stride, const GLvoid* offset);

	void enablePositionPointer();
	void enableColorPointer();

	void disablePositionPointer();
	void disableColorPointer();
};

}
