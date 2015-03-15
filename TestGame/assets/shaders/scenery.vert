#version 130

in vec3 in_Position;
uniform mat4 projectionMatrix;
uniform mat4 modelMatrix;

void main(void) {
	gl_Position = projectionMatrix * modelMatrix * vec4(in_Position, 1.0);
}