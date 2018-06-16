#include <GL/glut.h>
#include <cstdlib>
#include <cstdio>
#include <math.h>

// Variavel global que define o tipo de projecao
int op = 0;
// Variavel global que define o incremento
double increment = 3.47;
// Variavel global que define a escala
double zoom = 1;
// Variaveis globais que definem os angulos de rotacao nos eixos x, y e z
double angle_x = 0;
double angle_y = 0;
double angle_z = 0;
// Variaveis globais que definem as posicoes de translacao nos eixos x, y e z
double pos_x = 0;
double pos_y = 0;
double pos_z = 0;
int obj = 1;

void display();

void init() {
	glutInitDisplayMode(GLUT_DOUBLE);
	glutInitWindowSize(700, 700);
	glutCreateWindow("Teapot");

	glClearColor (0.0, 0.0, 0.0, 0.0); //fundo preto
	glShadeModel (GL_FLAT); // modelo flat
	// glShadeModel (GL_SMOOTH); // modelo de gouraud

	GLfloat luz_ambiente[4] = {0.2, 0.2, 0.2, 1.0}; 
	GLfloat luz_difusa[4] = {0.7, 0.7, 0.7, 1.0}; // cor 
	GLfloat luz_especular[4] = {1.0, 1.0, 1.0, 1.0}; // brilho 
	GLfloat posicao_luz[4] = {0.0, 50.0, 50.0, 1.0};

	GLfloat especularidade[4] = {1.0, 1.0, 1.0, 1.0}; // Capacidade de brilho do material
	GLint espec_material = 60;

	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, especularidade);
	// Define a concentração do brilho
	glMateriali(GL_FRONT_AND_BACK, GL_SHININESS, espec_material);

	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, luz_ambiente);

	// Define os parâmetros da luz de número 0
	glLightfv(GL_LIGHT0, GL_AMBIENT, luz_ambiente); 
	glLightfv(GL_LIGHT0, GL_DIFFUSE, luz_difusa);
	glLightfv(GL_LIGHT0, GL_SPECULAR, luz_especular);
	glLightfv(GL_LIGHT0, GL_POSITION, posicao_luz);

	// Habilita a cor do material pela cor atual
	glEnable(GL_COLOR_MATERIAL);
	// Habilita iluminação
	glEnable(GL_LIGHTING);  
	// Habilita a luz 0
	glEnable(GL_LIGHT0);
	// Habilita o depth-buffering
	glEnable(GL_DEPTH_TEST);
}

void setProjection(int mask) {
	
	glMatrixMode(GL_PROJECTION); // Define que ira trabalhar com a matriz de projecao
	glLoadIdentity(); // Carrega a matriz identidade

	// Projecao ortogonal
	if (op == 0) {
		glOrtho((mask & 1)? 20 : -20, (mask & 1)? -20 : 20, 
				(mask & 2)? 20 : -20, (mask & 2)? -20 : 20, -200, 200);
	}
	// Projecao perspectiva
	else {
		glFrustum((mask & 1)? 10 : -10, (mask & 1)? -10 : 10, 
				(mask & 2)? 10 : -10, (mask & 2)? -10 : 10, 10, 200);
	}

}
// Desenha os eixos x, y e z em degrade
void drawAxis()  {
	// Eixo x -> vermelho
	glBegin(GL_LINES);
	glColor3f(1, 0, 0);
	glVertex3f(0,0,0);
	glVertex3f(40,0,0);
	glEnd();
	
	// Eixo y -> amarelo
	glBegin(GL_LINES);
	glColor3f(1, 1, 0);
	glVertex3f(0,0,0);
	glVertex3f(0,40,0);
	glEnd();

	// Eixo z -> azul
	glBegin(GL_LINES);
	glColor3f(0, 0, 1);
	glVertex3f(0,0,0);
	glVertex3f(0,0,40);
	glEnd();
}
	
// Funcao para capturar os eventos do teclado
void keyPressEvent(GLubyte key, GLint x, GLint y) {

	if (key == '\x1b') exit(0); // Sai do programa se apertar ESC
	
	if (key == 'Q') angle_x += increment; // Rotaciona em x no sentido anti horario
	else if (key == 'A') angle_x -= increment; // Rotaciona em x no sentido horario
	else if (key == 'W') angle_y += increment; // Rotaciona em y no sentido anti horario
	else if (key == 'S') angle_y -= increment; // Rotaciona em y no sentido horario
	else if (key == 'E') angle_z += increment; // Rotaciona em z no sentido anti horario
	else if (key == 'D') angle_z -= increment; // Rotaciona em z no sentido horario
	
	if (key == 'q') pos_x += increment; // Translada em x no sentido positivo
	else if (key == 'a') pos_x -= increment; // Translada em x no sentido negativo
	else if (key == 'w') pos_y += increment; // Translada em y no sentido positivo
	else if (key == 's') pos_y -= increment; // Translada em y no sentido negativo
	else if (key == 'e') pos_z += increment; // Translada em z no sentido positivo
	else if (key == 'd') pos_z -= increment; // Translada em z no sentido negativo

	if (key == '1') obj = 1; // Seta o objeto como sendo a esfera
	else if (key == '2') obj = 2; // Seta o objeto como sendo o cubo
	else if (key == '3') obj = 3; // Seta o objeto como sendo o bule

	if (key == '+') {
		zoom += 0.1; // Aumenta a escala
		if (zoom > 4) zoom = 4; // valor maximo de escala		
	}
	else if (key == '-') zoom -= 0.1; // Diminui a escala
	if (zoom < 0.1) zoom = 0.1; // valor minimo de escala

	display();
}

void displayViewPort(int x, int y, int w, int h, int mask) {

	// Define uma porta de visao para a projecao
	glViewport(x, y, w, h);

	// Chama a funcao para configurar o tipo de projecao (ortogonal ou perspectiva)
	setProjection(mask);

	// Define as configuracoes do observador
	gluLookAt(15, 15, 15, 0, 0, 0, 0, 1, 0);

	// Define que ira trabalhar com a matriz de modelo/visao
	glMatrixMode(GL_MODELVIEW);

	// Carrega a matriz identidade
	glLoadIdentity();

	// Desenha os eixos 
	drawAxis();

	// Translada o objeto
	glTranslatef(pos_x, pos_y, pos_z);

	// Rotaciona o objeto
	glRotatef(angle_x, 1, 0, 0);
	glRotatef(angle_y, 0, 1, 0);
	glRotatef(angle_z, 0, 0, 1);

	// Escala o objeto
	glScalef(zoom, zoom, zoom);

	
	glColor3f(0,1,0);
	if (obj == 1) {
		glutSolidSphere(10, 50, 20); //Desenha um tetraedro solido na cor verde

		glShadeModel (GL_SMOOTH); // modelo de gouraud foi considerado mais adequado a esfera
	}
	else if (obj == 2) {
		glutSolidCube(10.0); // Desenha um cubo de arame na cor verde
		glShadeModel (GL_FLAT); // modelo flat foi considerado mais adequado ao cubo
	}
	else if (obj == 3) {
		glutSolidTeapot(10.0); // Desenha um bule de arame na cor verde
		glShadeModel (GL_SMOOTH); // modelo de gouraud foi considerado mais adequado ao teapot
	}
}

void drawLines() {
	glViewport(0, 0, 700, 700);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-0.50, 700, -0.5, 700, -1, 1);

	//Vertical 
	glBegin(GL_LINES);
	glColor3f(0.6, 0.6, 0.6);
	glVertex3f(350, 0, 0);
	glVertex3f(350, 700, 0);
	glEnd();

	//Horizontal
	glBegin(GL_LINES);
	glColor3f(0.6, 0.6, 0.6);
	glVertex3f(0,350,0);
	glVertex3f(700,350,0);
	glEnd();
}
	
void display() {

	glClear(GL_COLOR_BUFFER_BIT);


	displayViewPort(0, 350, 350, 350, 0);
	displayViewPort(350, 350, 350, 350, 1); // Espelha no eixo x
	displayViewPort(0, 0, 350, 350, 2); // Espelha no eixo y
	displayViewPort(350, 0, 350, 350, 3); // Espelha nos eixos x e y
	drawLines();

	glutSwapBuffers();
}

int main(int argc, char * argv[]) {
	printf("Qual o tipo de projecao desejada? (Digite 0 para paralela e 1 para perspectiva)\n");
	scanf("%d%*c", &op);

	printf("\nINFORMACOES\n");
	printf("===========\n");
	printf("\tO eixo vermelho e o eixo x\n");
	printf("\tO eixo amarelo e o eixo y\n");
	printf("\tO eixo azul e o eixo z\n");

	printf("COMANDOS\n");
	printf("========\n");
	printf("\tESCOLHA DE OBJETO\n");
	printf("\t\t(1) --> Escolhe a esfera\n");
	printf("\t\t(2) --> Escolhe o cubo\n");
	printf("\t\t(3) --> Escolhe o bule\n");
	printf("\tTRANSLACAO\n");
	printf("\t\t(q) --> positiva no eixo x\n");
	printf("\t\t(a) --> negativa no eixo x\n");
	printf("\t\t(w) --> positiva no eixo y\n");
	printf("\t\t(s) --> negativa no eixo y\n");
	printf("\t\t(e) --> positiva no eixo z\n");
	printf("\t\t(d) --> negativa no eixo z\n\n");
	printf("\tROTACAO\n");
	printf("\t\t(Q) (shift + q) --> sentido anti horario no eixo x\n");
	printf("\t\t(A) (shift + a) --> sentido horario no eixo x\n");
	printf("\t\t(W) (shift + w) --> sentido anti horario no eixo y\n");
	printf("\t\t(S) (shift + s) --> sentido horario no eixo y\n");
	printf("\t\t(E) (shift + e) --> sentido anti horario no eixo z\n");
	printf("\t\t(D) (shift + d) --> sentido horario no eixo z\n\n");
	printf("\tESCALA\n");
	printf("\t\t(+) (shift + =) --> aumenta a escala\n");
	printf("\t\t(-) --> diminui a escala\n\n");


	printf("Pressione ENTER para continuar\n");
	scanf("%*c");

	glutInit(&argc, argv);

	init();
	glutDisplayFunc(display);
	glutKeyboardFunc(keyPressEvent);

	printf("Pressione ESC para sair\n");

	glutMainLoop();

	return 0;
}