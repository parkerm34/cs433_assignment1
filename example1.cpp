// ch01.cpp : Defines the entry point for the console application.
//
//////////////////////////////////////////////////////////
//  triangles.cpp  from the OpenGL Red Book   Chapter 1
//////////////////////////////////////////////////////////

#include <iostream>
using namespace std;

#include "vgl.h"
#include "LoadShaders.h"

//enum VAO_IDs { Triangles, NumVAOs };
const GLuint Triangles = 0, NumVAOs = 1;
//enum Buffer_IDs { ArrayBuffer, NumBuffers };
const GLuint ArrayBuffer=0, NumBuffers=1;
//enum Attrib_IDs { vPosition = 0 };
const GLuint vPosition=0, vColor = 1;

GLuint VAOs[NumVAOs];
GLuint Buffers[NumBuffers];

const GLuint NumVertices = 9;

/////////////////////////////////////////////////////
//  int
/////////////////////////////////////////////////////
void init (void )
{
	glGenVertexArrays( NumVAOs, VAOs );
	glBindVertexArray( VAOs[Triangles] );

	struct VertexData {
		GLubyte color[4];
		GLfloat position[4];
	};

	VertexData vertices[NumVertices] = {
		{{ 255,   0,   0, 255 }, { -0.90f, -0.9f }},      // Triangle 1
                {{   0, 255,   0, 255 }, {  0.85f, -0.9f }},
                {{   0,   0, 255, 255 }, { -0.90f,  0.85f }},
                {{  10,  10,  10, 255 }, { 0.90f, -0.85f }},      // Triangle 2
                {{ 100, 100, 100, 255 }, { 0.90f,  0.90f }},
                {{ 255, 255, 255, 255 }, { -0.85f, 0.90f }},
		{{   0,   0,   0, 255 }, { 0.00f, 0.90f }},	// Triangle 3
		{{   0,   0,   0, 255 }, { -0.90f, -0.90f }},
		{{   0,   0,   0, 255 }, { 0.90f, -0.90f }}
	};

	glGenBuffers( NumBuffers, Buffers );
	glBindBuffer( GL_ARRAY_BUFFER, Buffers[ArrayBuffer] );
	glBufferData( GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW );

	ShaderInfo  shaders[] = {
		{ GL_VERTEX_SHADER, "triangles.vert" },
		{ GL_FRAGMENT_SHADER, "triangles.frag" },
		{ GL_NONE, NULL }
	};

	GLuint program = LoadShaders( shaders );
	glUseProgram( program );

	glVertexAttribPointer( vColor, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(VertexData), BUFFER_OFFSET(0));
	glVertexAttribPointer( vPosition, 2, GL_FLOAT, GL_FALSE, sizeof(VertexData), BUFFER_OFFSET(sizeof(vertices[0].color)));
	glEnableVertexAttribArray(vColor);
	glEnableVertexAttribArray( vPosition );

}

////////////////////////////////////////////////////////////////////
//	display
////////////////////////////////////////////////////////////////////
void display (void )
{
	glClear( GL_COLOR_BUFFER_BIT );
	glBindVertexArray( VAOs[Triangles] );
	glDrawArrays( GL_TRIANGLES, 0, NumVertices );
	//glDrawArrays( GL_LINES, 0, NumVertices );

	glFlush();
}

////////////////////////////////////////////////////////////////////////
//	main
////////////////////////////////////////////////////////////////////////
int main(int argc, char* argv[])
{
	

	glutInit( &argc, argv );
	glutInitDisplayMode( GLUT_3_2_CORE_PROFILE | GLUT_RGBA );
	glutInitWindowSize( 512, 512 );

	//glutInitContextVersion( 3, 2 );
	//glutInitContextProfile( GLUT_CORE_PROFILE );// GLUT_COMPATIBILITY_PROFILE );
	glutCreateWindow( argv[0] );

	glewExperimental = GL_TRUE;	// added for glew to work!
	if ( glewInit() )
	{
		cerr << "Unable to initialize GLEW ... exiting" << endl;
		exit (EXIT_FAILURE );
	}

	// code from OpenGL 4 Shading Language cookbook, second edition
	const GLubyte *renderer = glGetString( GL_RENDERER );
	const GLubyte *vendor = glGetString( GL_VENDOR );
	const GLubyte *version = glGetString( GL_VERSION );
	const GLubyte *glslVersion = glGetString (GL_SHADING_LANGUAGE_VERSION );
	GLint major, minor;
	glGetIntegerv( GL_MAJOR_VERSION, &major );
	glGetIntegerv( GL_MINOR_VERSION, &minor );

	cout << "GL Vendor            :" << vendor << endl;
	cout << "GL Renderer          :" << renderer  << endl;
	cout << "GL Version (string)  :" << version << endl;
	cout << "GL Version (integer) :" << major << " " << minor << endl;
	cout << "GLSL Version         :" << glslVersion << endl;
	//
	//GLint nExtensions;
	//glGetIntegerv( GL_NUM_EXTENSIONS, &nExtensions );
	//for ( int i = 0; i < nExtensions; i++ )
	//	cout << glGetStringi( GL_EXTENSIONS, i )  << endl;

	init();
	glutDisplayFunc( display );
	glutMainLoop();

	return 0;
}

