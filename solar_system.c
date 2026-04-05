#include <GL/glut.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
GLuint texturaCeu;
GLuint texturaMercurio;
GLuint texturaVenus;
GLuint texturaTerra;
GLuint texturaMarte;
GLuint texturaJupiter;
GLuint texturaSaturno;
GLuint texturaUrano;
GLuint texturaNetuno;
GLuint texturaSol;

// Variaveis para translação de cubos
static float angulo = 0.0f;

// Variaveis de tempo
float t1 = 0.0f;
float t2 = 0.0f;
float t3 = 0, t4 = 0, t5 = 0, t6 = 0, t7 = 0, t8 = 0;

float ajusteVelocidade = 1.0f;

float camAngleX = 0.7f; // inclinação vertical
float camAngleY = 0.0f; // rotação horizontal
float camDistance = 30.0f;

typedef struct
{
    float x, y, z;
} Vec3;

typedef struct
{
    Vec3 *orbita;
    int nOrbita;
    char *nome;

    float r, g, b;
    float t, scale, velocidade;
    GLuint textura;
    float rotX, rotY, rotZ;
} Planeta;

Planeta planetas[8];

Vec3 catmullRom(Vec3 p0, Vec3 p1, Vec3 p2, Vec3 p3, float t)
{
    float t2 = t * t;
    float t3 = t2 * t;

    Vec3 r;

    r.x = 0.5f * ((2 * p1.x) +
                  (-p0.x + p2.x) * t +
                  (2 * p0.x - 5 * p1.x + 4 * p2.x - p3.x) * t2 +
                  (-p0.x + 3 * p1.x - 3 * p2.x + p3.x) * t3);

    r.y = 0.5f * ((2 * p1.y) +
                  (-p0.y + p2.y) * t +
                  (2 * p0.y - 5 * p1.y + 4 * p2.y - p3.y) * t2 +
                  (-p0.y + 3 * p1.y - 3 * p2.y + p3.y) * t3);

    r.z = 0.5f * ((2 * p1.z) +
                  (-p0.z + p2.z) * t +
                  (2 * p0.z - 5 * p1.z + 4 * p2.z - p3.z) * t2 +
                  (-p0.z + 3 * p1.z - 3 * p2.z + p3.z) * t3);

    return r;
}

Vec3 getSplinePoint(Vec3 *pts, int n, float t)
{
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
        localT);
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
    {3, 0, -2}};

int norbitaMercurio = 8;

Vec3 orbitaVenus[] = {
    {8, 0, 0},
    {4, 0, 3},
    {0, 0, 5},
    {-4, 0, 3},
    {-8, 0, 0},
    {-4, 0, -3},
    {0, 0, -5},
    {4, 0, -3}};

int norbitaVenus = 8;

Vec3 orbitaTerra[] = {
    {10, 0, 0},
    {5, 0, 4},
    {0, 0, 6},
    {-5, 0, 4},
    {-10, 0, 0},
    {-5, 0, -4},
    {0, 0, -6},
    {5, 0, -4}};
int nOrbitaTerra = 8;

Vec3 orbitaMarte[] = {
    {12, 0, 0},
    {6, 0, 5},
    {0, 0, 7},
    {-6, 0, 5},
    {-12, 0, 0},
    {-6, 0, -5},
    {0, 0, -7},
    {6, 0, -5}};
int nOrbitaMarte = 8;

Vec3 orbitaJupiter[] = {
    {16, 0, 0},
    {8, 0, 7},
    {0, 0, 10},
    {-8, 0, 7},
    {-16, 0, 0},
    {-8, 0, -7},
    {0, 0, -10},
    {8, 0, -7}};
int nOrbitaJupiter = 8;

Vec3 orbitaSaturno[] = {
    {20, 0, 0},
    {10, 0, 8},
    {0, 0, 12},
    {-10, 0, 8},
    {-20, 0, 0},
    {-10, 0, -8},
    {0, 0, -12},
    {10, 0, -8}};
int nOrbitaSaturno = 8;

Vec3 orbitaUrano[] = {
    {24, 0, 0},
    {12, 0, 10},
    {0, 0, 14},
    {-12, 0, 10},
    {-24, 0, 0},
    {-12, 0, -10},
    {0, 0, -14},
    {12, 0, -10}};
int nOrbitaUrano = 8;

Vec3 orbitaNetuno[] = {
    {28, 0, 0},
    {14, 0, 12},
    {0, 0, 16},
    {-14, 0, 12},
    {-28, 0, 0},
    {-14, 0, -12},
    {0, 0, -16},
    {14, 0, -12}};
int nOrbitaNetuno = 8;

void initPlanets()
{

    // Mercurio
    Planeta mercurio;
    mercurio.nome = "Mercurio";
    mercurio.orbita = orbitaMercurio;
    mercurio.nOrbita = norbitaMercurio;
    mercurio.r = 0.7;
    mercurio.g = 0.47;
    mercurio.b = 0.2;
    mercurio.scale = 1.0f;
    mercurio.t = t1;
    mercurio.velocidade = 0.003f;

    // Venus
    Planeta venus;
    venus.nome = "Venus";
    venus.orbita = orbitaVenus;
    venus.nOrbita = norbitaVenus;
    venus.r = 1;
    venus.g = 0;
    venus.b = 0;
    venus.scale = 0.5;
    venus.t = t2;
    venus.velocidade = 0.002f;

    // Terra
    Planeta terra;
    terra.nome = "Terra";
    terra.orbita = orbitaTerra;
    terra.nOrbita = nOrbitaTerra;
    terra.r = 0.2;
    terra.g = 0.5;
    terra.b = 1.0;
    terra.scale = 0.6;
    terra.t = t3;
    terra.velocidade = 0.0015f;

    // Marte
    Planeta marte;
    marte.nome = "Marte";
    marte.orbita = orbitaMarte;
    marte.nOrbita = nOrbitaMarte;
    marte.r = 0.8;
    marte.g = 0.3;
    marte.b = 0.3;
    marte.scale = 0.45;
    marte.t = t4;
    marte.velocidade = 0.0012f;

    // jupiter
    Planeta jupiter;
    jupiter.nome = "Jupiter";
    jupiter.orbita = orbitaJupiter;
    jupiter.nOrbita = nOrbitaJupiter;
    jupiter.r = 0.8;
    jupiter.g = 0.5;
    jupiter.b = 0.5;
    jupiter.scale = 0.9;
    jupiter.t = t5;
    jupiter.velocidade = 0.0008f;

    // Saturno
    Planeta saturno;
    saturno.nome = "Saturno";
    saturno.orbita = orbitaSaturno;
    saturno.nOrbita = nOrbitaSaturno;
    saturno.r = 0.6;
    saturno.g = 0.5;
    saturno.b = 0.5;
    saturno.scale = 0.9;
    saturno.t = t6;
    saturno.velocidade = 0.0006f;

    // Urano
    Planeta urano;
    urano.nome = "Urano";
    urano.orbita = orbitaUrano;
    urano.nOrbita = nOrbitaUrano;
    urano.r = 0.0;
    urano.g = 0.4;
    urano.b = 0.8;
    urano.scale = 0.8;
    urano.t = t7;
    urano.velocidade = 0.0004f;

    // Netuno
    Planeta netuno;
    netuno.nome = "Netuno";
    netuno.orbita = orbitaNetuno;
    netuno.nOrbita = nOrbitaNetuno;
    netuno.r = 0.0;
    netuno.g = 0.4;
    netuno.b = 0.95;
    netuno.scale = 0.7;
    netuno.t = t8;
    netuno.velocidade = 0.0003f;

    // Adicionando Texturas
    mercurio.textura = 0;
    venus.textura = 0;
    terra.textura = 0;
    marte.textura = 0;
    jupiter.textura = 0;
    saturno.textura = 0;
    urano.textura = 0;
    netuno.textura = 0;

    // Rotacoes para ajeitar a textura
    mercurio.rotX = mercurio.rotY = mercurio.rotZ = 0.0f;
    venus.rotX = venus.rotY = venus.rotZ = 0.0f;
    marte.rotX = marte.rotY = marte.rotZ = 0.0f;
    jupiter.rotY = jupiter.rotZ = 0.0f;
    saturno.rotY = saturno.rotZ = 0.0f;
    urano.rotX = urano.rotY = urano.rotZ = 0.0f;
    netuno.rotY = netuno.rotZ = 0.0f;
    jupiter.rotX = 90.0f;
    saturno.rotX = 90.0f;
    netuno.rotX = 90.0f;
    terra.rotX = 0.0f;
    terra.rotY = 90.0f;
    terra.rotY = 0.0f;
    // Adicionando a variavel global
    planetas[0] = mercurio;
    planetas[1] = venus;
    planetas[2] = terra;
    planetas[3] = marte;
    planetas[4] = jupiter;
    planetas[5] = saturno;
    planetas[6] = urano;
    planetas[7] = netuno;
}

void drawText(float x, float y, const char *text)
{
    glRasterPos2f(x, y);

    for (const char *c = text; *c != '\0'; c++)
    {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, *c);
    }
}

void drawLabel3D(Vec3 pos, const char *nome)
{
    GLdouble model[16], proj[16];
    GLint view[4];

    glGetDoublev(GL_MODELVIEW_MATRIX, model);
    glGetDoublev(GL_PROJECTION_MATRIX, proj);
    glGetIntegerv(GL_VIEWPORT, view);

    GLdouble sx, sy, sz;

    gluProject(pos.x, pos.y, pos.z,
               model, proj, view,
               &sx, &sy, &sz);

    // Converter para coordenada OpenGL (-1 a 1)
    float x = (sx / view[2]) * 2.0f - 1.0f;
    float y = (sy / view[3]) * 2.0f - 1.0f;

    // Mudar para modo 2D
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(-1, 1, -1, 1);

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    glColor3f(1, 1, 1);
    drawText(x, y + 0.05f, nome);

    // Restaurar
    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
}

void renderPlanet(Planeta planet)
{
    Vec3 pos1 = getSplinePoint(planet.orbita, planet.nOrbita, planet.t);

    glPushMatrix();
    glTranslatef(pos1.x, pos1.y, pos1.z);
    glScalef(planet.scale, planet.scale, planet.scale);

    if (planet.textura != 0)
    {
        GLUquadric *quad = gluNewQuadric();
        gluQuadricTexture(quad, GL_TRUE);
        gluQuadricNormals(quad, GLU_SMOOTH);

        glEnable(GL_COLOR_MATERIAL);
        glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

        glRotatef(planet.rotX, 1.0f, 0.0f, 0.0f);
        glRotatef(planet.rotY, 0.0f, 1.0f, 0.0f);
        glRotatef(planet.rotZ, 0.0f, 0.0f, 1.0f);

        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, planet.textura);
        glColor3f(1.0f, 1.0f, 1.0f);

        gluSphere(quad, 1.0, 30, 30);

        glDisable(GL_TEXTURE_2D);
        gluDeleteQuadric(quad);
    }
    else
    {
        glColor3f(planet.r, planet.g, planet.b);
        glutSolidSphere(1.0, 16, 16);
    }
    glPopMatrix();

    drawLabel3D(pos1, planet.nome);
}

GLuint carregarTexturaPPM(const char *filename)
{
    FILE *fp = fopen(filename, "rb");
    if (!fp)
    {
        printf("Erro ao abrir textura: %s\n", filename);
        return 0;
    }

    char format[3];
    int width, height, maxval;

    fscanf(fp, "%2s", format);

    if (format[0] != 'P' || format[1] != '6')
    {
        printf("Formato inválido (use P6)\n");
        fclose(fp);
        return 0;
    }

    fscanf(fp, "%d %d", &width, &height);
    fscanf(fp, "%d", &maxval);
    fgetc(fp); // pula \n

    unsigned char *data = (unsigned char *)malloc(3 * width * height);
    fread(data, 3, width * height, fp);
    fclose(fp);

    GLuint texID;
    glGenTextures(1, &texID);
    glBindTexture(GL_TEXTURE_2D, texID);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0,
                 GL_RGB, GL_UNSIGNED_BYTE, data);

    free(data);
    return texID;
}

void drawSkySphere(float raio)
{
    GLUquadric *quad = gluNewQuadric();
    gluQuadricTexture(quad, GL_TRUE);

    glPushAttrib(GL_ENABLE_BIT | GL_DEPTH_BUFFER_BIT);

    glDisable(GL_LIGHTING);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texturaCeu);

    glDepthMask(GL_FALSE);

    glPushMatrix();
    glScalef(-1.0f, 1.0f, 1.0f); // desenha por dentro
    glColor3f(1.0f, 1.0f, 1.0f);
    gluSphere(quad, raio, 50, 50);
    glPopMatrix();

    glDepthMask(GL_TRUE);
    glPopAttrib();

    gluDeleteQuadric(quad);
}

void drawSun(float raio)
{
    GLUquadric *quad = gluNewQuadric();
    gluQuadricTexture(quad, GL_TRUE);
    gluQuadricNormals(quad, GLU_SMOOTH);

    GLfloat emissao[] = {1.0f, 1.0f, 1.0f, 1.0f};
    GLfloat sem_emissao[] = {0.0f, 0.0f, 0.0f, 1.0f};

    glMaterialfv(GL_FRONT, GL_EMISSION, emissao);

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texturaSol);

    glPushMatrix();
    gluSphere(quad, raio, 50, 50);
    glPopMatrix();

    glDisable(GL_TEXTURE_2D);

    // IMPORTANTE: resetar emissão
    glMaterialfv(GL_FRONT, GL_EMISSION, sem_emissao);

    gluDeleteQuadric(quad);
}

void configurarLuz()
{
    GLfloat posicao[] = {0.0f, 0.0f, 0.0f, 1.0f}; // luz pontual

    GLfloat ambiente[] = {0.0f, 0.0f, 0.0f, 1.0f};
    GLfloat difusa[] = {1.0f, 1.0f, 1.0f, 1.0f};
    GLfloat especular[] = {1.0f, 1.0f, 1.0f, 1.0f};

    glLightfv(GL_LIGHT0, GL_POSITION, posicao);
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambiente);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, difusa);
    glLightfv(GL_LIGHT0, GL_SPECULAR, especular);

    // Atenuação (importante pra realismo)
    glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, 1.0f);
    glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, 0.02f);
    glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, 0.001f);
}

void init(void)
{
    glClearColor(0.0, 0.0, 0.0, 0.0);
    initPlanets();
    glEnable(GL_DEPTH_TEST);

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_NORMALIZE);

    glShadeModel(GL_SMOOTH);

    texturaCeu = carregarTexturaPPM("stars.ppm");
    texturaMercurio = carregarTexturaPPM("mercury.ppm");
    texturaVenus = carregarTexturaPPM("venus.ppm");
    texturaTerra = carregarTexturaPPM("earth.ppm");
    texturaMarte = carregarTexturaPPM("mars.ppm");
    texturaJupiter = carregarTexturaPPM("jupiter.ppm");
    texturaSaturno = carregarTexturaPPM("saturn.ppm");
    texturaUrano = carregarTexturaPPM("uranus.ppm");
    texturaNetuno = carregarTexturaPPM("neptune.ppm");

    planetas[0].textura = texturaMercurio;
    planetas[1].textura = texturaVenus;
    planetas[2].textura = texturaTerra;
    planetas[3].textura = texturaMarte;
    planetas[4].textura = texturaJupiter;
    planetas[5].textura = texturaSaturno;
    planetas[6].textura = texturaUrano;
    planetas[7].textura = texturaNetuno;

    texturaSol = carregarTexturaPPM("sun.ppm");

    if (texturaSol == 0)
        printf("Erro ao carregar sun.ppm\n");

    if (texturaCeu == 0)
        printf("Erro ao carregar stars.ppm\n");
    if (texturaMercurio == 0)
        printf("Erro ao carregar mercury.ppm\n");
    if (texturaVenus == 0)
        printf("Erro ao carregar venus.ppm\n");
    if (texturaTerra == 0)
        printf("Erro ao carregar earth.ppm\n");
    if (texturaMarte == 0)
        printf("Erro ao carregar mars.ppm\n");
    if (texturaJupiter == 0)
        printf("Erro ao carregar jupiter.ppm\n");
    if (texturaSaturno == 0)
        printf("Erro ao carregar saturn.ppm\n");
    if (texturaUrano == 0)
        printf("Erro ao carregar uranus.ppm\n");
    if (texturaNetuno == 0)
        printf("Erro ao carregar neptune.ppm\n");
}

void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    float camX = camDistance * sin(camAngleY) * cos(camAngleX);
    float camY = camDistance * sin(camAngleX);
    float camZ = camDistance * cos(camAngleY) * cos(camAngleX);
    gluLookAt(camX, camY, camZ, // lookFrom
              0.0, 0.0, 0.0,    // lookAt
              0.0, 1.0, 0.0);   // Up

    configurarLuz();

    drawSkySphere(100.0f);

    glPushMatrix();
    drawSun(1.0f);
    glPopMatrix();

    for (int i = 0; i < 8; i++)
        renderPlanet(planetas[i]);
    glFlush();
    glutSwapBuffers();
}

void reshape(int w, int h)
{
    glViewport(0, 0, (GLsizei)w, (GLsizei)h);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    gluPerspective(60.0, (float)w / (float)h, 1.0, 200.0);

    glMatrixMode(GL_MODELVIEW);
}

void keyboard(unsigned char key, int x, int y)
{
    (void)x;
    (void)y;
    const float step = 0.33f;

    switch (key)
    {
    case 27:
        exit(0);
        break;

    case '[':
        ajusteVelocidade *= 0.8f; // desacelera
        if (ajusteVelocidade < 0.1f)
            ajusteVelocidade = 0.1f;
        break;

    case ']':
        ajusteVelocidade *= 1.2f; // acelera
        break;

    case '+':
        camDistance -= 1.0f;
        break;

    case '-':
        camDistance += 1.0f;
        break;

    case 'r':
        ajusteVelocidade = 1;
        camDistance = 30;
        camAngleX = 0.7;
        camAngleY = 0;
        break;
    default:
        return;
    }

    glutPostRedisplay();
}

void specialKeys(int key, int x, int y)
{
    float step = 0.05f;

    switch (key)
    {
    case GLUT_KEY_LEFT:
        camAngleY -= step;
        break;

    case GLUT_KEY_RIGHT:
        camAngleY += step;
        break;

    case GLUT_KEY_UP:
        camAngleX += step;
        if (camAngleX > 1.5f)
            camAngleX = 1.5f;
        break;

    case GLUT_KEY_DOWN:
        camAngleX -= step;
        if (camAngleX < -1.5f)
            camAngleX = -1.5f;
        break;
    }

    glutPostRedisplay();
}

void update(int valor)
{

    for (int i = 0; i < 8; i++)
    {
        planetas[i].t += planetas[i].velocidade * ajusteVelocidade;
        if (planetas[i].t >= 1.0f)
            planetas[i].t = 0;
    }

    glutPostRedisplay();
    glutTimerFunc(16, update, 0);
}

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(100, 100);
    glutCreateWindow(argv[0]);
    init();
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutTimerFunc(0, update, 0);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(specialKeys);
    glutMainLoop();
    return 0;
}
