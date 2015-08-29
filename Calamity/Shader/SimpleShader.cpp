#include "SimpleShader.h"

namespace clm {

SimpleShader::SimpleShader() {
	GLuint vertexShaderId = loadShader("Calamity/Shader/simple.vert", GL_VERTEX_SHADER);
	GLuint fragmentShaderId = loadShader("Calamity/Shader/simple.frag", GL_FRAGMENT_SHADER);

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
	}

	glDetachShader(programId, vertexShaderId);
	glDetachShader(programId, fragmentShaderId);

	glDeleteShader(vertexShaderId);
	glDeleteShader(fragmentShaderId);

	positionLocation = getAttribLocation("in_Position");

	//texCoordLocation = getAttribLocation("in_TexCoord");

	colorLocation = getAttribLocation("in_Color");

	projectionMatrixLocation = getUniformLocation("projectionMatrix");

	/*modelViewMatrixLocation = getUniformLocation("modelViewMatrix");*/

	/*useTextureLocation = getUniformLocation("useTexture");*/

	/*GLint textureLocation = getUniformLocation("tex");

	glUniform1i(textureLocation, 0);*/
}

void SimpleShader::setProjectionMatrix(glm::mat4 projectionMatrix) {
	glUniformMatrix4fv(projectionMatrixLocation, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
}

/*void SimpleShader::setModelViewMatrix(glm::mat4 modelViewMatrix) {
	glUniformMatrix4fv(modelViewMatrixLocation, 1, GL_FALSE, glm::value_ptr(modelViewMatrix));
}*/

/*void SimpleShader::setUseTexture(int useTexture) {
	glUniform1i(useTextureLocation, useTexture);
}*/

GLuint SimpleShader::createVertexArrayObject(GLuint vboId, GLsizei stride, const GLvoid* positionOffset, const GLvoid* colorOffset) {
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

void SimpleShader::setPositionPointer(GLsizei stride, const GLvoid* offset) {
	glVertexAttribPointer(positionLocation, 3, GL_FLOAT, GL_FALSE, stride, offset);
}

/*void SimpleShader::setTexCoordPointer(GLsizei stride, const GLvoid* offset) {
	glVertexAttribPointer(texCoordLocation, 2, GL_FLOAT, GL_FALSE, stride, offset);
}*/

void SimpleShader::setColorPointer(GLsizei stride, const GLvoid* offset) {
	glVertexAttribPointer(colorLocation, 4, GL_UNSIGNED_BYTE, GL_TRUE, stride, offset);
}

void SimpleShader::enablePositionPointer() {
	glEnableVertexAttribArray(positionLocation);
}

/*void SimpleShader::enableTexCoordPointer() {
	glEnableVertexAttribArray(texCoordLocation);
}*/

void SimpleShader::enableColorPointer() {
	glEnableVertexAttribArray(colorLocation);
}

void SimpleShader::disablePositionPointer() {
	glDisableVertexAttribArray(positionLocation);
}

/*void SimpleShader::disableTexCoordPointer() {
	glDisableVertexAttribArray(texCoordLocation);
}*/

void SimpleShader::disableColorPointer() {
	glDisableVertexAttribArray(colorLocation);
}


}
