#version 330 core

in vec2 in_Position;

uniform lowp sampler2D texture0;
uniform mat4 projectionMatrix;
uniform mat4 modelMatrix;
uniform mat4 viewMatrix;

out vec2 pass_Texture;
out vec3 pass_Normal;
out vec2 pass_BlendTextureCoordinates;

void main(void) {
	vec4 translatedPosition = modelMatrix * vec4(in_Position.x, 0.0, in_Position.y, 1.0);
	vec4 texel = texture(texture0, vec2(translatedPosition.x/4096.0, translatedPosition.z/4096.0));
	vec4 realPosition = vec4(translatedPosition.x, texel.r*75.0, translatedPosition.z, 1.0);	
	gl_Position = projectionMatrix * viewMatrix * realPosition;
	
	pass_Texture = vec2( (translatedPosition.x)/10.0, (translatedPosition.z)/10.0);
	pass_Normal = vec3(0,1,0);
	pass_BlendTextureCoordinates = vec2( (translatedPosition.x)/4096.0, (translatedPosition.z)/4096.0);
}