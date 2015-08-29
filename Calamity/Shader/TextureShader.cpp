#include "TextureShader.h"

namespace clm {

TextureShader::TextureShader() {
/*}

void SpriteShader::loadFromFiles(const char* vertexShader, const char* fragmentShader) {*/
	GLuint vertexShaderId = loadShader("Calamity/Shader/texture.vert", GL_VERTEX_SHADER);
	GLuint fragmentShaderId = loadShader("Calamity/Shader/texture.frag", GL_FRAGMENT_SHADER);
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
	texOriginLocation = getAttribLocation("in_TexOrigin");
	texDimensionsLocation = getAttribLocation("in_TexDimensions");

	projectionMatrixLocation = getUniformLocation("projectionMatrix");
	modelViewMatrixLocation = getUniformLocation("modelViewMatrix");

	//useTextureLocation = getUniformLocation("useTexture");

	renderBlackLocation = getUniformLocation("bRenderBlack");
	anaglyphLocation = getUniformLocation("bAnaglyph");

	GLint textureLocation = getUniformLocation("tex");

	bind();
	glUniform1i(textureLocation, 0);
	glUniform1i(renderBlackLocation, 0);
	glUniform1i(anaglyphLocation, false);
	unbind();
}

void TextureShader::setProjectionMatrix(glm::mat4 projectionMatrix) {
	glUniformMatrix4fv(projectionMatrixLocation, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
}

void TextureShader::setModelViewMatrix(glm::mat4 modelViewMatrix) {
	glUniformMatrix4fv(modelViewMatrixLocation, 1, GL_FALSE, glm::value_ptr(modelViewMatrix));
}

/*void TextureShader::setUseTexture(int useTexture) {
	glUniform1i(useTextureLocation, useTexture);
}*/

void TextureShader::setRenderBlack(bool renderBlack) {
	if (renderBlack)
		glUniform1i(renderBlackLocation, 1);
	else
		glUniform1i(renderBlackLocation, 0);
}

void TextureShader::setAnaglyph(bool bAnaglyph) {
	glUniform1i(anaglyphLocation, bAnaglyph);
}

GLuint TextureShader::createVertexArrayObject(GLuint vboId, GLsizei stride, const GLvoid* positionOffset, const GLvoid* texCoordOffset, const GLvoid* colorOffset, const GLvoid* texOriginOffset, const GLvoid* texDimensionsOffset) {
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
	enableTexOriginPointer();
	setTexOriginPointer(stride, texOriginOffset);
	enableTexDimensionsPointer();
	setTexDimensionsPointer(stride, texDimensionsOffset);

	glBindVertexArray(0);

	return vaoId;
}

void TextureShader::setPositionPointer(GLsizei stride, const GLvoid* offset) {
	glVertexAttribPointer(positionLocation, 2, GL_FLOAT, GL_FALSE, stride, offset);
}

void TextureShader::setTexCoordPointer(GLsizei stride, const GLvoid* offset) {
	glVertexAttribPointer(texCoordLocation, 2, GL_FLOAT, GL_FALSE, stride, offset);
}

void TextureShader::setColorPointer(GLsizei stride, const GLvoid* offset) {
	glVertexAttribPointer(colorLocation, 4, GL_UNSIGNED_BYTE, GL_TRUE, stride, offset);
}

void TextureShader::setTexOriginPointer(GLsizei stride, const GLvoid* offset) {
	glVertexAttribPointer(texOriginLocation, 2, GL_FLOAT, GL_FALSE, stride, offset);
}

void TextureShader::setTexDimensionsPointer(GLsizei stride, const GLvoid* offset) {
	glVertexAttribPointer(texDimensionsLocation, 2, GL_FLOAT, GL_FALSE, stride, offset);
}

void TextureShader::enablePositionPointer() {
	glEnableVertexAttribArray(positionLocation);
}

void TextureShader::enableTexCoordPointer() {
	glEnableVertexAttribArray(texCoordLocation);
}

void TextureShader::enableColorPointer() {
	glEnableVertexAttribArray(colorLocation);
}

void TextureShader::enableTexOriginPointer() {
	glEnableVertexAttribArray(texOriginLocation);
}

void TextureShader::enableTexDimensionsPointer() {
	glEnableVertexAttribArray(texDimensionsLocation);
}

void TextureShader::disablePositionPointer() {
	glDisableVertexAttribArray(positionLocation);
}

void TextureShader::disableTexCoordPointer() {
	glDisableVertexAttribArray(texCoordLocation);
}

void TextureShader::disableColorPointer() {
	glDisableVertexAttribArray(colorLocation);
}

void TextureShader::disableTexOriginPointer() {
	glDisableVertexAttribArray(texOriginLocation);
}

void TextureShader::disableTexDimensionsPointer() {
	glDisableVertexAttribArray(texDimensionsLocation);
}

}
