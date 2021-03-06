/* Vertex shader */

#version 130

in vec3 in_Position;
//in vec2 in_TexCoord;
in vec4 in_Color;
//out vec2 ex_TexCoord;
out vec4 ex_Color;

uniform mat4 projectionMatrix;
//uniform mat4 modelViewMatrix;
 
void main(void)
{
	gl_Position = projectionMatrix * vec4(in_Position.x, in_Position.y, in_Position.z, 1.0);

	ex_Color = in_Color;
}
