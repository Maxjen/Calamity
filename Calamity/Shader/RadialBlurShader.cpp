#include "RadialBlurShader.h"

namespace clm {

RadialBlurShader::RadialBlurShader(int screenWidth, int screenHeight) {
	this->screenWidth = screenWidth;
	this->screenHeight = screenHeight;

	GLuint vertexShaderId = loadShader("Calamity/Shader/radialBlur.vert", GL_VERTEX_SHADER);
	GLuint fragmentShaderId = loadShader("Calamity/Shader/radialBlur.frag", GL_FRAGMENT_SHADER);

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
	texCoordLocation = getAttribLocation("in_TexCoord");
	colorLocation = getAttribLocation("in_Color");

	projectionMatrixLocation = getUniformLocation("projectionMatrix");
	modelViewMatrixLocation = getUniformLocation("modelViewMatrix");

	//useTextureLocation = getUniformLocation("useTexture");

	GLint textureLocation = getUniformLocation("tex");

	/*vec2 radialSize;	// texel size
	float radialBlur;	// blur factor
	float radialBright;	// bright factor
	vec2 radialOrigin;*/

	GLint radialSizeLocation = getUniformLocation("radialSize");
	GLint radialBlurLocation = getUniformLocation("radialBlur");
	GLint radialBrightLocation = getUniformLocation("radialBright");
	GLint radialOriginLocation = getUniformLocation("radialOrigin");

	// for moon
	/*bind();
	glUniform1i(textureLocation, 0);
	glUniform2f(radialSizeLocation, 1.0f/800, 1.0f/600);
	//glUniform1f(radialBlurLocation, 0.99f);
	glUniform1f(radialBlurLocation, 0.8f);
	//glUniform1f(radialBrightLocation, 0.1f);
	glUniform1f(radialBrightLocation, 0.06f);
	//glUniform2f(radialOriginLocation, 0.45f, 0.8f);
	glUniform2f(radialOriginLocation, 0.45f, 0.7f);
	unbind();*/

	// for sun
	bind();
	glUniform1i(textureLocation, 0);
	glUniform2f(radialSizeLocation, 1.0f/screenWidth, 1.0f/screenHeight);
	//glUniform2f(radialSizeLocation, 1.0f/800, 1.0f/600);
	//glUniform1f(radialBlurLocation, 0.99f);
	glUniform1f(radialBlurLocation, 0.8f);
	//glUniform1f(radialBrightLocation, 0.1f);
	glUniform1f(radialBrightLocation, 0.05f);
	//glUniform2f(radialOriginLocation, 0.45f, 0.8f);
	glUniform2f(radialOriginLocation, 0.45f, 0.7f);
	unbind();
}

void RadialBlurShader::setProjectionMatrix(glm::mat4 projectionMatrix) {
	glUniformMatrix4fv(projectionMatrixLocation, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
}

void RadialBlurShader::setModelViewMatrix(glm::mat4 modelViewMatrix) {
	glUniformMatrix4fv(modelViewMatrixLocation, 1, GL_FALSE, glm::value_ptr(modelViewMatrix));
}

void RadialBlurShader::setBlurOrigin(float x, float y) {
	GLint radialOriginLocation = glGetUniformLocation(programId, "radialOrigin");
	if (radialOriginLocation == -1) {
		printf( "%s is not a valid glsl program variable!\n", "radialOrigin" );
	}

	bind();
	glUniform2f(radialOriginLocation, x / screenWidth, y / screenHeight);
	unbind();
}

/*void RadialBlurShader::setUseTexture(int useTexture) {
	glUniform1i(useTextureLocation, useTexture);
}*/

GLuint RadialBlurShader::createVertexArrayObject(GLuint vboId, GLsizei stride, const GLvoid* positionOffset, const GLvoid* texCoordOffset, const GLvoid* colorOffset) {
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

void RadialBlurShader::setPositionPointer(GLsizei stride, const GLvoid* offset) {
	glVertexAttribPointer(positionLocation, 2, GL_FLOAT, GL_FALSE, stride, offset);
}

void RadialBlurShader::setTexCoordPointer(GLsizei stride, const GLvoid* offset) {
	glVertexAttribPointer(texCoordLocation, 2, GL_FLOAT, GL_FALSE, stride, offset);
}

void RadialBlurShader::setColorPointer(GLsizei stride, const GLvoid* offset) {
	glVertexAttribPointer(colorLocation, 4, GL_UNSIGNED_BYTE, GL_TRUE, stride, offset);
}

void RadialBlurShader::enablePositionPointer() {
	glEnableVertexAttribArray(positionLocation);
}

void RadialBlurShader::enableTexCoordPointer() {
	glEnableVertexAttribArray(texCoordLocation);
}

void RadialBlurShader::enableColorPointer() {
	glEnableVertexAttribArray(colorLocation);
}

void RadialBlurShader::disablePositionPointer() {
	glDisableVertexAttribArray(positionLocation);
}

void RadialBlurShader::disableTexCoordPointer() {
	glDisableVertexAttribArray(texCoordLocation);
}

void RadialBlurShader::disableColorPointer() {
	glDisableVertexAttribArray(colorLocation);
}


}
