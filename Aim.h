#pragma once
#include "const.h"

class Aim
{
public:
	Aim();
	~Aim();

	void draw() const;

	inline void setX(double set)
	{
		x = set;
	}

	inline void setY(double set)
	{
		y = set;
	}

	inline void changeShown()
	{
		isShown = !isShown;
	}

	void moveX(int dir);
	void moveY(int dir);
private:
	double x,y;
	double rad;
	
	bool isShown;
};

