#include <math.h>
#include <iostream>
#include <vector>
#include <Windows.h>
#include <glut.h>
#include <time.h>
#include <iomanip>

#include "spline.c"

#include "const.h"
#include "Tank.h"
#include "Background.h"
#include "Aim.h"
#include "Shell.h"
#include "Fire.h"

double *bDistToSize, *cDistToSize, *dDistToSize;
double *bZToY, *cZToY, *dZToY;

using std :: setw;

BackgroundPointer b = BackgroundPointer(new Background());
TankPointer t = TankPointer(new Tank());
AimPointer a = AimPointer(new Aim());
ShellPointer s = ShellPointer(new Shell());

bool   gp;                              // G ������? ( ����� )
GLuint filter;                          // ������������ ������ ��� �������
GLuint fogMode[]= { GL_EXP, GL_EXP2, GL_LINEAR }; // ������ ��� ���� ������
GLuint fogfilter= 2;                    // ��� ������������� ������
GLfloat fogColor[4]= {0.5f, 0.5f, 0.5f, 1.0f}; // ���� ������

void display();
void specialKeys();
void keyPress();
double rotate_y=0; 
double rotate_x=0;
bool flag = false;

double stX = 0.0, stY = 0.0;

double startX = -0.5, startY = -0.5, startZ = -0.5;

double bondI = 0.0;
	
std :: vector<int> KeyDown(256);

int flagStop = true;

void stop()
{
	flagStop = false;
}

void display(){

	//  Clear screen and Z-buffer
	keyPress();

	if (flagStop)
	{
		glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

		t->forward();
		b->draw();
		t->draw();
		a->drawPicture();
		s->stepForward();
		s->draw();
		b->drawSpeed();
		b->drawBondBlood();

		glutSwapBuffers();
	}

	if (!flag && s->isExlosion())
	{
		flag = true;

		double shellX = s->getX(),
			shellY = s->getY(),
			tankX = t->getX(),
			tankY = t->getY();
			
		std :: cout << setw(10) <<abs(shellX - tankX) << setw(10) << abs(shellY - tankY) << std :: endl;

		stop();
	}

	Sleep(50);
}

void key(unsigned char key, int x, int y){
	KeyDown[key] = 1;
}
void keyUp(unsigned char key, int x, int y){
	KeyDown[key] = 0;
}
void keyPress() 
{
//  Right arrow - increase rotation by 5 degree
	if (KeyDown['6'])
		rotate_y += 0.1;

	//  Left arrow - decrease rotation by 5 degree
	else if (KeyDown['4'])
		rotate_y -= 0.1;

	else if (KeyDown['8'] )
		rotate_x += 0.1;

	else if (KeyDown['5'])
		rotate_x -= 0.1;

	else if (KeyDown['w'])
		t->forward();
	else if (KeyDown['s'])
		t->backward();

	if (KeyDown['g'] && !gp)                   // ������ �� ������� "G"?
	{
		   gp=TRUE;                         // gp ������������� � TRUE
		   fogfilter+=1;                    // ���������� fogfilter �� 1
		   if (fogfilter>2)                 // fogfilter ������ 2 ... ?
		   {
				  fogfilter=0;              // ���� ���, ���������� fogfilter � ����
		   }
		   glFogi (GL_FOG_MODE, fogMode[fogfilter]); // ����� ������
	}
	if (!KeyDown['g'])                         // ������� "G" ��������?
	{
		   gp=FALSE;                        // ���� ��, gp ���������� � FALSE
	}
}

void Init()
{
	bDistToSize = new double[NdistToSize];
	cDistToSize = new double[NdistToSize];
	dDistToSize = new double[NdistToSize];

	bZToY = new double[NZToY];
	cZToY = new double[NZToY];
	dZToY = new double[NZToY];
	int flag;

	double *splineDist = new double[NdistToSize], *splineSize = new double[NdistToSize];
	double *splineZ = new double[NZToY], *splineY = new double[NZToY];

	memcpy(splineDist,dist,sizeof(dist));
	memcpy(splineSize,size,sizeof(size));

	memcpy(splineZ,Z,sizeof(Z));
	memcpy(splineY,Y,sizeof(Y));

	spline(NdistToSize,0,0,0,0,splineDist,splineSize,bDistToSize,cDistToSize,dDistToSize,&flag);
	if (flag != 0)
	{
		std :: cout << "SPLINE FLAG != 0\n";
		return exit(-1);
	}

	spline(NdistToSize,0,0,0,0,splineZ,splineY,bZToY,cZToY,dZToY,&flag);
	if (flag != 0)
	{
		std :: cout << "SPLINE FLAG != 0\n";
		return exit(-1);
	}

	srand(time(0u));
	std :: cout << "It's started\n";

	std :: cout << f(5) << std :: endl;

	glClearColor(0.5f,0.5f,0.5f,1.0f);      // ����� ������� �����, �������� ��� ������ ������. ( �������� )

	glOrtho(0.0,POLE_X,0.0,POLE_Y,0.0,POLE_Z);
	//  Enable Z-buffer depth test
	glEnable(GL_DEPTH_TEST);

	t->load();
	b->load();
	a->load();
	s->load();

	s->setBackgroundPointer(b);

	s->setWindSpeed(b->getWindSpeed());
	s->setAngleWind(b->getWindAngle());

	t->setSpeed(b->getTankSpeed());
	t->setCurState(b->getCurTankDir());

	t->setAimRad(a->getRad());
	t->setDistToSize(bDistToSize, cDistToSize, dDistToSize);
	t->setSplineMas(splineDist,splineSize);

	s->setDistToSize(bDistToSize, cDistToSize, dDistToSize);
	s->setSplineMas(splineDist,splineSize);


}

void mouse(int but, int st, int x,int y)
{
	if (but == GLUT_RIGHT_BUTTON)
	{
		if (st == GLUT_DOWN)
		{
			a->changeShown();
		}
	}
	if (but == GLUT_LEFT_BUTTON)
	{
		if (st == GLUT_DOWN)
		{
			s->fire(stX,stY);
		}
	}
}

void motion(int x, int y)
{
	stX = x;
	a->setX(x);
	stY = iWindowY - y;
	a->setY(iWindowY - y);
}

int main(int argc, char* argv[]){

	//  Initialize GLUT and process user parameters
	glutInit(&argc,argv);

	//  Request double buffered true color window with Z-buffer
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(iWindowX,iWindowY);				
	glutInitWindowPosition(50,50);	

	// Create window
	glutCreateWindow("Awesome Cube");

	Init();

	// Callback functions
	glutDisplayFunc(display);
	glutIdleFunc(display);
	glutKeyboardFunc(key);
	glutKeyboardUpFunc(keyUp);
	glutMouseFunc(mouse);
	glutPassiveMotionFunc(motion);

	//  Pass control to GLUT for events
	glutMainLoop();

	//  Return to OS
	return 0;

}