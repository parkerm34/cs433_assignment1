#include <assert.h>
#include <GL/glut.h>
#include <math.h>
#include <memory.h>
#include <stdlib.h>

/* this program contains a few sections:
 *
 * vector stuff: simple vector functions
 * matrix stuff: matrix related functions, including matrix * vector mult
 * quad stuff: for messing with Quad structure
 * other stuff: glut code and main()
 *
 * so basically it starts off with g_oquad, and the glut idle function is
 * called repeatedly. each time the glut idle function is called, the frame
 * number is incremented and a transformation matrix is calculated based on
 * the frame number (for animation). the transformation matrix is applied
 * to the 4 vertices and normals of g_oquad, and the result is stored in
 * g_tquad. g_tquad is rendered on the screen.
 *
 * that math is performed by transform_quad(), the rendering is trivial and
 * is done by glut_display().
 *
 * i used frame counts for animation timing because i do not know what
 * system you are running this on; and timing is different on windows and
 * linux. if you want to change the animation speed then you can scale the
 * frame number in transform_quad().
 *
 * i put the globals down in the "other stuff" section; sorry this is all
 * so ugly; hopefully it's readable.
 */


/* you have a full model; i just chose a single quad for simplicity. the
 * quad has four points, pt[index][xyzw]. */
typedef struct {
  double pt[4][4];
  double n[4][4];  /* normals */
} Quad;

/* this is a 4x4 transformation matrix; el[row][column]. */
typedef struct Matrix {
  double el[4][4];
} Matrix;


/*---------------
 * vector stuff
 *---------------*/


void vec4_set (double v[4], double x, double y, double z, double w) {
  v[0] = x;
  v[1] = y;
  v[2] = z;
  v[3] = w;
}


/* note: ignores w component */
void vec3_normalize (double v[3]) {
  double mag = sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
  v[0] /= mag;
  v[1] /= mag;
  v[2] /= mag;
}


/*----------------
 * matrix stuff
 *----------------*/


void matrix_make_identity (Matrix *m) {

  int i;

  /* first zero the whole thing, then set the diagonal to 1's */
  memset(m->el, 0, sizeof(double) * 4 * 4);
  for (i = 0; i < 4; ++ i)
    m->el[i][i] = 1.0;

}


/* make transformation matrix for translation */
void matrix_make_translate (Matrix *m, double x, double y, double z) {

  /* the rightmost column contains the translation values for this
   * type of transformation. */
  matrix_make_identity(m);
  m->el[0][3] = x;
  m->el[1][3] = y;
  m->el[2][3] = z;

}


/* make transformation matrix for rotation, given roll pitch and yaw IN
 * RADIANS. */
void matrix_make_rpy (Matrix *m, double r, double p, double y) {

  double cosr, cosp, cosy, sinr, sinp, siny;

  cosr = cos(r);
  cosp = cos(p);
  cosy = cos(y);
  sinr = sin(r);
  sinp = sin(p);
  siny = sin(y);

  /* this was taken from some code i had on my computer already but i'm sure you
   * can find these formulas on the internet easily */
  matrix_make_identity(m);
  m->el[0][0] = cosy * cosp;
  m->el[0][1] = cosy * sinp * sinr - siny * cosr;
  m->el[0][2] = cosy * sinp * cosr + siny * sinr;
  m->el[1][0] = siny * cosp;
  m->el[1][1] = siny * sinp * sinr + cosy * cosr;
  m->el[1][2] = siny * sinp * cosr - cosy * sinr;
  m->el[2][0] = -sinp;
  m->el[2][1] = cosp * sinr;
  m->el[2][2] = cosp * cosr;
  /* the right column and 4th row are already set correctly by make_identity. */

}


/* multiply two matrices, R = A x B. this can't operate in place, R can't be
 * the same as A or B! */
void matrix_multiply (Matrix *R, const Matrix *A, const Matrix *B) {

  int i, j, k;

  assert(R != A);
  assert(R != B);

  for (i = 0; i < 4; ++ i)
    for (j = 0; j < 4; ++ j) {
      R->el[i][j] = 0.0;
      for (k = 0; k < 4; ++ k)
		R->el[i][j] += A->el[i][k] * B->el[k][j];
    }
}

/* multiply a matrix by a 4-element vector; result is a 4-element vector.
 * R = A * B. this can't operate in place! */
void matrix_vec_multiply (double R[4], const Matrix *A, const double B[4]) {

  int i;

  /* R and B can't be the same pointer */
  assert(R != B);

  /* this is pretty much what you had; just different var names and a for
   * loop instead. */
  for (i = 0; i < 4; ++ i)
    R[i] = (B[0] * A->el[i][0] +
	    B[1] * A->el[i][1] +
	    B[2] * A->el[i][2] +
	    B[3] * A->el[i][3]);

}


/*----------------
 * quad stuff
 *----------------*/


/* create the quad that we'll be using for testing. */
Quad * create_test_quad (void) {

  Quad *q = (Quad *)malloc(sizeof(Quad));

  /* here are the positions (note W=1) */
  vec4_set(q->pt[0], -3.0, -3.0, -3.0,  1.0);
  vec4_set(q->pt[1], -3.0,  3.0, -3.0,  1.0);
  vec4_set(q->pt[2],  3.0,  3.0, -3.0,  1.0);
  vec4_set(q->pt[3],  3.0, -3.0,  1.5,  1.0);

  /* here are the normals (note W=0); they might not be quite right,
   * i made them up. */
  vec4_set(q->n[0],   0.0,  0.0, -1.0,  0.0);
  vec4_set(q->n[1],   0.0,  0.0, -1.0,  0.0);
  vec4_set(q->n[2],   0.0,  0.0, -1.0,  0.0);
  vec4_set(q->n[3],   1.0, -1.0, -1.0,  0.0);
  vec3_normalize(q->n[3]); /* make that last normal unit length. */

  return q;

}


/* q is the input quad to transform.
 * t is the transformed result.
 * frame number is used to calculate angle.
 * cpt is the center point to rotate about; -1 means the origin, 0-3 are
 *   quad point indices. */
void transform_quad (Quad *t, const Quad *q, int frame, int cpt) {

  double roll, pitch, yaw, cx, cy, cz;
  Matrix T1, R, T2, T2_R, T2_R_T1;
  int i;

  /* if you want to change the animation speed; do it here */
  roll  = (double)frame * 0.0010 * M_PI;
  pitch = (double)frame * 0.0009 * M_PI;
  yaw   = (double)frame * 0.0008 * M_PI;

  /* this is the point that the rotation will be about. */
  if (cpt == -1)
    cx = cy = cz = 0.0;
  else {
    cx = q->pt[cpt][0];
    cy = q->pt[cpt][1];
    cz = q->pt[cpt][2];
  }

  /* and now here we construct all the necessary transformations */
  matrix_make_translate(&T1, -cx, -cy, -cz);
  matrix_make_rpy(&R, roll, pitch, yaw);
  matrix_make_translate(&T2, cx, cy, cz);

  /* T = T2 * R * T1 */
  matrix_multiply(&T2_R, &T2, &R);
  matrix_multiply(&T2_R_T1, &T2_R, &T1);

  /* apply transformation to quad vertices and normals note that the
   * vertices will be moved, but the normals will only change dirs,
   * because of the W coordinate values. */
  for (i = 0; i < 4; ++ i) {
    matrix_vec_multiply(t->pt[i], &T2_R_T1, q->pt[i]);
    matrix_vec_multiply(t->n[i], &T2_R_T1, q->n[i]);
  }

}


/*---------------------------------------------
 * just standard gl stuff below here
 *---------------------------------------------*/

Quad *g_oquad = NULL;   /* the original quad */
Quad *g_tquad = NULL;   /* the transformed quad */
int   g_frame = 0;      /* frame number */
int   g_cpt   = -1;     /* center point index */


void glut_reshape (int w, int h) {

  glViewport(0, 0, w, h);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(60.0, (double)w / (double)h, 1.0, 100.0);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  gluLookAt(0.0, 5.0, -20.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

  glEnable(GL_LIGHT0);
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_COLOR_MATERIAL);

  glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);

}


void glut_display (void) {

  int i;

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glEnable(GL_LIGHTING);

  glColor3f(0.0, 0.5, 1.0);

  glBegin(GL_QUADS);

  for (i = 0; i < 4; ++ i) {
    /* note: it's ok that n[] and pt[] have 4 elements. the 3dv call will only
     * use the first 3, which are X,Y,Z. there is a 4dv call but we won't bother
     * with that -- gl doesn't need to know about our W coordinates. */
    glNormal3dv(g_tquad->n[i]);
    glVertex3dv(g_tquad->pt[i]);
  }

  glEnd();

  /* draw the normals too */
/*
  glDisable(GL_LIGHTING);
  glBegin(GL_LINES);
  glColor3f(1.0, 0.0, 0.0);
  for (i = 0; i < 4; ++ i) {
    glVertex3dv(g_tquad->pt[i]);
    glVertex3d(g_tquad->pt[i][0] + 2.0 * g_tquad->n[i][0],
	       g_tquad->pt[i][1] + 2.0 * g_tquad->n[i][1],
	       g_tquad->pt[i][2] + 2.0 * g_tquad->n[i][2]);
  }
  glEnd();
*/
  glutSwapBuffers();

}


void glut_keyboard (unsigned char ch, int x, int y) {

  if (ch == 'q' || ch == 'Q' || ch == 27)
    exit(0);
  else if (ch == '5')
    g_cpt = -1;
  else if (ch >= '1' && ch <= '4')
    g_cpt = ch - '1';

}


void glut_idle (void) {

  ++ g_frame;
  transform_quad(g_tquad, g_oquad, g_frame, g_cpt);
  glutPostRedisplay();

}


void glut_visible (int vis) {

  glutIdleFunc((vis == GLUT_VISIBLE) ? &glut_idle : NULL);

}


int main (int argc, char **argv) {

  g_oquad = create_test_quad();
  g_tquad = create_test_quad(); /* tquad starts the same as oquad */

  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
  glutInitWindowSize(640, 480);
  glutCreateWindow("test");
  glutReshapeFunc(&glut_reshape);
  glutDisplayFunc(&glut_display);
  glutKeyboardFunc(&glut_keyboard);
  glutVisibilityFunc(&glut_visible);
  glutMainLoop();

  return 0;

}
