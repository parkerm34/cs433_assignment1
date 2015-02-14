#version 430 core

layout(location = 0) in vec4 trisVert;
layout(location = 1) in vec4 trisColor;
layout(location = 3) in vec4 circVert;

out vec4 color;

void
main()
{
    color = trisColor;
    gl_Position = trisVert;
     // gl_Position = circVert;
}
