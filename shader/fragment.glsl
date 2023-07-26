#version 150 core

in vec2 Texcoord;

uniform sampler2D tex;
uniform float in_Time;

out vec4 out_Color;

void main()
{
    out_Color = vec4(sin(in_Time) * 0.66, 0.66, 0.66, 1.0);
}