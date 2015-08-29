#include "Color3DShader.h"

namespace clm {

Color3DShader::Color3DShader() {
/*}

void SpriteShader::loadFromFiles(const char* vertexShader, const char* fragmentShader) {*/
	GLuint vertexShaderId = loadShader("Calamity/Shader/color3D.vert", GL_VERTEX_SHADER);
	GLuint fragmentShaderId = loadShader("Calamity/Shader/color3D.frag", GL_FRAGMENT_SHADER);
	/*GLuint vertexShaderId = loadShader(vertexShader, GL_VERTEX_SHADER);
	GLuint fragmentShaderId = loadShader(fragmentShader, GL_FRAGMENT_SHADER);*/

	programId = glCreateProgram();
	glAttachShader(programId, vertexShaderId);
	glAttachShader(programId, fragmentShaderId);
	glLinkProgram(programId);

	// Check for errors
	GLint programSuccess = GL_TRUE;
	glGetProgramiv(programId, GL_LINK_STATUS, &programSuccess);
	if (programSuccess != GL_TRUE) {
		printf("Error linking program %d!\n", programId);
		printProgramLog(programId);
		glDeleteProgram(programId);
		programId = 0;

		glDetachShader(programId, vertexShaderId);
		glDetachShader(programId, fragmentShaderId);

		glDeleteShader(vertexShaderId);
		glDeleteShader(fragmentShaderId);

		return;
	}

	glDetachShader(programId, vertexShaderId);
	glDetachShader(programId, fragmentShaderId);

	glDeleteShader(vertexShaderId);
	glDeleteShader(fragmentShaderId);

	positionLocation = getAttribLocation("in_Position");
	colorLocation = getAttribLocation("in_Color");

	projectionMatrixLocation = getUniformLocation("projectionMatrix");
	modelViewMatrixLocation = getUniformLocation("modelViewMatrix");
}

void Color3DShader::setProjectionMatrix(glm::mat4 projectionMatrix) {
	glUniformMatrix4fv(projectionMatrixLocation, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
}

void Color3DShader::setModelViewMatrix(glm::mat4 modelViewMatrix) {
	glUniformMatrix4fv(modelViewMatrixLocation, 1, GL_FALSE, glm::value_ptr(modelViewMatrix));
}

GLuint Color3DShader::createVertexArrayObject(GLuint vboId, GLsizei stride, const GLvoid* positionOffset, const GLvoid* colorOffset) {
	GLuint vaoId;
	glGenVertexArrays(1, &vaoId);
	glBindVertexArray(vaoId);

	glBindBuffer(GL_ARRAY_BUFFER, vboId);
	enablePositionPointer();
	setPositionPointer(stride, positionOffset);
	enableColorPointer();
	setColorPointer(stride, colorOffset);

	glBindVertexArray(0);

	return vaoId;
}

void Color3DShader::setPositionPointer(GLsizei stride, const GLvoid* offset) {
	glVertexAttribPointer(positionLocation, 3, GL_FLOAT, GL_FALSE, stride, offset);
}

void Color3DShader::setColorPointer(GLsizei stride, const GLvoid* offset) {
	glVertexAttribPointer(colorLocation, 4, GL_UNSIGNED_BYTE, GL_TRUE, stride, offset);
}

void Color3DShader::enablePositionPointer() {
	glEnableVertexAttribArray(positionLocation);
}

void Color3DShader::enableColorPointer() {
	glEnableVertexAttribArray(colorLocation);
}

void Color3DShader::disablePositionPointer() {
	glDisableVertexAttribArray(positionLocation);
}

void Color3DShader::disableColorPointer() {
	glDisableVertexAttribArray(colorLocation);
}


}
