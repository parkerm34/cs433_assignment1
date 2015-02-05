#version 410 core

int vec4 color;

out vec4 fColor;

void
main()
{
//    fColor = vec4( 1.0, 0.0, 1.0, 0.0 );
    fColor = color;
}
