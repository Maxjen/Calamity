#include "FontShader.h"

namespace clm {

FontShader::FontShader() {
/*}

void SpriteShader::loadFromFiles(const char* vertexShader, const char* fragmentShader) {*/
	GLuint vertexShaderId = loadShader("Calamity/Shader/font.vert", GL_VERTEX_SHADER);
	GLuint fragmentShaderId = loadShader("Calamity/Shader/font.frag", GL_FRAGMENT_SHADER);
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
	texCoordLocation = getAttribLocation("in_TexCoord");
	colorLocation = getAttribLocation("in_Color");

	projectionMatrixLocation = getUniformLocation("projectionMatrix");
	modelViewMatrixLocation = getUniformLocation("modelViewMatrix");

	//useTextureLocation = getUniformLocation("useTexture");

	renderBlackLocation = getUniformLocation("renderBlack");

	GLint textureLocation = getUniformLocation("tex");

	bind();
	glUniform1i(textureLocation, 0);
	glUniform1i(renderBlackLocation, 0);
	unbind();
}

void FontShader::setProjectionMatrix(glm::mat4 projectionMatrix) {
	glUniformMatrix4fv(projectionMatrixLocation, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
}

void FontShader::setModelViewMatrix(glm::mat4 modelViewMatrix) {
	glUniformMatrix4fv(modelViewMatrixLocation, 1, GL_FALSE, glm::value_ptr(modelViewMatrix));
}

/*void SpriteShader::setUseTexture(int useTexture) {
	glUniform1i(useTextureLocation, useTexture);
}*/

void FontShader::setRenderBlack(bool renderBlack) {
	if (renderBlack)
		glUniform1i(renderBlackLocation, 1);
	else
		glUniform1i(renderBlackLocation, 0);
}

GLuint FontShader::createVertexArrayObject(GLuint vboId, GLsizei stride, const GLvoid* positionOffset, const GLvoid* texCoordOffset, const GLvoid* colorOffset) {
	GLuint vaoId;
	glGenVertexArrays(1, &vaoId);
	glBindVertexArray(vaoId);

	glBindBuffer(GL_ARRAY_BUFFER, vboId);
	enablePositionPointer();
	setPositionPointer(stride, positionOffset);
	enableTexCoordPointer();
	setTexCoordPointer(stride, texCoordOffset);
	enableColorPointer();
	setColorPointer(stride, colorOffset);

	glBindVertexArray(0);

	return vaoId;
}

void FontShader::setPositionPointer(GLsizei stride, const GLvoid* offset) {
	glVertexAttribPointer(positionLocation, 2, GL_FLOAT, GL_FALSE, stride, offset);
}

void FontShader::setTexCoordPointer(GLsizei stride, const GLvoid* offset) {
	glVertexAttribPointer(texCoordLocation, 2, GL_FLOAT, GL_FALSE, stride, offset);
}

void FontShader::setColorPointer(GLsizei stride, const GLvoid* offset) {
	glVertexAttribPointer(colorLocation, 4, GL_UNSIGNED_BYTE, GL_TRUE, stride, offset);
}

void FontShader::enablePositionPointer() {
	glEnableVertexAttribArray(positionLocation);
}

void FontShader::enableTexCoordPointer() {
	glEnableVertexAttribArray(texCoordLocation);
}

void FontShader::enableColorPointer() {
	glEnableVertexAttribArray(colorLocation);
}

void FontShader::disablePositionPointer() {
	glDisableVertexAttribArray(positionLocation);
}

void FontShader::disableTexCoordPointer() {
	glDisableVertexAttribArray(texCoordLocation);
}

void FontShader::disableColorPointer() {
	glDisableVertexAttribArray(colorLocation);
}


}
