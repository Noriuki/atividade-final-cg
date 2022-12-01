#include <windows.h>
#include <gl/glut.h>
#include <math.h>
#include <iostream>
#include <string>
#include<gl/GLU.h>
#include <direct.h>
#include <sstream>
#include "stb_image.h"
#define GetCurrentDir _getcwd

GLfloat x_ball = 100.0f;
GLfloat y_ball = 150.0f;
GLsizei size_ball = 10;

GLuint texID;
GLuint bgText;
GLfloat y_retangles;
GLfloat size_retangles = 20;

GLfloat xstep = 1.0f;
GLfloat ystep = 1.0f;

// Largura e altura da janela
GLfloat windowWidth;
GLfloat windowHeight;
// Largura e altura limite da janela
GLfloat windowWidthLimit;
GLfloat windowHeightLimit;

int life = 3;
int gameStarted = 0;

// GET WORKING DIR
std::string get_current_dir() {
  char buff[FILENAME_MAX];
  GetCurrentDir( buff, FILENAME_MAX);
  std::string current_working_dir(buff);
  return current_working_dir;
}

void gameReset() {
  x_ball = 100.0f;
  y_ball = 150.0f;
  xstep = 1.0f;
  ystep = 1.0f;
  gameStarted = 0;
}

// LOAD TEXTURE
void carregaTextura(GLuint tex_id, std::string filePath)
{
	unsigned char* imgData;
    int largura, altura, canais;
  std::string filePathString = get_current_dir() + "\\" + filePath;
  stbi_set_flip_vertically_on_load(1);

	imgData = stbi_load(filePathString.c_str(), &largura, &altura, &canais, 4);

	if (imgData)
	{
		glBindTexture(GL_TEXTURE_2D, tex_id);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, largura, altura, 0, GL_RGBA, GL_UNSIGNED_BYTE, imgData);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		stbi_image_free(imgData);
	}	else {
    if(stbi_failure_reason())
    std::cout << stbi_failure_reason();

		std::cout << "ERRO:: Nao foi possivel carregar a textura!" << filePathString.c_str() << std::endl;
	}
}

void cube () {
    glBindTexture( GL_TEXTURE_2D, bgText ); //bind the texture

    glBegin( GL_QUADS );
        glTexCoord2d(0.0,0.0); glVertex2i(0,windowHeight);
        glTexCoord2d(1.0,0.0); glVertex2i(0,0);
        glTexCoord2d(1.0,1.0); glVertex2i(windowWidth,0);
        glTexCoord2d(0.0,1.0); glVertex2i(windowWidth,windowHeight);
    glEnd();

    glBindTexture(GL_TEXTURE_2D, 0);
}

// DRAW RENTAGLE
void DrawRectangle(float rx, float ry, float width, float height) {
    glBindTexture(GL_TEXTURE_2D, texID);

    glBegin(GL_QUADS);
        glTexCoord2f(0.0, 0.0); glVertex2i(rx,ry);
        glTexCoord2f(1.0, 0.0); glVertex2i(rx+width,ry);
        glTexCoord2f(1.0, 1.0); glVertex2i(rx+width,ry+height);
        glTexCoord2f(0.0, 1.0); glVertex2i(rx,ry+height);
    glEnd();

    glBindTexture(GL_TEXTURE_2D, 0);
}

// DRAW CIRCLE
void DrawCircle(float cx, float cy, float cr, int num_segments) {
    glBegin(GL_POLYGON);
    for (int ii = 0; ii < num_segments; ii++)   {
        float theta = 2.0f * 3.1415926f * float(ii) / float(num_segments);
        float x = cr * cosf(theta);
        float y = cr * sinf(theta);
        glVertex2f(x + cx, y + cy);
    }
    glEnd();
}

// DRAW SQUARE
void DrawSquare(float sx, float sy, float sr) {
    glBegin(GL_QUADS);
        glVertex2i(sx,sy+sr);
        glVertex2i(sx,sy);
        glVertex2i(sx+sr,sy);
        glVertex2i(sx+sr,sy+sr);
    glEnd();
}

// callback chamada para fazer o desenho
void Desenha(void)
{
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if(gameStarted == 1) {
        glRasterPos2f(0, windowHeightLimit);
        glColor4f(1.0f, 1.0f, 1.0f, 0.0f);
        // DISPLAY LIFE
        char str[100];
        sprintf(str, "VIDAS: %d", life);
        char *string = str;
        while(*string){
            glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *string++);
        }
        // DRAW GAME OBJECTS
        glColor4f(1.0f, 0.0f, 1.0f, 0.0f);
        DrawCircle(x_ball, y_ball, size_ball, 20);
        DrawRectangle(windowWidthLimit, y_retangles, size_retangles, windowHeightLimit / 4);
        DrawRectangle(0, y_retangles, size_retangles, windowHeightLimit / 4);
    } else {

        glRasterPos2f((windowWidth / 2 - 40), windowHeight / 2);

        // DISPLAY LIFE
        char str[100];
        sprintf(str, "NEW GAME [PRESS F1]");
        char *string = str;
        while(*string){
            glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *string++);
        }

    }
    cube();
    glutSwapBuffers();
}

bool CheckCollision()
{
    // COLISSAO SE O X DA BOLA FOR MAIOR OU IGUAL AO RENTANGULO E O Y DA BOLA ESTIVER ENTRE A ALTURA DO RETANGULO
    bool collission_1 = x_ball < size_retangles  && (y_ball > y_retangles && y_ball < y_retangles + windowHeightLimit / 4);
    bool collission_2 = x_ball > windowWidthLimit && (y_ball > y_retangles && y_ball < y_retangles + windowHeightLimit / 4);
    // collision if on both axes
    return collission_1 || collission_2;
}
// x do circulo menor que posicao do retangulo

// funcao callback chamada pela GLUT a cada intervalo de tempo
// (a window nï¿½o estï¿½ sendo redimensionada ou movida)
void Timer(int value)
{
    bool collission = CheckCollision();
    if (collission) {
      xstep = xstep > 0 ? (xstep + 1) * -1 : (xstep - 1) * -1;
      ystep = ystep > 0 ? (ystep + 1) * -1 : (ystep - 1) * -1;

    } else if (x_ball > windowWidth - size_ball || x_ball < 0) {
        xstep = -xstep;
        glColor3f(1.0f,0.0f,0.0f);
        if(life > 1) {
            life -= 1;
        } else {
            gameReset();
        }
    } else if (y_ball > windowHeight-size_ball || y_ball < 0) {
        ystep = -ystep;
    }

    if(x_ball > windowWidth-size_ball)
        x_ball = windowWidth-size_ball-1;

    if(y_ball > windowHeight-size_ball)
        y_ball = windowHeight-size_ball-1;

    // Move a bola
    x_ball += xstep;
    y_ball += ystep;
    // Redesenha a bola com as novas coordenadas
    glutPostRedisplay();
    glutTimerFunc(33,Timer, 1);
}

// Inicializa parï¿½metros de rendering
void Inicializa (void)
{
    // Define a cor de fundo da janela de visualizaï¿½ï¿½o como preta
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
}

// Funï¿½ï¿½o callback chamada quando o tamanho da janela ï¿½ alterado
void AlteraTamanhoJanela(GLsizei w, GLsizei h)
{
    // Evita a divisao por zero
    if(h == 0) h = 1;

    // Especifica as dimensï¿½es da Viewport
    glViewport(0, 0, w, h);

    // Inicializa o sistema de coordenadas
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    // Estabelece a janela de seleï¿½ï¿½o (left, right, bottom, top)
    if (w <= h)  {
        windowHeight = 250.0f*h/w;
        windowWidth = 250.0f;
    }
    else  {
        windowWidth = 250.0f*w/h;
        windowHeight = 250.0f;
    }
    windowWidthLimit = windowWidth - size_retangles;
    windowHeightLimit = windowHeight - size_retangles;
    y_retangles = windowHeightLimit / 2;
    gluOrtho2D(0.0f, windowWidth, 0.0f, windowHeight);
}

void handleSpecialKeypress(int key, int x, int y) {
    switch (key) {
        case GLUT_KEY_UP:
          if(y_retangles + windowHeightLimit / 4 < windowHeight) {
            y_retangles += 4;
          }
        break;

        case GLUT_KEY_DOWN:
          if(y_retangles > 0) {
            y_retangles -= 4;
          }
        break;

        case GLUT_KEY_F1:
          if(gameStarted == 0) {
            life = 3;
            gameStarted = 1;
          }

        break;
    }
}
void init()
{
	glEnable(GL_DEPTH_TEST);


    // LOAD TEXTURES
	  glEnable(GL_TEXTURE_2D);
	  glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);



    glGenTextures(1, &bgText);
    carregaTextura(bgText, "game-background.jpg");

    // PLAY BACKGROUND SONG
    sndPlaySound("bg-sound.wav", SND_LOOP | SND_ASYNC);

    glGenTextures(1, &texID);
    carregaTextura(texID, "neon-brick.jpg");
}

// Programa Principal
int main(void)
{
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800,800);
    glutInitWindowPosition(0,0);
    glutCreateWindow("Avaliacao Pratica Lane Zupiroli");

    init();

    glutDisplayFunc(Desenha);
    glutReshapeFunc(AlteraTamanhoJanela);
    glutSpecialFunc(handleSpecialKeypress);
    glutTimerFunc(33, Timer, 1);
    Inicializa();
    glutMainLoop();
}
