/*
 * Copyright (c) 1993-1997, Silicon Graphics, Inc.
 * ALL RIGHTS RESERVED 
 * Permission to use, copy, modify, and distribute this software for 
 * any purpose and without fee is hereby granted, provided that the above
 * copyright notice appear in all copies and that both the copyright notice
 * and this permission notice appear in supporting documentation, and that 
 * the name of Silicon Graphics, Inc. not be used in advertising
 * or publicity pertaining to distribution of the software without specific,
 * written prior permission. 
 *
 * THE MATERIAL EMBODIED ON THIS SOFTWARE IS PROVIDED TO YOU "AS-IS"
 * AND WITHOUT WARRANTY OF ANY KIND, EXPRESS, IMPLIED OR OTHERWISE,
 * INCLUDING WITHOUT LIMITATION, ANY WARRANTY OF MERCHANTABILITY OR
 * FITNESS FOR A PARTICULAR PURPOSE.  IN NO EVENT SHALL SILICON
 * GRAPHICS, INC.  BE LIABLE TO YOU OR ANYONE ELSE FOR ANY DIRECT,
 * SPECIAL, INCIDENTAL, INDIRECT OR CONSEQUENTIAL DAMAGES OF ANY
 * KIND, OR ANY DAMAGES WHATSOEVER, INCLUDING WITHOUT LIMITATION,
 * LOSS OF PROFIT, LOSS OF USE, SAVINGS OR REVENUE, OR THE CLAIMS OF
 * THIRD PARTIES, WHETHER OR NOT SILICON GRAPHICS, INC.  HAS BEEN
 * ADVISED OF THE POSSIBILITY OF SUCH LOSS, HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, ARISING OUT OF OR IN CONNECTION WITH THE
 * POSSESSION, USE OR PERFORMANCE OF THIS SOFTWARE.
 * 
 * US Government Users Restricted Rights 
 * Use, duplication, or disclosure by the Government is subject to
 * restrictions set forth in FAR 52.227.19(c)(2) or subparagraph
 * (c)(1)(ii) of the Rights in Technical Data and Computer Software
 * clause at DFARS 252.227-7013 and/or in similar or successor
 * clauses in the FAR or the DOD or NASA FAR Supplement.
 * Unpublished-- rights reserved under the copyright laws of the
 * United States.  Contractor/manufacturer is Silicon Graphics,
 * Inc., 2011 N.  Shoreline Blvd., Mountain View, CA 94039-7311.
 *
 * OpenGL(R) is a registered trademark of Silicon Graphics, Inc.
 */

#include <stdio.h>
#include <stdlib.h>

#include <GL/glu.h>
#include <GL/glut.h>

GLUquadricObj *qobj;

void init(void) 
{
	/*
	* http://devernay.free.fr/cours/opengl/materials.html
	*/

	GLfloat light_position[] = {1.0, 1.0, 1.0, 0.0};
	GLfloat model_ambient[] = {0.5, 0.5, 0.5, 1.0};

	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, model_ambient);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_DEPTH_TEST);

	glClearColor(0.0, 0.0, 0.0, 0.0);

/*
	// Bronze
	GLfloat mat_ambient[] = {0.2125, 0.1275, 0.054, 1.0};
	GLfloat mat_diffuse[] = {0.714, 0.4284, 0.18144};
	GLfloat mat_specular[] = {0.393548, 0.271906, 0.166721};
	GLfloat shine = 0.2;
*/
/*
	// Silver
	GLfloat mat_ambient[] = {0.19225, 0.19225, 0.19225, 1.0};
	GLfloat mat_diffuse[] = {0.50754, 0.50754, 0.50754};
	GLfloat mat_specular[] = {0.508273, 0.508273, 0.508273};
	GLfloat shine = 0.4;
*/
	// Pearl
	GLfloat mat_ambient[] = {0.25, 0.20725, 0.20725, 1.0};
	GLfloat mat_diffuse[] = {1, 0.829, 0.829};
	GLfloat mat_specular[] = {0.296648, 0.296648, 0.296648};
	GLfloat shine = 0.088;

	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialf(GL_FRONT, GL_SHININESS, shine * 128.0);

	qobj = gluNewQuadric();

	gluQuadricDrawStyle(qobj, GLU_FILL);
	gluQuadricNormals(qobj, GLU_SMOOTH);
}

void display(void)
{
	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glPushMatrix();

	glEnable(GL_LIGHTING);
	glShadeModel(GL_SMOOTH);
	glColor3f(0.0, 0.0, 0.0);

	glPushMatrix();
		glTranslatef(0.0, 1.0, 0.0);
		glRotatef(300.0, 1.0, 0.0, 0.0);

		gluSphere(qobj, 0.5, 15, 10);
	glPopMatrix();

	glEnable(GL_LIGHTING);
	glShadeModel(GL_SMOOTH);
	glColor3f(0.0, 0.0, 0.0);

	glPushMatrix();
		glTranslatef(0.0, 0.0, 0.0);
		glRotatef(300.0, 1.0, 0.0, 0.0);

		gluSphere(qobj, 0.5, 15, 10);
	glPopMatrix();

	glDisable(GL_LIGHTING);
	glShadeModel(GL_SMOOTH);
	glColor3f(1.0, 0.0, 0.0);

	glPushMatrix();
		glTranslatef(0.0, -1.0, 0.0);
		glRotatef(300.0, 1.0, 0.0, 0.0);

		gluSphere(qobj, 0.5, 15, 10);
	glPopMatrix();

	glPopMatrix();
	glFlush();
}

void reshape (int w, int h)
{
	glViewport(0, 0, (GLsizei) w, (GLsizei) h);

	glMatrixMode(GL_PROJECTION);

	glLoadIdentity();

	if (w <= h)
		glOrtho(-2.5, 2.5, -2.5*(GLfloat)h/(GLfloat)w, 2.5*(GLfloat)h/(GLfloat)w, -10.0, 10.0);
	else
		glOrtho(-2.5*(GLfloat)w/(GLfloat)h,2.5*(GLfloat)w/(GLfloat)h, -2.5, 2.5, -10.0, 10.0);

	glMatrixMode(GL_MODELVIEW);

	glLoadIdentity();
}

void keyboard(unsigned char key, int x, int y)
{
	switch (key) {
		case 'q':
		case 'Q':
			exit(0);
		break;
	}
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);

	glutInitWindowSize(500, 500);

	glutInitWindowPosition(100, 100);

	glutCreateWindow(argv[0]);

	init();

	glutDisplayFunc(display);

	glutReshapeFunc(reshape);

	glutKeyboardFunc(keyboard);

	glutMainLoop();

	return 0;
}
