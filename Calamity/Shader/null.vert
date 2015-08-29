/* Vertex shader */

#version 130

in vec3 in_Position;

uniform mat4 projectionMatrix;
//uniform mat4 viewMatrix;
uniform mat4 modelViewMatrix;
 
void main(void) {
	gl_Position = projectionMatrix * modelViewMatrix * vec4(in_Position, 1.0);
}
