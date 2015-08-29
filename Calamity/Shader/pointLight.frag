/* Fragment shader */

#version 130

out vec4 out_Color;

struct BaseLight {
	vec3 color;
	float ambientIntensity;
	float diffuseIntensity;
};

struct DirectionalLight {
	BaseLight base;
	//vec3 color;
	vec3 direction;
	//float ambientIntensity;
	//float diffuseIntensity;
};

struct PointLight {
	BaseLight base;
	vec3 position;
	float attConstant;
	float attLinear;
	float attExponential;
};

uniform sampler2D positionTexture;
uniform sampler2D colorTexture;
uniform sampler2D normalTexture;

uniform DirectionalLight directionalLight;
uniform PointLight pointLight;

uniform vec3 eyeWorldPos;
uniform float matSpecularIntensity;
uniform float matSpecularPower;

uniform vec2 screenSize;

vec3 calcLightInternal(BaseLight light, vec3 lightDirection, vec3 worldPos, vec3 normal) {
	vec3 ambientColor = light.color * light.ambientIntensity;
	float diffuseFactor = dot(normal, -lightDirection);

	vec3 diffuseColor = vec3(0, 0, 0);
	vec3 specularColor = vec3(0, 0, 0);
	if (diffuseFactor > 0.0) {
		diffuseColor = light.color * light.diffuseIntensity * diffuseFactor;

		vec3 vertexToEye = normalize(eyeWorldPos - worldPos);
		vec3 lightReflect = normalize(reflect(lightDirection, normal));
		float specularFactor = dot(vertexToEye, lightReflect);
		if (specularFactor > 0.0) {
			specularFactor = pow(specularFactor, matSpecularPower);
		//}
		//if (specularFactor > 0.0) {
			specularColor = light.color * matSpecularIntensity * specularFactor;
		}
	}
	return ambientColor + diffuseColor + specularColor;
}

vec3 calcDirectionalLight(vec3 worldPos, vec3 normal) {
	return calcLightInternal(directionalLight.base, directionalLight.direction, worldPos, normal);
}

vec3 calcPointLight(vec3 worldPos, vec3 normal) {
	vec3 lightDirection = worldPos - pointLight.position;
	float distance = length(lightDirection);
	lightDirection = normalize(lightDirection);

	vec3 color = calcLightInternal(pointLight.base, lightDirection, worldPos, normal);
	float attenuation = pointLight.attConstant +
			pointLight.attLinear * distance +
			pointLight.attExponential * distance * distance;

	return color / attenuation;
}

void main() {
	vec2 texCoord = gl_FragCoord.xy / screenSize;
	vec3 worldPos = texture(positionTexture, texCoord).xyz;
	vec3 color = texture(colorTexture, texCoord).xyz;
	vec3 normal = texture(normalTexture, texCoord).xyz;
	normal = normalize(normal);

	if (normal == vec3(1.0, 1.0, 1.0))
		out_Color = vec4(0, 0, 0, 0);
	else
		out_Color = vec4(color * calcPointLight(worldPos, normal), 1.0);
}
