/* Vertex shader */

#version 130

in vec3 in_Position;
in vec3 in_Normal;
in vec2 in_TexCoord;
//in vec4 in_Color;
out vec3 ex_Normal;
out vec2 ex_TexCoord;
out vec3 ex_WorldPos;
//out vec4 ex_Color;

uniform mat4 projectionMatrix;
uniform mat4 modelViewMatrix;
uniform mat4 modelMatrix;
uniform mat4 modelInverseTranspose;
//uniform int renderBlack;
uniform bool bInvertNormals;
 
void main(void)
{
	gl_Position = projectionMatrix * modelViewMatrix * vec4(in_Position.x, in_Position.y, in_Position.z, 1.0);
	if (bInvertNormals)
		ex_Normal = (modelInverseTranspose * vec4(-in_Normal, 0)).xyz;
	else
		ex_Normal = (modelInverseTranspose * vec4(in_Normal, 0)).xyz;
	ex_TexCoord = in_TexCoord;
	ex_WorldPos = (modelMatrix * vec4(in_Position, 1.0)).xyz;
	/*if (renderBlack == 0)
		ex_Color = in_Color;
	else
		ex_Color = vec4(0.0, 0.0, 0.0, in_Color.a);*/
}
