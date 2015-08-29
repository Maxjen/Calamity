#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"

namespace clm {

class ColorShader : public Shader {
private:
	GLint positionLocation;
	GLint colorLocation;
	GLint projectionMatrixLocation;
	GLint modelViewMatrixLocation;
	GLint anaglyphLocation;
public:
	explicit ColorShader();
	//void loadFromFiles(const char* vertexShader, const char* fragmentShader);

	void setProjectionMatrix(glm::mat4 projectionMatrix);
	void setModelViewMatrix(glm::mat4 modelViewMatrix);

	void setAnaglyph(bool bAnaglyph);

	GLuint createVertexArrayObject(GLuint vboId, GLsizei stride, const GLvoid* positionOffset, const GLvoid* colorOffset);

	void setPositionPointer(GLsizei stride, const GLvoid* offset);
	void setColorPointer(GLsizei stride, const GLvoid* offset);

	void enablePositionPointer();
	void enableColorPointer();

	void disablePositionPointer();
	void disableColorPointer();
};

}
