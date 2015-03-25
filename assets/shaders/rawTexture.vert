#version 330

in vec3 in_Position;
in vec2 in_TextureCoord;

uniform mat4 projectionMatrix;
uniform mat4 modelMatrix;
uniform mat4 viewMatrix;

out vec2 pass_Texture;

void main(void) {
	gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(in_Position, 1.0);
	pass_Texture = in_TextureCoord;
}