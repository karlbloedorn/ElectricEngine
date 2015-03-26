#version 330 core

uniform sampler2D texture0;
in vec2 pass_Texture;
out vec4 FragColor;
 
void main()
{
    vec4 texel = texture(texture0,pass_Texture);
    FragColor = texel;
}