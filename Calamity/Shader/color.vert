/* Vertex shader */

#version 130

in vec2 in_Position;
in vec4 in_Color;
out vec4 ex_Color;

uniform mat4 projectionMatrix;
uniform mat4 modelViewMatrix;
 
void main(void)
{
	gl_Position = projectionMatrix * modelViewMatrix * vec4(in_Position.x, in_Position.y, 0.0, 1.0);
	ex_Color = in_Color;
}
