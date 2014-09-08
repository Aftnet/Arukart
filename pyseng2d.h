#ifndef _PYSENG2D_H_
#define _PYSENG2D_H_

#include "PS2Defines.h"
#include "primitives.h"

class PhysEng2D
{
	public:
	
	PhysEng2D::PhysEng2D ();
	
	PhysEng2D::~PhysEng2D ();
	
	// Collision detection algorithms
	
	bool BoundingBox(PS2Sprite & S1, PS2Sprite & S2);
	
	bool BoundingCircle(PS2Sprite & S1, PS2Sprite & S2);

	// Collision response algorithms
	
	void SwapVelocities(PS2Sprite & S1, PS2Sprite & S2);
	
	void ReverseVelocities(PS2Sprite & S1, PS2Sprite & S2);
	
	void RealVelocities(PS2Sprite & S1, PS2Sprite & S2);

	// Environmental collision handling
	
	void WallCollision(PS2Sprite & S);
};

#endif