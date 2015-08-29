/* Vertex shader */

#version 130

in vec3 in_Position;
in vec3 in_Normal;
in vec2 in_TexCoord;
in ivec4 in_BoneIds;
in vec4 in_BoneWeights;
//in vec4 in_Color;
out vec3 ex_Normal;
out vec2 ex_TexCoord;
out vec3 ex_WorldPos;
//out vec4 ex_Color;

uniform mat4 projectionMatrix;
//uniform mat4 viewMatrix;
uniform mat4 modelViewMatrix;
uniform mat4 modelMatrix;
uniform mat4 modelInverseTranspose;

uniform mat4 bones[100];
 
void main(void) {
	mat4 boneTransform = bones[in_BoneIds[0]] * in_BoneWeights[0] +
			bones[in_BoneIds[1]] * in_BoneWeights[1] +
			bones[in_BoneIds[2]] * in_BoneWeights[2] +
			bones[in_BoneIds[3]] * in_BoneWeights[3];
	//boneTransform = mat4(vec4(1,0,0,0), vec4(0,1,0,0), vec4(0,0,1,0), vec4(0,0,0,1));
	//boneTransform = bones[0];

	gl_Position = projectionMatrix * modelViewMatrix * boneTransform * vec4(in_Position, 1.0);
	//ex_Normal = (modelInverseTranspose * vec4(in_Normal, 0)).xyz;
	ex_Normal = (modelInverseTranspose * boneTransform * vec4(in_Normal, 0)).xyz;
	ex_TexCoord = in_TexCoord;
	/*vec4 wp = modelMatrix * boneTransform * vec4(in_Position, 1.0);
	wp = wp / wp.w;
	ex_WorldPos = wp.xyz;*/
	ex_WorldPos = (modelMatrix * boneTransform * vec4(in_Position, 1.0)).xyz;

	/*float w = 0;
	ex_Normal = ex_Normal * 0.00001;
	ex_Normal = vec3(w, w, w);
	float weightSum = in_BoneWeights[0] + in_BoneWeights[1] + in_BoneWeights[2] + in_BoneWeights[3];
	if (weightSum > 0.9 && weightSum < 1.1)
		ex_Normal.x = 1;*/
}
