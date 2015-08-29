#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"

namespace clm {

class NullShader : public Shader {
private:
	GLint positionLocation;

	GLint projectionMatrixLocation;
	//GLint viewMatrixLocation;
	GLint modelViewMatrixLocation;
public:
	explicit NullShader();
	//void loadFromFiles(const char* vertexShader, const char* fragmentShader);

	void setProjectionMatrix(glm::mat4 projectionMatrix);
	//void setViewMatrix(glm::mat4 viewMatrix);
	void setModelViewMatrix(glm::mat4 modelViewMatrix);

	GLuint createVertexArrayObject(GLuint vboId, GLsizei stride, const GLvoid* positionOffset);

	void setPositionPointer(GLsizei stride, const GLvoid* offset);

	void enablePositionPointer();

	void disablePositionPointer();
};

}
