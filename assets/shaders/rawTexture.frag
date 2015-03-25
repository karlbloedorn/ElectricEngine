#version 330

uniform sampler2D texture0;
in vec2 pass_Texture;

void main()
{
    vec4 texel = texture2D(texture0,pass_Texture);
    gl_FragColor = texel;
}