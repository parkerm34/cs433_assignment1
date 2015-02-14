/*
 * Program:
 *		CSc 533 - Cube by Nathan Sema.
 *
 * Overview:
 * 		The Cube program loads 6 images as a "ImageData" jpeg image structures
 *		where each image corresponds to a given face of a cube.
 *
 *		The a opengl mode is implemented in which the cube is displayed using
 *		the hardware-accelerated GL pipeline depicted by gluPerspective().
 *		Notice this mode is much more efficient granted the images are textured
 *		on the sides of the cube.
 *
 *		Rotations are implemented accordingly:
 *
 *			Rotation around the Y axis : 'y'
 *			Rotation around the X axis : 'x'
 *			Rotation around the Z axis : 'z'
 *			Rotation around the diagonal axis : 'd'
 *
 *		Pressing the 's' key switches the direction in which cube is rotating.
 *
 *		Pressing the '-'/'+' decreases/increases the degree of rotation at which
 *		the cube is rotating.
 *
 *		Pressing the 'r' key resets the cube to its initial front face. The 'q'
 *		key exits the program, first destroying the loaded textured images and rgb 
 *		pixel data accordingly.
 *
 * Acknowledgements:
 *		Modified from a demonstrator program by Leonard D. Brown,
 *		University of Arizona.
 *
 *		Portions of demo code provided by Kobus Barnard,
 *		University of Arizona.
 *
 *		ByteRaster class and methods modified from Michael Garland's
 *		LibGFX library in accordance with granted terms and conditions
 *		(see http://graphics.cs.uiuc.edu/~garland/software/libgfx.html)
 *
 * To make this code relatively independent of build paths and install
 *      	paths, I've based directory paths in the VS project on environment
 *		variables GLUT_PATH and LIBJPEG_PATH.  These environment variables
 *		must be defined by the OS should point to the root directories of
 *		the GLUT and LIBJPEG libraries, respectively.
 *
 * If your system is lacking these APIs, you can find them here:
 *		GLUT: http://www.xmission.com/~nate/glut.html
 *		LibJPEG: http://www.ijg.org/
 *		Windows: Use the library provided on the CSc433 website OpenGL page.
 *					Other versions may cause jpeg_read_header to crash due
 *					to standard C library incompatibility issue with MSVC
 *					unless you rebuild the entire library from source.
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

#include <GL/glu.h>
#include <GL/glut.h>

#include <jpeglib.h>

/* rotation definitions */
#define Y 1
#define X 2
#define Z 3
#define D 4
#define R 5

/* direction definitions */
#define NEG 6
#define POS 7

typedef struct ImageData_ {
	unsigned char *pixels;
	int width;
	int height;
    GLuint texture;
} ImageData;

/* -------------------------------------------------------------------------- */

void load_images(void);
void destroy_images(void);

static void display_cube_CB(void);
static void display_cube_opengl_CB(void);

static void reshape_CB(GLsizei width, GLsizei height);
static void reshape_basic_CB(GLsizei width, GLsizei height);
static void reshape_opengl_CB(GLsizei width, GLsizei height);

static void idle_CB(void);
static void key_CB(unsigned char key, int x, int y);

void load_jpeg_data(char *file_name, ImageData *image);

/* -------------------------------------------------------------------------- */

/* default window size on our display device, in pixels */
GLsizei window_width = 600;
GLsizei window_height = 600;
GLfloat aspect = 0.0f;

/* math definitions */
static double pi = 3.14159265;

/* rotation */
static int direction = NEG;
static int rotation = R;

/* rotational angle for cube */
GLfloat angle = 0.0f;
GLfloat degree = 0.50f;

/* image pics */
static char pic1[]  = "images/pic1.jpg";
static char pic2[]  = "images/pic2.jpg";
static char pic3[]  = "images/pic3.jpg";
static char pic4[]  = "images/pic4.jpg";
static char pic5[]  = "images/pic5.jpg";
static char pic6[]  = "images/pic6.jpg";

/* image structures */
ImageData img1;
ImageData img2;
ImageData img3;
ImageData img4;
ImageData img5;
ImageData img6;

/* -------------------------------------------------------------------------- */

int main(int argc, char *argv[])
{
	/* initialize GLUT system */
	glutInit(&argc, argv);

	/* initialize display format */
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);

	/* define and get handle to the window (render) context */
	glutInitWindowSize(window_width, window_height);

	/* Create a window with the given title */
	glutCreateWindow("Cube");

	/* set window's display callback */
	glutDisplayFunc(display_cube_CB);

	/* handle window re-size event */
	glutReshapeFunc(reshape_CB);

	/* background events and animation */
	glutIdleFunc(idle_CB);

	/* set window's key callback */
	glutKeyboardFunc(key_CB);

	/* set background color to black and opaque */
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	/* clear background depth */
	glClearDepth(1.0f);

	/* set background depth */
	glDepthFunc(GL_LEQUAL);

	/* enable smooth shading */
	glShadeModel(GL_SMOOTH);

	/* nice perspective corrections */
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

	/* load all images/textures */
	load_images();

	/* start processing events */
	glutMainLoop();

    return 0;
}

/* -------------------------------------------------------------------------- */

void load_images(void)
{
	load_jpeg_data(pic1, &img1);
	load_jpeg_data(pic2, &img2);
	load_jpeg_data(pic3, &img3);
	load_jpeg_data(pic4, &img4);
	load_jpeg_data(pic5, &img5);
	load_jpeg_data(pic6, &img6);
}

/* -------------------------------------------------------------------------- */

void destroy_images(void)
{
	if (img1.texture) {
		glDeleteTextures(1, &img1.texture);
		free(img1.pixels);
	}
	if (img2.texture) {
		glDeleteTextures(1, &img2.texture);
		free(img2.pixels);
	}
	if (img3.texture) {
		glDeleteTextures(1, &img3.texture);
		free(img3.pixels);
	}
	if (img4.texture) {
		glDeleteTextures(1, &img4.texture);
		free(img4.pixels);
	}
	if (img5.texture) {
		glDeleteTextures(1, &img5.texture);
		free(img5.pixels);
	}
	if (img6.texture) {
		glDeleteTextures(1, &img6.texture);
		free(img6.pixels);
	}
}

/* -------------------------------------------------------------------------- */

static void display_cube_opengl_CB(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_PROJECTION);

	glLoadIdentity();

	gluPerspective(50.0f, aspect, 0.1f, 100.0f);

	glMatrixMode(GL_MODELVIEW);

	glLoadIdentity();

	glPushMatrix();

	glTranslatef(0.0f, 0.0f, -5.0f);

	if (rotation == X)
		glRotatef(angle, 1.0f, 0.0f, 0.0f);
	else if (rotation == Y)
		glRotatef(angle, 0.0f, 1.0f, 0.0f);
	else if (rotation == Z)
		glRotatef(angle, 0.0f, 0.0f, 1.0f);
	else if (rotation == D)
		glRotatef(angle, 1.0f, 1.0f, 0.0f);

	glEnable(GL_CULL_FACE);

	glEnable(GL_DEPTH_TEST);

	glEnable(GL_TEXTURE_2D);

	glColor3f(1.0f, 1.0f, 1.0f);

	glBindTexture(GL_TEXTURE_2D, img1.texture);

	// Front Face
	glBegin(GL_QUADS);

	glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f,  1.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f, -1.0f,  1.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,  1.0f,  1.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,  1.0f,  1.0f);

	glEnd();

	glColor3f(1.0f, 1.0f, 1.0f);

	glBindTexture(GL_TEXTURE_2D, img2.texture);

	// Back Face
	glBegin(GL_QUADS);

	glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f,  1.0f, -1.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f,  1.0f, -1.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f, -1.0f, -1.0f);

	glEnd();

	glColor3f(1.0f, 1.0f, 1.0f);

	glBindTexture(GL_TEXTURE_2D, img3.texture);

	// Right face
	glBegin(GL_QUADS);

	glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f, -1.0f, -1.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,  1.0f, -1.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f,  1.0f,  1.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f, -1.0f,  1.0f);

	glEnd();

	glColor3f(1.0f, 1.0f, 1.0f);

	glBindTexture(GL_TEXTURE_2D, img4.texture);

	// Left Face
	glBegin(GL_QUADS);

	glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f,  1.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f,  1.0f,  1.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,  1.0f, -1.0f);

	glEnd();

	glColor3f(1.0f, 1.0f, 1.0f);

	glBindTexture(GL_TEXTURE_2D, img5.texture);

	// Top Face
	glBegin(GL_QUADS);

	glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,  1.0f, -1.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f,  1.0f,  1.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f,  1.0f,  1.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,  1.0f, -1.0f);

	glEnd();

	glColor3f(1.0f, 1.0f, 1.0f);

	glBindTexture(GL_TEXTURE_2D, img6.texture);

	// Bottom Face
	glBegin(GL_QUADS);

	glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f, -1.0f, -1.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f, -1.0f, -1.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f, -1.0f,  1.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f,  1.0f);

	glEnd();

	glFlush();

	glDisable(GL_TEXTURE_2D);

	glDisable(GL_DEPTH_TEST);

	glDisable(GL_CULL_FACE);

	glPopMatrix();

	glutSwapBuffers();

	if (rotation != R) {
		if (direction == NEG) {
			angle -= degree;
		}
		else if (direction == POS) {
			angle += degree;
		}
	}
}

/* -------------------------------------------------------------------------- */

static void display_cube_CB(void)
{
	display_cube_opengl_CB();
}

/* -------------------------------------------------------------------------- */

static void reshape_CB(GLsizei width, GLsizei height)
{
	/* handle case for divide by 0 */
	if (height == 0)
		height = 1;

	/* compute aspect ratio of the new window */
	aspect = (GLfloat) width / (GLfloat) height;

	/* set the viewport */
	glViewport(0, 0, width, height);

	/* set the window width & height */
	window_width = width;
	window_height = height;
}

/* -------------------------------------------------------------------------- */

static void idle_CB (void)
{
	/* tell GLUT to redraw */
	glutPostRedisplay();
}

/* -------------------------------------------------------------------------- */

static void key_CB(unsigned char key, int x_cord, int y_cord)
{
	switch (key)
	{
		/* rotation 'x' - rotate the cube around the X axis */
		case 'x':
			rotation = X;
			break;
		/* rotation 'y' - rotate the cube around the Y axis */
		case 'y':
			rotation = Y;
			break;
		/* rotation 'z' - rotate the cube around the Z axis */
		case 'z':
			rotation = Z;
			break;
		/* rotation 'd' - rotate the cube around the diagonal axis */
		case 'd':
			rotation = D;
			break;
		/* rotation 'r' - reset the cube */
		case 'r':
			angle = 0.0f;
			rotation = R;
			break;

		/* switch direction of rotation */
		case 's':
			if (direction == NEG) {
				direction = POS;
			}
			else if (direction == POS) {
				direction = NEG;
			}
			break;

		/* decrease the degree of rotation */
		case '-':
			degree -= 0.1f;
			break;

		case '+':
			degree += 0.1f;
			break;

		/* quit the program -- 'hard quit' */
		case 'q':
			destroy_images();
			exit(0);
			break;
		default:
			break;
	}
}

/* -------------------------------------------------------------------------- */

void load_jpeg_data(char *file_name, ImageData *image)
{
	FILE* file;
	JSAMPARRAY buffer;

	int counter = 0;
	unsigned int data_size;
	unsigned int stride;

	struct jpeg_decompress_struct info;
	struct jpeg_error_mgr err;

	file = fopen(file_name, "rb");

	info.err = jpeg_std_error(& err);

	jpeg_create_decompress(& info);

	if(!file) {
		printf("Error: '%s' is missing.\n", file_name);
		exit(-1);
	}

	jpeg_stdio_src(&info, file);

	jpeg_read_header(&info, TRUE);

	jpeg_start_decompress(&info);

	image -> width = info.output_width;
	image -> height = info.output_height;

	stride = info.output_width * info.output_components;

	data_size = info.output_width * info.output_height * 3;

	counter = data_size;

	buffer = (JSAMPARRAY) malloc(sizeof(JSAMPROW));

	buffer[0] = (JSAMPROW) malloc(sizeof(JSAMPLE) * stride);

	image -> pixels = (unsigned char *) malloc(data_size);

	while (info.output_scanline < info.output_height) {
		counter -= stride;
		jpeg_read_scanlines(&info, buffer, 1);
		memcpy(image->pixels + counter, buffer[0], stride);
	}

	jpeg_finish_decompress(&info);

	fclose(file);

	glGenTextures(1, &image -> texture);
	glBindTexture(GL_TEXTURE_2D, image -> texture);

	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image -> width, image -> height, 0, GL_RGB, GL_UNSIGNED_BYTE, image -> pixels);
}

/* -------------------------------------------------------------------------- */
