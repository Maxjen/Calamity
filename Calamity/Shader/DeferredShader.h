#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"

namespace clm {

struct BaseLight {
	glm::vec3 color;
	float ambientIntensity;
	float diffuseIntensity;

	BaseLight() :
		color(0, 0, 0),
		ambientIntensity(0.0f),
		diffuseIntensity(0.0f)
	{}
};

struct DirectionalLight : public BaseLight {
	glm::vec3 direction;

	DirectionalLight() :
		direction(0, 0, 0)
	{}
};

struct PointLight : public BaseLight {
	glm::vec3 position;
	float attConstant;
	float attLinear;
	float attExponential;

	PointLight() :
		position(0, 0, 0),
		attConstant(1.0f),
		attLinear(0),
		attExponential(0)
	{}
};

class DeferredShader : public Shader {
private:
	GLint positionLocation;

	GLint projectionMatrixLocation;
	GLint modelViewMatrixLocation;

	/*GLint positionTextureLocation;
	GLint colorTextureLocation;
	GLint normalTextureLocation;*/

	/*GLint dirLightColorLocation;
	GLint dirLightDirectionLocation;
	GLint dirLightAmbientIntensityLocation;
	GLint dirLightDiffuseIntensityLocation;*/

	struct {
		GLint color;
		GLint ambientIntensity;
		GLint diffuseIntensity;

		GLint direction;
	} dirLightLocation;

	struct {
		GLint color;
		GLint ambientIntensity;
		GLint diffuseIntensity;

		GLint position;
		GLint attConstant;
		GLint attLinear;
		GLint attExponential;
	} pointLightLocation;

	GLint eyeWorldPosLocation;
	GLint matSpecularIntensityLocation;
	GLint matSpecularPowerLocation;
	GLint screenSizeLocation;
public:
	explicit DeferredShader();
	//void loadFromFiles(const char* vertexShader, const char* fragmentShader);

	void setProjectionMatrix(glm::mat4 projectionMatrix);
	void setModelViewMatrix(glm::mat4 modelViewMatrix);

	//void setDirectionalLight(glm::vec3 color, glm::vec3 direction, float ambientIntensity, float diffuseIntensity);
	void setPointLight(PointLight pointLight);

	void setEyeWorldPos(glm::vec3 eyeWorldPos);
	void setMatSpecularIntensity(float intensity);
	void setMatSpecularPower(float power);
	void setScreenSize(float screenWidth, float screenHeight);

	GLuint createVertexArrayObject(GLuint vboId, GLsizei stride, const GLvoid* positionOffset);

	void setPositionPointer(GLsizei stride, const GLvoid* offset);
	void enablePositionPointer();
	void disablePositionPointer();
};

}
