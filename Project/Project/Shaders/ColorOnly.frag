#version 460 core

out vec4 FragColor;
in vec2 v_UV;
uniform vec4 u_Color;

void main()
{
    FragColor =  u_Color;
}
