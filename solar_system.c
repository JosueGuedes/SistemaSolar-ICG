#include <GL/glut.h>
#include <stdlib.h>
#include <stdio.h>

// Variaveis para translação de cubos
static float angulo = 0.0f;

// Variaveis de tempo
float t1 = 0.0f;
float t2 = 0.0f;
float t3=0, t4=0, t5=0, t6=0, t7=0, t8=0;


typedef struct {
    float x, y, z;
} Vec3;

Vec3 catmullRom(Vec3 p0, Vec3 p1, Vec3 p2, Vec3 p3, float t) {
    float t2 = t * t;
    float t3 = t2 * t;

    Vec3 r;

    r.x = 0.5f * ((2*p1.x) +
        (-p0.x + p2.x)*t +
        (2*p0.x - 5*p1.x + 4*p2.x - p3.x)*t2 +
        (-p0.x + 3*p1.x - 3*p2.x + p3.x)*t3);

    r.y = 0.5f * ((2*p1.y) +
        (-p0.y + p2.y)*t +
        (2*p0.y - 5*p1.y + 4*p2.y - p3.y)*t2 +
        (-p0.y + 3*p1.y - 3*p2.y + p3.y)*t3);

    r.z = 0.5f * ((2*p1.z) +
        (-p0.z + p2.z)*t +
        (2*p0.z - 5*p1.z + 4*p2.z - p3.z)*t2 +
        (-p0.z + 3*p1.z - 3*p2.z + p3.z)*t3);

    return r;
}

Vec3 getSplinePoint(Vec3* pts, int n, float t) {
    float total = t * n;
    int i = (int)total;
    float localT = total - i;

    int i0 = (i - 1 + n) % n;
    int i1 = i % n;
    int i2 = (i + 1) % n;
    int i3 = (i + 2) % n;

    return catmullRom(
        pts[i0],
        pts[i1],
        pts[i2],
        pts[i3],
        localT
    );
}

// ----------------- PONTOS DE CONTROLE DO MOVIMENTO DOS ASTROS --------------
Vec3 orbitaMercurio[] = {
    {6, 0, 0},
    {3, 0, 2},
    {0, 0, 3},
    {-3, 0, 2},
    {-6, 0, 0},
    {-3, 0, -2},
    {0, 0, -3},
    {3, 0, -2}
};

int norbitaMercurio = 8;

Vec3 orbitaVenus[] = {
    {8, 0, 0},
    {4, 0, 3},
    {0, 0, 5},
    {-4, 0, 3},
    {-8, 0, 0},
    {-4, 0, -3},
    {0, 0, -5},
    {4, 0, -3}
};

int norbitaVenus = 8;

Vec3 orbitaTerra[] = {
    {10, 0, 0},
    {5, 0, 4},
    {0, 0, 6},
    {-5, 0, 4},
    {-10, 0, 0},
    {-5, 0, -4},
    {0, 0, -6},
    {5, 0, -4}
};
int nOrbitaTerra = 8;

Vec3 orbitaMarte[] = {
    {12, 0, 0},
    {6, 0, 5},
    {0, 0, 7},
    {-6, 0, 5},
    {-12, 0, 0},
    {-6, 0, -5},
    {0, 0, -7},
    {6, 0, -5}
};
int nOrbitaMarte = 8;

Vec3 orbitaJupiter[] = {
    {16, 0, 0},
    {8, 0, 7},
    {0, 0, 10},
    {-8, 0, 7},
    {-16, 0, 0},
    {-8, 0, -7},
    {0, 0, -10},
    {8, 0, -7}
};
int nOrbitaJupiter = 8;

Vec3 orbitaSaturno[] = {
    {20, 0, 0},
    {10, 0, 8},
    {0, 0, 12},
    {-10, 0, 8},
    {-20, 0, 0},
    {-10, 0, -8},
    {0, 0, -12},
    {10, 0, -8}
};
int nOrbitaSaturno = 8;

void init(void) 
{
   glClearColor (0.0, 0.0, 0.0, 0.0);
   glShadeModel (GL_FLAT);
   glEnable(GL_DEPTH_TEST);
}

void display(void)
{
   glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   glLoadIdentity ();             
   gluLookAt ( 0.0, 7.0, 30.0,  // lookFrom
               0.0, 0.0, 0.0,  // lookAt
               0.0, 1.0, 0.0); // Up

   glPushMatrix();
      glColor3f (1.0, 0.0, 0.0);
      glScalef (1.0, 1.0, 1.0);      /* modeling transformation */ 
      glutSolidSphere (1.0, 16, 16);
   glPopMatrix();

   // Primeiro astro - Mercurio
   Vec3 pos1 = getSplinePoint(orbitaMercurio, norbitaMercurio, t1);

   glPushMatrix();
      glTranslatef(pos1.x, pos1.y, pos1.z);
      glColor3f (0.7, 0.47, 0.2);
      glScalef (0.5, 0.5, 0.5);
      glutSolidSphere (1.0, 16, 16);
   glPopMatrix();

   // Segundo astro = venus
   Vec3 pos2 = getSplinePoint(orbitaVenus, norbitaVenus, t2);

   glPushMatrix();
      glTranslatef(pos2.x, pos2.y, pos2.z);
      glColor3f (0.0, 1.0, 0.0);
      glScalef (0.5, 0.5, 0.5);
      glutSolidSphere (1.0, 16, 16);
   glPopMatrix();

   // Terceiro astro - Terra
   Vec3 posTerra = getSplinePoint(orbitaTerra, nOrbitaTerra, t3);

   glPushMatrix();
      glTranslatef(posTerra.x, posTerra.y, posTerra.z);
      glColor3f(0.2, 0.5, 1.0);
      glScalef(0.6, 0.6, 0.6);
      glutSolidSphere(1.0, 16, 16);
   glPopMatrix();

   // Quarto astro - Marte
   Vec3 posMarte = getSplinePoint(orbitaMarte, nOrbitaMarte, t4);

   glPushMatrix();
      glTranslatef(posMarte.x, posMarte.y, posMarte.z);
      glColor3f(0.8, 0.3, 0.3);
      glScalef(0.45, 0.45, 0.45);
      glutSolidSphere(1.0, 16, 16);
   glPopMatrix();

   // Quinto astro - Jupiter
   Vec3 posJupiter = getSplinePoint(orbitaJupiter, nOrbitaJupiter, t5);

   glPushMatrix();
      glTranslatef(posJupiter.x, posJupiter.y, posJupiter.z);
      glColor3f(0.8, 0.5, 0.5);
      glScalef(0.9, 0.9, 0.9);
      glutSolidSphere(1.0, 16, 16);
   glPopMatrix();

   // Sexto astro - Saturno
   Vec3 posSaturno = getSplinePoint(orbitaSaturno, nOrbitaSaturno, t6);
   glPushMatrix();
      glTranslatef(posSaturno.x, posSaturno.y, posSaturno.z);
      glColor3f(0.6, 0.5, 0.5);
      glScalef(0.9, 0.9, 0.9);
      glutSolidSphere(1.0, 16, 16);
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

    t1 += 0.003f;
    t2 += 0.002f;
    t3 += 0.0015f;
    t4 += 0.0012f;
    t5 += 0.0008f;
    t6 += 0.0006f;
    t7 += 0.0004f;
    t8 += 0.0003f;
   float progressoTempos[8] = {t1, t2, t3, t4, t5, t6, t7, t8};

    for (int i = 0 ; i < 8 ; i++){
      if(progressoTempos[i] >= 1.0f) progressoTempos[i] = 0.0;
    }

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
