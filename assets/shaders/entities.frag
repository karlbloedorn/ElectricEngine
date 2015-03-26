#version 330 core

uniform sampler2D texture0;
in vec2 pass_Texture;
in vec3 pass_Normal;
out vec4 FragColor;

const vec3 lightPos = vec3(1.0,1.0,1.0);
const vec4 ambientColor = vec4(0.4, 0.4, 0.4,1.0);
const vec4 diffuseColor = vec4(0.3,0.3,0.3,1.0);
void main()
{
	vec3 ct,cf;
    vec4 texel;
    float intensity,at,af;
    intensity = max(dot(lightPos,normalize(pass_Normal)),0.0);
    cf = intensity * diffuseColor.rgb +  ambientColor.rgb;
    af = diffuseColor.a;
    texel = texture(texture0,pass_Texture);
    ct = texel.rgb;
    at = texel.a;
    FragColor = vec4(ct * cf, texel.a);	

}