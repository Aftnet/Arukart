#include <sps2lib.h>
#include <sps2tags.h>
#include <sps2util.h>
#include <sps2regstructs.h>
#include <math.h>
#include "PS2Defines.h"
#include "ps2matrix4x4.h"
#include "sps2wrap.h"
#include "dma.h"
#include "primitives.h"
#include "pyseng2d.h"

PhysEng2D::PhysEng2D()
{
}

PhysEng2D::~PhysEng2D()
{
}

bool PhysEng2D::BoundingBox(PS2Sprite & S1, PS2Sprite & S2)
{
	float x1 = S1.GetXPos();
	float y1 = S1.GetYPos();
	float hw1 = S1.GetBBhwidth();
	float hh1 = S1.GetBBhheight();
	float x2 = S2.GetXPos();
	float y2 = S2.GetYPos();
	float hw2 = S2.GetBBhwidth();
	float hh2 = S2.GetBBhheight();
	if((x1+hw1)<(x2-hw2) || (x1-hw1)>(x2+hw2))
	{
		return false;
	}
	else if((y1-hh1)>(y2+hh2) || (y1+hh1)<(y2-hh2))
	{
		return false;
	}
	else
	{
		return true;
	}
}

bool PhysEng2D::BoundingCircle(PS2Sprite & S1, PS2Sprite & S2)
{
	float x1 = S1.GetXPos();
	float y1 = S1.GetYPos();
	float r1 = S1.GetRadius();
	float x2 = S2.GetXPos();
	float y2 = S2.GetYPos();
	float r2 = S2.GetRadius();
	if((((x1-x2)*(x1-x2))+((y1-y2)*(y1-y2)))<=((r1+r2)*(r1+r2)))
	{
		return true;
	}
	else
	{
		return false;
	}
}

void PhysEng2D::SwapVelocities(PS2Sprite & S1, PS2Sprite & S2)
{
	float tempx = S1.GetXSpd();
	float tempy = S1.GetYSpd();
	S1.SetSpd(S2.GetXSpd(),S2.GetYSpd());
	S2.SetSpd(tempx,tempy);
	
}
	
void PhysEng2D::ReverseVelocities(PS2Sprite & S1, PS2Sprite & S2)
{
	S1.SetSpd((0-S1.GetXSpd()),(0-S1.GetYSpd()));
	S2.SetSpd((0-S2.GetXSpd()),(0-S2.GetYSpd()));
}
	
void PhysEng2D::RealVelocities(PS2Sprite & S1, PS2Sprite & S2)
{
}

void PhysEng2D::WallCollision(PS2Sprite & S)
{
}
