#version 130

in vec3 in_Position;
in vec2 in_TextureCoord;
in vec3 in_Normal;

uniform mat4 projectionMatrix;
uniform mat4 modelMatrix;
out vec3 pass_Position;
out vec2 pass_Texture;
out vec3 pass_Normal;

void main(void) {
	gl_Position = projectionMatrix * modelMatrix * vec4(in_Position, 1.0);
	pass_Position = in_Position;
	pass_Texture = in_TextureCoord;
	pass_Normal = in_Normal;
}