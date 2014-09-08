#ifndef _3DPRIMITIVES_H_
#define _3DPRIMITIVES_H_

#include "PS2Defines.h"

class PS23DSprite 
{
public:

	PS23DSprite();
	
	PS23DSprite(const float x, const float y);
	
	PS23DSprite(const float v1x, const float v1y, const float v1z, const float v2x, const float v2y, const float v2z, const float v3x, const float v3y, const float v3z, const float v4x, const float v4y, const float v4z);
	
	~PS23DSprite();
	
	virtual void Render(void) const;
	
	void Edit(const float v1x, const float v1y, const float v1z, const float v2x, const float v2y, const float v2z, const float v3x, const float v3y, const float v3z, const float v4x, const float v4y, const float v4z);
	
	//void MoveTo(const float x, const float y);
	
	//void MoveDeltaX(const float x);
	
	//void MoveDeltaY(const float y);
	
	
	
protected:

	void Initialise(void);		// Initialise variables

	float m_x, m_y;				// position on screen
	float m_z;					// z depth (big = near)
	float m_w, m_h;				// width and height
	uint8 m_r, m_g, m_b, m_a;	// colour and alpha
	
	// Note - The z coordinate here is only used for
	// perspective correct texture mapping if the quad
	// to be drawn is not rectangular. It has got no 
	// relationship with the m_z member which is used
	// for z buffer sorting.
	struct Vector3				// Local Space
	{							// Vertex coordinates and normalized (from 0.0 to 1.0)  texture coordinates
		float x, y, z, tnx, tny;
	}m_TopLeft, m_BottomLeft, m_TopRight, m_BottomRight;
};

class PS23DSpriteT : protected PS23DSprite
{
public:
	
	PS23DSpriteT();
	
	PS23DSpriteT(const float x, const float y);
	
	PS23DSpriteT(const float v1x, const float v1y, const float v1z, const float v1tx, const float v1ty, const float v2x, float v2y, const float v2z, const float v2tx, const float v2ty, const float v3x, const float v3y, const float v3z, const float v3tx, const float v3ty, const float v4x, const float v4y, const float v4z, const float v4tx, const float v4ty);
	
	~PS23DSpriteT();
	
	void Edit(const float v1x, const float v1y, const float v1z, const float v1tx, const float v1ty, const float v2x, float v2y, const float v2z, const float v2tx, const float v2ty, const float v3x, const float v3y, const float v3z, const float v3tx, const float v3ty, const float v4x, const float v4y, const float v4z, const float v4tx, const float v4ty);

	float GetVertexData(const int vertexno, const char attr) const;
	
	void Render(void) const;
	
protected:
};

#endif