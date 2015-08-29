/* Vertex shader */

#version 130

in vec2 in_Position;
in vec2 in_TexCoord;
in vec4 in_Color;
in vec2 in_TexOrigin;
in vec2 in_TexDimensions;
out vec2 ex_TexCoord;
out vec4 ex_Color;
out vec2 ex_TexOrigin;
out vec2 ex_TexDimensions;

uniform mat4 projectionMatrix;
uniform mat4 modelViewMatrix;
 
void main(void)
{
	gl_Position = projectionMatrix * modelViewMatrix * vec4(in_Position.x, in_Position.y, 0.0, 1.0);
	ex_TexCoord = in_TexCoord;
	//ex_Color = vec4(in_TexOrigin.x, in_TexOrigin.x, in_TexOrigin.x, 1.0);
	ex_Color = in_Color;
	//ex_Color = vec4(in_TexCoord.y, 0, 0, 1);
	ex_TexOrigin = in_TexOrigin;
	ex_TexDimensions = in_TexDimensions;
}
