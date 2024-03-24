#version 330 core

out vec3 color;
uniform vec3 custom_color;

void main()
{
    color = custom_color;
}