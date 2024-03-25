#version 330 core

in vec3 Color;
in vec2 Texcoord;

uniform sampler2D in_texture;

out vec4 color;

void main()
{
    color = texture(in_texture, Texcoord) * vec4(Color, 1.0);
}