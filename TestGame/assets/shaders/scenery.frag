#version 130

uniform sampler2D texture;
in vec2 pass_Texture;
in vec3 pass_Normal;
const vec3 lightPos = vec3(1.0,1.0,1.0);
const vec4 ambientColor = vec4(0.4, 0.4, 0.4,1.0);
const vec4 diffuseColor = vec4(0.3,0.3,0.3,1.0);
void main()
{
	//vec4 texel = texture2D(texture, pass_Texture);
	//gl_FragColor = vec4(texel.rgb,1.0);	
	
	vec3 ct,cf;
    vec4 texel;
    float intensity,at,af;
    intensity = max(dot(lightPos,normalize(pass_Normal)),0.0);
    cf = intensity * diffuseColor.rgb +  ambientColor.rgb;
    af = diffuseColor.a;
    texel = texture2D(texture, pass_Texture);
    ct = texel.rgb;
    at = texel.a;
    gl_FragColor = vec4(ct * cf, at * af);	

}