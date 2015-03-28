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
	const float heightMultiplier = 25;
	float height = texel.r*heightMultiplier;
	
	vec4 texel1 = texture(texture0, vec2( (translatedPosition.x-1)/4096.0, translatedPosition.z/4096.0));
	vec4 texel2 = texture(texture0, vec2(translatedPosition.x/4096.0, (translatedPosition.z-1)/4096.0));
	vec4 texel3 = texture(texture0, vec2( (translatedPosition.x+1)/4096.0, translatedPosition.z/4096.0));
	vec4 texel4 = texture(texture0, vec2(translatedPosition.x/4096.0, (translatedPosition.z+1)/4096.0));

	vec3 v1 = vec3(-1, height - texel1.r*heightMultiplier, 0);
	vec3 v2 = vec3(0, height - texel2.r*heightMultiplier, -1);
	vec3 v3 = vec3(1, height - texel3.r*heightMultiplier, 0);
	vec3 v4 = vec3(0, height - texel4.r*heightMultiplier, 1);
	
	vec3 v12 = cross(v2,v1);
	vec3 v23 = cross(v3,v2);
	vec3 v34 = cross(v4,v3);
	vec3 v41 = cross(v1,v4);
		
	vec4 realPosition = vec4(translatedPosition.x, height, translatedPosition.z, 1.0);	
	gl_Position = projectionMatrix * viewMatrix * realPosition;
	
	pass_Texture = vec2( (translatedPosition.x)/10.0, (translatedPosition.z)/10.0);
	pass_Normal = normalize(v12 + v23 + v34 + v41);
	pass_BlendTextureCoordinates = vec2( (translatedPosition.x)/4096.0, (translatedPosition.z)/4096.0);
}