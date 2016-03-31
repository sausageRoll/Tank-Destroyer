#include "Shell.h"
#include <math.h>
#include <glut.h>

#include "Fire.h"

Shell::Shell() : isFire(false), isExplosion(false)
{
}


Shell::~Shell()
{
}

void Shell :: fire(double startX, double setEndZ)
{
	isFire = true;
	x = startX;
	y = 0.0;
	z = 0.0;

	angle = PI/2.0;

	speed = 30.0;
	endZ = setEndZ;
}

void Shell :: load()
{
	f = FirePointer(new Fire());
	f->load();
}

void Shell :: stepForward()
{
	if (isFire)
	{
		double tempAngle = atan(2*z);

		z += speed*shellH;

		//y = -0.005*z*z + 1.0*z + 3.0/5.0*z;
		y = -0.005*z*(z-endZ) + 3.5/5.0*z;
		
		x += cos(angle);

		if (z > endZ)
		{
			isFire = false;
			isExplosion = true;
			auto hScaled = 552.0/9.0 - 48.0/900.0*z;
			f->setSize(hScaled);
			f->setShellSize(10.0);
			f->setX(x);
			f->setY(y);
		}
	}
}

void Shell :: draw() const
{
	if (isFire)
	{
		glPushMatrix();
		glTranslated(x,y,0.0);
		glScaled(10.0,10.0,1.0);

		glBegin(GL_POLYGON);
		glColor3d(1.0,0.0,0.0);
			for (double i = 0.0; i < 2*PI; i += 0.1)
				glVertex2d(cos(i), sin(i));
		glEnd();

		glPopMatrix();
	}
	else if (isExplosion)
	{
		
		explosion();
	}
}

void Shell :: explosion() const
{
	f->draw();
}