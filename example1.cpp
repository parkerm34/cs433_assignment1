// ch01.cpp : Defines the entry point for the console application.
//
//////////////////////////////////////////////////////////
//  triangles.cpp  from the OpenGL Red Book   Chapter 1
//////////////////////////////////////////////////////////

#include <iostream>
#include <cmath>
using namespace std;

#include "vgl.h"
#include "LoadShaders.h"

const GLuint Triangles = 0, Triangle = 1, Circle = 2, NumVAOs = 3;
const GLuint ArrayBuffer=0, TriBuffer = 1, TriColorBuffer = 2, CircBuffer = 3, CircColorBuffer = 4, NumBuffers=5;
//const GLuint vPosition=0, vColor = 1, cPosition = 2, cColor = 3;
const GLuint trisVert = 0, triVert = 1, triColor = 2, circVert = 3, circColor = 4;

GLuint Program, Program2;

const int debug = 1, debug_level2 = 1;
bool X = 1, Y = 1, Z = 0, W = 0, S = 0;
float C1 = 0.0f, C2 = 0.0f, C3 = 1.0f, G_RAD = 0.0f;
int G_NUM = 0;

GLuint VAOs[NumVAOs];
GLuint Buffers[NumBuffers];
GLuint elementbuffer;
GLuint elementbuffer2;

GLfloat** circ_vertices;
GLfloat * circ_vertices_memory;

GLfloat circ_vert2[6][2] = {
	{0.25, 0.43301},
	{-0.25, 0.433013},
	{-0.5, 0.0},
	{-0.25, -0.433012},
	{0.25, -0.433014},
	{0.5, 0.0}
};

const GLuint NumTrisVertices = 6, NumTriVertices = 3;
const GLfloat Pi = 3.14159f;
GLuint NumCircVertices = 6;

struct VertexData {
	GLubyte color[4];
	GLfloat position[2];
};

struct Color {
	GLuint col[4];
};

struct Vertex {
	GLfloat position[4];
};

/////////////////////////////////////////////////////
//  int
/////////////////////////////////////////////////////
void init (void )
{
	glGenVertexArrays( NumVAOs, VAOs );
	glBindVertexArray( VAOs[Triangles]);

	GLfloat tris_vertices[6][2] = {
		{ -0.90f, -0.90f },
		{ 0.85f, -0.90f },
		{ -0.90f, 0.85f },
		{ 0.90f, -0.85f },
		{ 0.90f, 0.90f },
		{ -0.85f, 0.90f }
	};

	glGenBuffers(NumBuffers, Buffers);
	glBindBuffer(GL_ARRAY_BUFFER, Buffers[ArrayBuffer]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(tris_vertices), tris_vertices, GL_STATIC_DRAW);


	ShaderInfo  shaders[] = {
		{ GL_VERTEX_SHADER, "triangles.vert" },
		{ GL_FRAGMENT_SHADER, "triangles.frag" },
		{ GL_NONE, NULL }
	};

	Program = LoadShaders( shaders );
	glUseProgram( Program );

	glVertexAttribPointer(trisVert, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
	glEnableVertexAttribArray(trisVert);

	GLfloat tri_vertices[3][2] = {
		{ -0.90f, -0.90f },
		{ 0.00f, 0.90f },
		{ 0.90f, -0.90f }
	};

	GLfloat tri_color[3][4] = {
		{ 0.0f, 0.0f, 1.0f, 1.0f },
		{ 1.0f, 0.0f, 0.0f, 1.0f },
		{ 0.0f, 1.0f, 0.0f, 1.0f }
	};


	glBindBuffer( GL_ARRAY_BUFFER, Buffers[TriBuffer] );
	glBufferData(GL_ARRAY_BUFFER, sizeof(tri_vertices), tri_vertices, GL_STATIC_DRAW);

	glBindBuffer( GL_ARRAY_BUFFER, Buffers[TriColorBuffer] );
	glBufferData( GL_ARRAY_BUFFER, sizeof(tri_color), tri_color, GL_STATIC_DRAW );

	ShaderInfo  shaders2[] = {
                { GL_VERTEX_SHADER, "triangle.vert" },
                { GL_FRAGMENT_SHADER, "triangle.frag" },
                { GL_NONE, NULL }
        };

	Program2 = LoadShaders( shaders2 );
        glUseProgram( Program2 );

	glBindVertexArray( VAOs[Triangle] );
	glEnableVertexAttribArray(triVert);
	glEnableVertexAttribArray(triColor);

	glBindBuffer( GL_ARRAY_BUFFER, Buffers[TriBuffer] );
	glVertexAttribPointer(triVert, 2, GL_FLOAT, GL_FALSE, 0, NULL);
	glBindBuffer( GL_ARRAY_BUFFER, Buffers[TriColorBuffer] );
	glVertexAttribPointer(triColor, 4, GL_FLOAT, GL_FALSE, 0, NULL);
/*
	GLfloat circ_vertices[3][2] = {
                { -0.45f, -0.45f },
                { 0.00f, 0.45f },
                { 0.45f, -0.45f }
        };
*/
	glBindBuffer(GL_ARRAY_BUFFER, Buffers[CircBuffer]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(circ_vertices), circ_vertices, GL_DYNAMIC_DRAW);

	glUseProgram( Program );

	glBindVertexArray(VAOs[Circle]);
	glEnableVertexAttribArray(trisVert);

	glBindBuffer( GL_ARRAY_BUFFER, Buffers[CircBuffer] );
	glVertexAttribPointer(trisVert, 2, GL_FLOAT, GL_FALSE, 0, NULL);

}

void print_circ()
{
	for(int i = 0; i < G_NUM; i++)
	{
		for(int j = 0; j < 2; j++)
		{
			cout << "circ_vert2: " << circ_vert2[i][j] << " " << &circ_vert2[i][j] << endl;
			cout << "circ_vertices: " << circ_vertices[i][j] << " " << &circ_vertices[i][j] << endl;
		}
		cout << endl;
	}
}

////////////////////////////////////////////////////////////////////
//	display
////////////////////////////////////////////////////////////////////
void display (void )
{
	glClear( GL_COLOR_BUFFER_BIT );

	glUseProgram( Program );
	glBindVertexArray( VAOs[Triangles] );
	if(!W)glDrawArrays( GL_TRIANGLES, 0, NumTrisVertices );
	else glDrawArrays( GL_LINE_LOOP, 0, NumTrisVertices );

	glUseProgram( Program2 );
	glBindVertexArray( VAOs[Triangle] );
        if(W)glDrawArrays( GL_LINE_LOOP, 0, NumTriVertices );
	else glDrawArrays( GL_TRIANGLES, 0, NumTrisVertices );

	if(Z)
	{
		glUseProgram( Program );
		glBindVertexArray( VAOs[Circle] );
		if(!W)glDrawArrays(GL_TRIANGLE_FAN, 0, G_NUM );
		else glDrawArrays( GL_LINE_LOOP, 0, G_NUM );
	}

//	if(W)
//		glDrawArrays( GL_LINES, 0, NumVertices);
//	else
//		glDrawArrays( GL_TRIANGLES, 0, NumVertices);
//	glBindVertexArray( VAOs[Triangle] );
//	glDrawArrays( GL_TRIANGLES, 0, 6 );
	//glDrawArrays( GL_LINES, 0, NumVertices );

	glFlush();
}

void generate_circle_verts()
{
	GLfloat yval = 0.0f, xval = 0.0f;
//	int N = 2;
//	GLfloat** circ_vertices = new GLfloat*[2];
	circ_vertices = new GLfloat * [G_NUM*2];

	circ_vertices_memory = new GLfloat [G_NUM * 2];

	for(size_t i = 0; i < G_NUM; i++)
	{
		circ_vertices[i] = circ_vertices_memory + (i * 2 + (sizeof(GLfloat)*2 + 1));
		circ_vertices[i][0] = (GLfloat)G_RAD*std::cos((2*Pi/(G_NUM))*(i+1));
		cout << &circ_vertices[i][0] << " " << circ_vertices[i][0] << endl;
		cout << &circ_vertices[i][1] << endl;
	}

	for(size_t i = 0; i < G_NUM; i++){
//		circ_vertices[i] = new GLfloat[2];
		xval = (GLfloat)G_RAD*std::cos((2*Pi/(G_NUM))*(i+1));
		yval = (GLfloat)G_RAD*std::sin((2*Pi/(G_NUM))*(i+1));
//		circ_vertices[i][0] = xval;
	}
	cout << "Size of circ_vert2: " << sizeof(circ_vert2) << endl;
	cout << "Size of circ_vertices: " << sizeof(circ_vertices)*G_NUM << endl;
	cout << "Size of circ_vertices_memory: " << sizeof(GLfloat) << endl;
/*
	for(int i = 1; i <= G_NUM; i++)
	{
		for(int j = 1; j <= 2; j++)
		{
			if(j == 1)
			{
				xval = G_RAD*std::cos((2*Pi/(G_NUM))*i);
				cout << xval;
				if(xval < 0.0001 && xval > -0.0001) xval = 0;
				circ_vertices[i-1][j-1] = xval;
			}
			if(j == 2)
			{
				yval = G_RAD*std::sin((2*Pi/(G_NUM))*i);
				cout << " " << yval << endl;
				if(yval < 0.0001 && yval > -0.0001) yval = 0;
				circ_vertices[i-1][j-1] = yval;
			}
		}
	}
/*
	circ_vertices = new GLfloat[3][2];

	circ_vertices = {
                { -0.45f, -0.45f },
                { 0.00f, 0.45f },
                { 0.45f, -0.45f }
        };
*/
	glBindBuffer(GL_ARRAY_BUFFER, Buffers[CircBuffer]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(circ_vertices)*G_NUM, circ_vertices, GL_DYNAMIC_DRAW);
//	glBufferData(GL_ARRAY_BUFFER, sizeof(circ_vert2), circ_vert2, GL_STATIC_DRAW);
}

void generate_circle()
{
	fflush(stdin);
	if(debug | debug_level2)
		cout << "User has selected to generate a circle. Please input radius value as float: ";
	cin >> G_RAD;
	if(debug | debug_level2)
		cout << "Please input number of steps to estimate circle: ";
	cin >> G_NUM;
	generate_circle_verts();
	return;
}

void set_color()
{
	GLint TrisLoc = glGetUniformLocation(Program, "myColor");
	float newCol[4] = { C1, C2, C3, 1.0 };
	glProgramUniform4fv(Program, TrisLoc, 1, newCol);
}

void change_color()
{
	fflush(stdin);
	if(debug | debug_level2)
		cout << "User has selected to change the color. Please input color value for red as float: ";
	cin >> C1;
	if(debug | debug_level2)
		cout << "Please input color value for green as float: ";
	cin >> C2;
	if(debug | debug_level2)
                cout << "Please input color value for blue as float: ";
        cin >> C3;
	set_color();
	return;
}

void idle_CB()
{
	glutPostRedisplay();
}

void key_CB(unsigned char key, int x_cord, int y_cord)
{
	switch (key)
	{
		// Specify the color for both triangles and circle, user provides 3 floating point numbers
		// through stdin for the color, in the range 0. to 1., in the order of red, green, blue components.
		// The default initial color is blue (0., 0., 1.)
		case'c':
			if(debug)
				cout << key << " received" << endl;
			change_color();
			glutPostRedisplay();
			break;
		// shaded surface display : renders objects as solid surfaces
		// the drawing or rendering mode in OpenGL mode is controlled through a call to glPolygonMode.
		// After changing the OpenGL rasterization (drawing) mode, you can force a redraw through GLUT
		// using glutPostRedisplay function call.
                case's':
			S = !S;
			if(debug)
                        	cout << key << " received" << endl; 
			glutPostRedisplay();
                        break;
                // wireFrame display : render ofbjects in wireframe. glLineWidth to specify thickness of lines
                case'w':
			W = !W;
			if(debug)
                        	cout << key << " received" << W << endl;
			glutPostRedisplay();
                        break;
                // Generate the gemoetry for the circle, user needs to provide a floating point number for radius
		// value in the range of 0. to 1., and a integer number for steps to use when generating triangles
		// to approx the circle
                case'g':
			if(debug)
                        	cout << key << " received" << endl;
			generate_circle();
			cout << "G_RAD = " << G_RAD << endl << "G_NUM = " << G_NUM << endl;
                        break;
                // Toggle display of the two striangles, default value is on
                case'x':
			glUseProgram( Program );
			X = !X;

			if(X)
			{
				glBindVertexArray( VAOs[Triangles] );
				glEnableVertexAttribArray( trisVert );
			}
			if(!X)
			{
				glBindVertexArray( VAOs[Triangles] );
				glDisableVertexAttribArray( trisVert );
			}
			if(debug)
                        	cout << key << " received " << X << endl;
			glutPostRedisplay();
                        break;
                // Toggle display of the single triangle, default value is on
               case'y':
			glUseProgram( Program2 );
			Y = !Y;
			if(Y)
			{
				glBindVertexArray( VAOs[Triangle] );
                                glEnableVertexAttribArray( triVert );
			}
                        if(!Y)
			{
				glBindVertexArray( VAOs[Triangle] );
                                glDisableVertexAttribArray( triVert );
			}
			if(debug)
                        	cout << key << " received " << Y << endl;
			glutPostRedisplay();
                        break;
                // Toggle display of the circle, default is off, should not be visible until after g(enerate) is used.
                case'z':
			print_circ();
			Z = !Z;
			if(Z)
			{
				glBindVertexArray( VAOs[Circle] );
                                glEnableVertexAttribArray( trisVert );
			}
                        if(!Z)
			{
				glBindVertexArray( VAOs[Circle] );
                                glDisableVertexAttribArray( trisVert );
			}
			if(debug)
                        	cout << key << " received " << Z << endl;
			glutPostRedisplay();
                        break;
                // Quits application
                case'q':
			if(debug)
				cout << key << " received" << endl;
                        exit(0); 
                        break;
                
                default:
			if(debug)
                        	cout << "[default] " << key << " received and not handled." << endl;
                        break;
	}
}                


////////////////////////////////////////////////////////////////////////
//	main
////////////////////////////////////////////////////////////////////////
int main(int argc, char* argv[])
{
	// initialize GLUT system
	glutInit( &argc, argv );

	// initialize display format
	glutInitDisplayMode( GLUT_3_2_CORE_PROFILE | GLUT_RGBA );

	// initialize window size
	glutInitWindowSize( 512, 512 );


	//glutInitContextVersion( 3, 2 );
	//glutInitContextProfile( GLUT_CORE_PROFILE );// GLUT_COMPATIBILITY_PROFILE );

	// create window with name from executable
	glutCreateWindow( argv[0] );

	glutIdleFunc(idle_CB);

	glutKeyboardFunc(key_CB);

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

