#version 410 core

in vec4 color;

uniform vec4 myColor = vec4(0.0, 0.0, 1.0, 1.0);

out vec4 fColor;

void
main()
{
//    fColor = vec4( 1.0, 0.0, 1.0, 0.0 );
    fColor = color;
}
