#include "DeferredShader.h"

namespace clm {

DeferredShader::DeferredShader() {
/*}

void SpriteShader::loadFromFiles(const char* vertexShader, const char* fragmentShader) {*/
	GLuint vertexShaderId = loadShader("Calamity/Shader/pointLight.vert", GL_VERTEX_SHADER);
	GLuint fragmentShaderId = loadShader("Calamity/Shader/pointLight.frag", GL_FRAGMENT_SHADER);
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

	projectionMatrixLocation = getUniformLocation("projectionMatrix");
	modelViewMatrixLocation = getUniformLocation("modelViewMatrix");

	/*dirLightColorLocation = getUniformLocation("directionalLight.color");
	dirLightDirectionLocation = getUniformLocation("directionalLight.direction");
	dirLightAmbientIntensityLocation = getUniformLocation("directionalLight.ambientIntensity");
	dirLightDiffuseIntensityLocation = getUniformLocation("directionalLight.diffuseIntensity");*/

	/*dirLightLocation.color = getUniformLocation("directionalLight.base.color");
	dirLightLocation.ambientIntensity = getUniformLocation("directionalLight.base.ambientIntensity");
	dirLightLocation.diffuseIntensity = getUniformLocation("directionalLight.base.diffuseIntensity");
	dirLightLocation.direction = getUniformLocation("directionalLight.direction");*/

	pointLightLocation.color = getUniformLocation("pointLight.base.color");
	pointLightLocation.ambientIntensity = getUniformLocation("pointLight.base.ambientIntensity");
	pointLightLocation.diffuseIntensity = getUniformLocation("pointLight.base.diffuseIntensity");
	pointLightLocation.position = getUniformLocation("pointLight.position");
	pointLightLocation.attConstant = getUniformLocation("pointLight.attConstant");
	pointLightLocation.attLinear = getUniformLocation("pointLight.attLinear");
	pointLightLocation.attExponential = getUniformLocation("pointLight.attExponential");

	eyeWorldPosLocation = getUniformLocation("eyeWorldPos");
	matSpecularIntensityLocation = getUniformLocation("matSpecularIntensity");
	matSpecularPowerLocation = getUniformLocation("matSpecularPower");
	screenSizeLocation = getUniformLocation("screenSize");

	GLint positionTextureLocation = getUniformLocation("positionTexture");
	GLint colorTextureLocation = getUniformLocation("colorTexture");
	GLint normalTextureLocation = getUniformLocation("normalTexture");

	bind();
	glUniform1i(positionTextureLocation, 0);
	glUniform1i(colorTextureLocation, 1);
	glUniform1i(normalTextureLocation, 2);
	unbind();
}

void DeferredShader::setProjectionMatrix(glm::mat4 projectionMatrix) {
	glUniformMatrix4fv(projectionMatrixLocation, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
}

void DeferredShader::setModelViewMatrix(glm::mat4 modelViewMatrix) {
	glUniformMatrix4fv(modelViewMatrixLocation, 1, GL_FALSE, glm::value_ptr(modelViewMatrix));
}

/*void DeferredShader::setDirectionalLight(glm::vec3 color, glm::vec3 direction, float ambientIntensity, float diffuseIntensity) {
	glUniform3f(dirLightLocation.color, color.x, color.y, color.z);
	glm::vec3 directionN = glm::normalize(direction);
	glUniform3f(dirLightLocation.direction, directionN.x, directionN.y, directionN.z);
	glUniform1f(dirLightLocation.ambientIntensity, ambientIntensity);
	glUniform1f(dirLightLocation.diffuseIntensity, diffuseIntensity);
}*/

void DeferredShader::setPointLight(PointLight pointLight) {
	glUniform3f(pointLightLocation.color, pointLight.color.x, pointLight.color.y, pointLight.color.z);
	glUniform1f(pointLightLocation.ambientIntensity, pointLight.ambientIntensity);
	glUniform1f(pointLightLocation.diffuseIntensity, pointLight.diffuseIntensity);
	glUniform3f(pointLightLocation.position, pointLight.position.x, pointLight.position.y, pointLight.position.z);
	glUniform1f(pointLightLocation.attConstant, pointLight.attConstant);
	glUniform1f(pointLightLocation.attLinear, pointLight.attLinear);
	glUniform1f(pointLightLocation.attExponential, pointLight.attExponential);
}

void DeferredShader::setEyeWorldPos(glm::vec3 eyeWorldPos) {
	glUniform3f(eyeWorldPosLocation, eyeWorldPos.x, eyeWorldPos.y, eyeWorldPos.z);
}

void DeferredShader::setMatSpecularIntensity(float intensity) {
	glUniform1f(matSpecularIntensityLocation, intensity);
}

void DeferredShader::setMatSpecularPower(float power) {
	glUniform1f(matSpecularPowerLocation, power);
}

void DeferredShader::setScreenSize(float screenWidth, float screenHeight) {
	glUniform2f(screenSizeLocation, screenWidth, screenHeight);
}

GLuint DeferredShader::createVertexArrayObject(GLuint vboId, GLsizei stride, const GLvoid* positionOffset) {
	GLuint vaoId;
	glGenVertexArrays(1, &vaoId);
	glBindVertexArray(vaoId);

	glBindBuffer(GL_ARRAY_BUFFER, vboId);
	enablePositionPointer();
	setPositionPointer(stride, positionOffset);

	glBindVertexArray(0);

	return vaoId;
}

void DeferredShader::setPositionPointer(GLsizei stride, const GLvoid* offset) {
	glVertexAttribPointer(positionLocation, 3, GL_FLOAT, GL_FALSE, stride, offset);
}

void DeferredShader::enablePositionPointer() {
	glEnableVertexAttribArray(positionLocation);
}

void DeferredShader::disablePositionPointer() {
	glDisableVertexAttribArray(positionLocation);
}

}
