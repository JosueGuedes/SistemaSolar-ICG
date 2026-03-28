#include <GL/glut.h>
#include <stdlib.h>
#include <stdio.h>

// Variaveis para translação de cubos
static float angulo = 0.0f;

void init(void) 
{
   glClearColor (0.0, 0.0, 0.0, 0.0);
   glShadeModel (GL_FLAT);
   glEnable(GL_DEPTH_TEST);
}

void display(void)
{
   glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   glLoadIdentity ();             /* clear the matrix */
   gluLookAt ( 0.0, 7.0, 15.0,  // lookFrom
               0.0, 0.0, 0.0,  // lookAt
               0.0, 1.0, 0.0); // Up

   glPushMatrix();
      glColor3f (1.0, 0.0, 0.0);
      glScalef (1.0, 1.0, 1.0);      /* modeling transformation */ 
      glutSolidSphere (1.0, 16, 16);
   glPopMatrix();

   glPushMatrix();
      glRotatef(angulo, 0, 1, 0);
      glColor3f (1.0, 0.67, 0.0);
      glTranslatef(0, 0, -3.0);
      glScalef (0.5, 0.5, 0.5);      /* modeling transformation */ 
      glutSolidSphere (1.0, 16, 16);
   glPopMatrix();

   glPushMatrix();
      glRotatef(angulo, 0, 1, 1);
      glColor3f (0.0, 1.0, 0.0);
      glTranslatef(-5, 0, 0.0);
      glScalef (0.5, 0.5, 0.5);      /* modeling transformation */ 
      glutSolidSphere (1.0, 16, 16);
   glPopMatrix();

   glFlush ();
   glutSwapBuffers();
}

void reshape (int w, int h)
{
   glViewport (0, 0, (GLsizei) w, (GLsizei) h); 

   glMatrixMode (GL_PROJECTION);
   glLoadIdentity ();

   gluPerspective(60.0, (float)w/(float)h, 1.0, 50.0);

   glMatrixMode (GL_MODELVIEW);
}

void keyboard(unsigned char key, int x, int y)
{
   (void)x; (void)y;
   const float step = 0.33f;

   switch (key) {

      case 27:
         exit(0);
         break;
      default: return;
   }

   glutPostRedisplay();
}

void update(int valor){
    angulo += 1.0f;
    if (angulo > 360)
        angulo = 0;
    glutPostRedisplay();
    glutTimerFunc(16, update, 0);
}

int main(int argc, char** argv)
{
   glutInit(&argc, argv);
   glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
   glutInitWindowSize (500, 500); 
   glutInitWindowPosition (100, 100);
   glutCreateWindow (argv[0]);
   init ();
   glutDisplayFunc(display); 
   glutReshapeFunc(reshape);
   glutTimerFunc(0, update, 0);
   glutKeyboardFunc(keyboard);
   glutMainLoop();
   return 0;
}
