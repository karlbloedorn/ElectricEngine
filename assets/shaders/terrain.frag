#version 130

uniform sampler2D texture0;
uniform sampler2D texture1;
uniform sampler2D texture2;
uniform sampler2D texture3;
uniform sampler2D texture4;

in vec2 pass_Texture;
in vec3 pass_Normal;
in vec2 pass_BlendTextureCoordinates;

//out vec4 gl_FragColor; implied

const vec3 lightPos = vec3(1.0,1.0,1.0);
const vec4 ambientColor = vec4(0.3, 0.3, 0.3,1.0);
const vec4 diffuseColor = vec4(0.5, 0.5, 0.5,1.0);
const vec4 specColor = vec4(1.0, 1.0, 1.0,1.0);

void main()
{
	vec3 ct,cf;

    float intensity,at,af;
    intensity = max(dot(lightPos,normalize(pass_Normal)),0.0);
    cf = intensity * diffuseColor.rgb +  ambientColor.rgb;
    af = diffuseColor.a;
	
	vec4 blendMapColor = texture2D(texture0,pass_BlendTextureCoordinates);
	float backTextureAmount = 1 - ( blendMapColor.r +  blendMapColor.g +  blendMapColor.b);
	vec4 colorBack = texture2D(texture1, pass_Texture) * backTextureAmount;
	vec4 colorR = texture2D(texture2, pass_Texture) * blendMapColor.r;
	vec4 colorG = texture2D(texture3, pass_Texture) * blendMapColor.g;
	vec4 colorB = texture2D(texture4, pass_Texture) * blendMapColor.b;
	ct = (colorBack + colorR + colorG + colorB).rgb;
    at = 1.0;
    gl_FragColor = vec4(ct * cf, at * af);	
}