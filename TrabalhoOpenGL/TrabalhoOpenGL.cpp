// Iluminacao.c - Isabel H. Manssour
// Um programa OpenGL que exemplifica a visualização 
// de objetos 3D com a inserção de uma fonte de luz.
// Este código está baseado nos exemplos disponíveis no livro 
// "OpenGL SuperBible", 2nd Edition, de Richard S. e Wright Jr.

#include <gl/glut.h>
#include <iostream>

#include "csvReader.h"

GLfloat angle, fAspect;
std::vector<std::vector<int>> myMap;

void fps()
{
	static float fps = 0.0f;
	static float before = 0.0f;
	static char strFPS[20] = { 0 };
	static float now = (glutGet(GLUT_ELAPSED_TIME));

	++fps;

	if (now - before>1.0f)
	{
		before = now;
		std::cout << "FPS:" << int(fps) << std::endl;
		fps = 0.0f;
	}
}

void DrawCube(double cubeCoord_x, double cubeCoord_y, double cubeCoord_z, double width, double length, double height)
{
	// Desenha um cubo

	glBegin(GL_QUADS);			// Face posterior
	glNormal3f(0.0, 0.0, 1.0);	// Normal da face
	glVertex3f(cubeCoord_x + width, cubeCoord_y + length, cubeCoord_z + height);
	glVertex3f(cubeCoord_x, cubeCoord_y + length, cubeCoord_z + height);
	glVertex3f(cubeCoord_x, cubeCoord_y, cubeCoord_z + height);
	glVertex3f(cubeCoord_x + width, cubeCoord_y, cubeCoord_z + height);
	glEnd();

	glBegin(GL_QUADS);			// Face frontal
	glNormal3f(0.0, 0.0, -1.0); 	// Normal da face
	glVertex3f(cubeCoord_x + width, cubeCoord_y + length, cubeCoord_z);
	glVertex3f(cubeCoord_x + width, cubeCoord_y, cubeCoord_z);
	glVertex3f(cubeCoord_x, cubeCoord_y, cubeCoord_z);
	glVertex3f(cubeCoord_x, cubeCoord_y + length, cubeCoord_z);
	glEnd();

	glBegin(GL_QUADS);			// Face lateral esquerda
	glNormal3f(-1.0, 0.0, 0.0); 	// Normal da face
	glVertex3f(cubeCoord_x, cubeCoord_y + length, cubeCoord_z + height);
	glVertex3f(cubeCoord_x, cubeCoord_y + length, cubeCoord_z);
	glVertex3f(cubeCoord_x, cubeCoord_y, cubeCoord_z);
	glVertex3f(cubeCoord_x, cubeCoord_y, cubeCoord_z + height);
	glEnd();

	glBegin(GL_QUADS);			// Face lateral direita
	glNormal3f(1.0, 0.0, 0.0);	// Normal da face
	glVertex3f(cubeCoord_x + width, cubeCoord_y + length, cubeCoord_z + height);
	glVertex3f(cubeCoord_x + width, cubeCoord_y, cubeCoord_z + height);
	glVertex3f(cubeCoord_x + width, cubeCoord_y, cubeCoord_z);
	glVertex3f(cubeCoord_x + width, cubeCoord_y + length, cubeCoord_z);
	glEnd();

	glBegin(GL_QUADS);			// Face superior
	glNormal3f(0.0, 1.0, 0.0);  	// Normal da face
	glVertex3f(cubeCoord_x, cubeCoord_y + length, cubeCoord_z);
	glVertex3f(cubeCoord_x, cubeCoord_y + length, cubeCoord_z + height);
	glVertex3f(cubeCoord_x + width, cubeCoord_y + length, cubeCoord_z + height);
	glVertex3f(cubeCoord_x + width, cubeCoord_y + length, cubeCoord_z);
	glEnd();

	glBegin(GL_QUADS);			// Face inferior
	glNormal3f(0.0, -1.0, 0.0); 	// Normal da face
	glVertex3f(cubeCoord_x, cubeCoord_y, cubeCoord_z);
	glVertex3f(cubeCoord_x + width, cubeCoord_y, cubeCoord_z);
	glVertex3f(cubeCoord_x + width, cubeCoord_y, cubeCoord_z + height);
	glVertex3f(cubeCoord_x, cubeCoord_y, cubeCoord_z + height);
	glEnd();
}

// Função callback chamada para fazer o desenho
void Desenha(void)
{

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glColor3f(0.0f, 0.0f, 1.0f);

	int cubeWidth(10);
	int cubeLength(10);

	int line(0);
	for (std::vector<std::vector<int>>::iterator lineIterator = myMap.begin(); lineIterator != myMap.end(); ++lineIterator)
	{
		int column(0);
		for (std::vector<int>::iterator columnIterator = lineIterator->begin(); columnIterator != lineIterator->end(); ++columnIterator)
		{
			if (*columnIterator != 0)
			{
				DrawCube(line * cubeWidth + line, column * cubeLength + column, 0, cubeWidth, cubeLength, static_cast<double>(*columnIterator));
			}
			++column;
		}
		++line;
	}
	fps();

	glutSwapBuffers();
}

// Inicializa parâmetros de rendering
void Inicializa(void)
{
	GLfloat luzAmbiente[4] = { 0.2,0.2,0.2,1.0 };
	GLfloat luzDifusa[4] = { 0.7,0.7,0.7,1.0 };		 // "cor" 
	GLfloat luzEspecular[4] = { 1.0, 1.0, 1.0, 1.0 };// "brilho" 
	GLfloat posicaoLuz[4] = { 0.0, 50.0, 50.0, 1.0 };

	// Capacidade de brilho do material
	GLfloat especularidade[4] = { 1.0,1.0,1.0,1.0 };
	GLint especMaterial = 60;

	// Especifica que a cor de fundo da janela será preta
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	// Habilita o modelo de colorização de Gouraud
	glShadeModel(GL_SMOOTH);

	// Define a refletância do material 
	glMaterialfv(GL_FRONT, GL_SPECULAR, especularidade);
	// Define a concentração do brilho
	glMateriali(GL_FRONT, GL_SHININESS, especMaterial);

	// Ativa o uso da luz ambiente 
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, luzAmbiente);

	// Define os parâmetros da luz de número 0
	glLightfv(GL_LIGHT0, GL_AMBIENT, luzAmbiente);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, luzDifusa);
	glLightfv(GL_LIGHT0, GL_SPECULAR, luzEspecular);
	glLightfv(GL_LIGHT0, GL_POSITION, posicaoLuz);

	// Habilita a definição da cor do material a partir da cor corrente
	glEnable(GL_COLOR_MATERIAL);
	//Habilita o uso de iluminação
	glEnable(GL_LIGHTING);
	// Habilita a luz de número 0
	glEnable(GL_LIGHT0);
	// Habilita o depth-buffering
	glEnable(GL_DEPTH_TEST);

	angle = 45;
}

// Função usada para especificar o volume de visualização
void EspecificaParametrosVisualizacao(void)
{
	// Especifica sistema de coordenadas de projeção
	glMatrixMode(GL_PROJECTION);
	// Inicializa sistema de coordenadas de projeção
	glLoadIdentity();

	// Especifica a projeção perspectiva
	gluPerspective(angle, fAspect, 0.4, 500);

	// Especifica sistema de coordenadas do modelo
	glMatrixMode(GL_MODELVIEW);
	// Inicializa sistema de coordenadas do modelo
	glLoadIdentity();

	// Especifica posição do observador e do alvo
	gluLookAt(0, 80, 200, 0, 0, 0, 0, 1, 0);
}

// Função callback chamada quando o tamanho da janela é alterado 
void AlteraTamanhoJanela(GLsizei w, GLsizei h)
{
	// Para previnir uma divisão por zero
	if (h == 0) h = 1;

	// Especifica o tamanho da viewport
	glViewport(0, 0, w, h);

	// Calcula a correção de aspecto
	fAspect = (GLfloat)w / (GLfloat)h;

	EspecificaParametrosVisualizacao();
}

// Função callback chamada para gerenciar eventos do mouse
void GerenciaMouse(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON)
		if (state == GLUT_DOWN) {  // Zoom-in
			if (angle >= 10) angle -= 5;
		}
	if (button == GLUT_RIGHT_BUTTON)
		if (state == GLUT_DOWN) {  // Zoom-out
			if (angle <= 130) angle += 5;
		}
	EspecificaParametrosVisualizacao();
	glutPostRedisplay();
}

// Programa Principal
int main(void)
{
	csvReader myCsvReader("E:\\Documents\\WiLL\\Msc\\CG\\Input\\input.csv");
	myMap = myCsvReader.getAllLines();
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(400, 350);
	glutCreateWindow("CG - T1 - Paulo & William");
	glutDisplayFunc(Desenha);
	glutReshapeFunc(AlteraTamanhoJanela);
	glutMouseFunc(GerenciaMouse);
	Inicializa();
	glutMainLoop();
}
