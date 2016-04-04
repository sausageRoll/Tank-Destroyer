#include "Background.h"
#include "lodepng.h"
#include <iostream>
#include <glut.h>
#include <algorithm>
#include <math.h>

Background::Background() : fileName("picture/back.png")
{
	speedFileName[0] = "picture/VT.png";
	speedFileName[1] = "picture/��_�.png";
	speedFileName[2] = "picture/5.png";
	speedFileName[3] = "picture/10.png";
	speedFileName[4] = "picture/15.png";
	speedFileName[5] = "picture/20.png";
	speedFileName[6] = "picture/25.png";
	speedFileName[7] = "picture/30.png";
	speedFileName[8] = "picture/35.png";
	speedFileName[9] = "picture/40.png";

	speedFileName[10] = "picture/VW.png";
	speedFileName[11] = "picture/�_�.png";
	speedFileName[12] = "picture/1W.png";
	speedFileName[13] = "picture/2W.png";
	speedFileName[14] = "picture/3W.png";
	speedFileName[15] = "picture/4W.png";
	speedFileName[16] = "picture/5W.png";

	speedFileName[17] = "picture/0WD.png";
	speedFileName[18] = "picture/90WD.png";
	speedFileName[19] = "picture/180WD.png";
	speedFileName[20] = "picture/270WD.png";
}

Background::~Background()
{
}

void Background :: load()
{
	curTankSpeed = 2 + random(8);
	curWindSpeed = TANK_N + 2 + random(5);
	curWindDir = TANK_N + WIND_N + random(4);

	unsigned char* data;
	unsigned width, height;
	person = lodepng_decode32_file(&data, &width, &height, fileName);

	glGenTextures(1,&person);
	glBindTexture(GL_TEXTURE_2D,person);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
	gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA, width, height, GL_RGBA, GL_UNSIGNED_BYTE, data);

	for (int i = 0; i < TANK_N + WIND_N + DIR_N; ++i)
	{
		speedId[i] = lodepng_decode32_file(&data, &width, &height, speedFileName[i]);

		glGenTextures(1,&(speedId[i]));
		glBindTexture(GL_TEXTURE_2D,speedId[i]);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
		gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA, width, height, GL_RGBA, GL_UNSIGNED_BYTE, data);
	}

	delete data;
}

void Background :: draw() const
{
	double X1 = 0.0,Y1 = 0,X2= 1,Y2 = 1, Z = 1.0;

	glPushMatrix();
	glTranslated(0.0,0.0,-5.0);

	glEnable( GL_TEXTURE_2D );
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	
	glBindTexture( GL_TEXTURE_2D, person);

	glPixelStorei( GL_UNPACK_ALIGNMENT, 1 ); 
	glBegin( GL_QUADS );

	glTexCoord2d(X1,Y2); glVertex2f(0.0,0.0);
	glTexCoord2d(X1,Y1); glVertex2f(0.0,POLE_Y);
	glTexCoord2d(X2,Y1); glVertex2f(POLE_X,POLE_Y);
	glTexCoord2d(X2,Y2); glVertex2f(POLE_X,0.0);
	glEnd();

	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

	glDisable(GL_BLEND);
	glDisable(GL_TEXTURE_2D);

	glPopMatrix();
}

void Background :: drawSpeed() const
{
	double X1 = 0.0,Y1 = 0,X2= 1,Y2 = 1, Z = 1.0;
	glPushMatrix();

		/*SPEED TANK*/
		glTranslated(SPEED_POS_X,SPEED_POS_Y,0.0);
		glScaled(TEXT_SIZE,TEXT_SIZE,1.0);

		glEnable( GL_TEXTURE_2D );
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	
		glBindTexture( GL_TEXTURE_2D, speedId[0]);

		glPixelStorei( GL_UNPACK_ALIGNMENT, 1 ); 
		glBegin( GL_QUADS );

		glTexCoord2d(X1,Y2); glVertex2f(0.0,0.0);
		glTexCoord2d(X1,Y1); glVertex2f(0.0,1.0);
		glTexCoord2d(X2,Y1); glVertex2f(1.0,1.0);
		glTexCoord2d(X2,Y2); glVertex2f(1.0,0.0);
		glEnd();

		glBindTexture( GL_TEXTURE_2D, speedId[curTankSpeed]);

		glPixelStorei( GL_UNPACK_ALIGNMENT, 1 ); 
		glBegin( GL_QUADS );

		glTexCoord2d(X1,Y2); glVertex2f(1.0,0.0);
		glTexCoord2d(X1,Y1); glVertex2f(1.0,1.0);
		glTexCoord2d(X2,Y1); glVertex2f(2.0,1.0);
		glTexCoord2d(X2,Y2); glVertex2f(2.0,0.0);
		glEnd();

		glBindTexture( GL_TEXTURE_2D, speedId[1]);

		glPixelStorei( GL_UNPACK_ALIGNMENT, 1 ); 
		glBegin( GL_QUADS );

		glTexCoord2d(X1,Y2); glVertex2f(2.0,0.0);
		glTexCoord2d(X1,Y1); glVertex2f(2.0,1.0);
		glTexCoord2d(X2,Y1); glVertex2f(3.0,1.0);
		glTexCoord2d(X2,Y2); glVertex2f(3.0,0.0);
		glEnd();

		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

		/*SPEED WIND*/

		glEnable( GL_TEXTURE_2D );
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	
		glBindTexture( GL_TEXTURE_2D, speedId[TANK_N]);

		glPixelStorei( GL_UNPACK_ALIGNMENT, 1 ); 
		glBegin( GL_QUADS );

		glTexCoord2d(X1,Y2); glVertex2f(0.0,-1.0);
		glTexCoord2d(X1,Y1); glVertex2f(0.0,0.0);
		glTexCoord2d(X2,Y1); glVertex2f(1.0,0.0);
		glTexCoord2d(X2,Y2); glVertex2f(1.0,-1.0);
		glEnd();

		glBindTexture( GL_TEXTURE_2D, speedId[curWindSpeed]);

		glPixelStorei( GL_UNPACK_ALIGNMENT, 1 ); 
		glBegin( GL_QUADS );

		glTexCoord2d(X1,Y2); glVertex2f(1.0,-1.0);
		glTexCoord2d(X1,Y1); glVertex2f(1.0,0.0);
		glTexCoord2d(X2,Y1); glVertex2f(2.0,0.0);
		glTexCoord2d(X2,Y2); glVertex2f(2.0,-1.0);
		glEnd();

		glBindTexture( GL_TEXTURE_2D, speedId[TANK_N + 1]);

		glPixelStorei( GL_UNPACK_ALIGNMENT, 1 ); 
		glBegin( GL_QUADS );

		glTexCoord2d(X1,Y2); glVertex2f(2.0,-1.0);
		glTexCoord2d(X1,Y1); glVertex2f(2.0,0.0);
		glTexCoord2d(X2,Y1); glVertex2f(3.0,0.0);
		glTexCoord2d(X2,Y2); glVertex2f(3.0,-1.0);
		glEnd();
		
			/*Wind Dir*/
		glBindTexture( GL_TEXTURE_2D, speedId[curWindDir]);

		glPixelStorei( GL_UNPACK_ALIGNMENT, 1 ); 
		glBegin( GL_QUADS );

		glTexCoord2d(X1,Y2); glVertex2f(3.0,-1.0);
		glTexCoord2d(X1,Y1); glVertex2f(3.0,0.0);
		glTexCoord2d(X2,Y1); glVertex2f(4.0,0.0);
		glTexCoord2d(X2,Y2); glVertex2f(4.0,-1.0);
		glEnd();

		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

		glDisable(GL_BLEND);
		glDisable(GL_TEXTURE_2D);
			/**/
	glPopMatrix();
}