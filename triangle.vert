#version 430 core

layout(location = 1) in vec4 vPosition;
layout(location = 2) in vec4 vColor;

out vec4 color;

void
main()
{
    color = vColor;
    gl_Position = vPosition;
}
