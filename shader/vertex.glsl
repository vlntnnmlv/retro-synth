#version 330 core

layout (location = 0) in vec3 aPos;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

in vec3 color;
in vec2 texcoord;

out vec3 Color;
out vec2 Texcoord;

void main()
{                  
    Color = color;
    Texcoord = texcoord;                     
    gl_Position = projection * view * model * vec4(aPos, 1.0);
}