#ifndef _PRIMITIVES_H_
#define _PRIMITIVES_H_

#include "PS2Defines.h"

class PS2Sprite 
{
public:

	PS2Sprite::PS2Sprite();
	
	PS2Sprite::PS2Sprite(const float x, const float y);
	
	PS2Sprite::PS2Sprite(const float x, const float y, const float width, const float height);
	
	PS2Sprite::PS2Sprite(const float x, const float y, const float z, const float width, const float height);
	
	PS2Sprite::PS2Sprite(const float x, const float y, const uint8 rcolor, const uint8 gcolor, const uint8 bcolor, const uint8 alpha);
	
	PS2Sprite::PS2Sprite(const float x, const float y, const float width, const float height, const uint8 rcolor, const uint8 gcolor, const uint8 bcolor, const uint8 alpha);
	
	PS2Sprite::~PS2Sprite();
	
	virtual void Render(void) const;
	
	void MoveTo(const float x, const float y);
	
	void MoveDeltaX(const float x);
	
	void MoveDeltaY(const float y);
	
	void Move(void);
	
	void SetSpd(const float Cx, const float Cy);
	
	void SetColor(const uint8 rcolor, const uint8 gcolor, const uint8 bcolor, const uint8 alpha);
	
	void RotateX(const float nangle);
	
	void RotateY(const float nangle);
	
	void RotateZ(const float nangle);
	
	void Scale(const float nscale);
	
	void ScaleX(const float nxscale);
	
	void ScaleY(const float nyscale);
	
	float GetXPos(void) const;
	
	float GetYPos(void) const;
	
	float GetWidth(void) const;
	
	float GetHeight(void) const;
	
	float GetRadius(void) const;
	
	float GetBBhwidth(void) const;
	
	float GetBBhheight(void) const;
	
	float GetXSpd(void) const;
	
	float GetYSpd(void) const;
	
	float GetRColor(void) const;
	
	float GetGColor(void) const;
	
	float GetBColor(void) const;
	
	float GetAlpha(void) const;
	
	float GetXAngle(void) const;
	
	float GetYAngle(void) const;
	
	float GetZAngle(void) const;
	
	float GetXScale(void) const;
	
	float GetYScale(void) const;
	
	float GetV1xpos(void) const;
	
	float GetV1ypos(void) const;
	
	float GetV2xpos(void) const;
	
	float GetV2ypos(void) const;
	
	float GetV3xpos(void) const;
	
	float GetV3ypos(void) const;
	
	float GetV4xpos(void) const;
	
	float GetV4ypos(void) const;
	
protected:

	void Initialise(void);		// Initialise variables
	void ApplyTransforms(void); //Applies transformations

	float m_x, m_y;				// position on screen
	float m_z;					// z depth (big = near)
	float m_w, m_h;				// width and height
	float m_radius;
	float m_bbhw, m_bbhh;
	uint8 m_r, m_g, m_b, m_a;	// colour and alpha
	float m_Vx, m_Vy;           //velocity
	float m_xangle;             //rotation angle about X axis
	float m_yangle;             //rotation angle about Y axis
	float m_zangle;             //rotation angle about Z axis
	float m_xscale;             //horizontal scale
	float m_yscale;             //vertical scale
	
	struct Vector2				// Local Space
	{							// Vertex coordinates
		float x, y;
	}m_TopLeft, m_BottomLeft, m_TopRight, m_BottomRight;
};


class PS2SpriteT : public PS2Sprite
{
public:
	
	PS2SpriteT();
	
	PS2SpriteT(const float x, const float y);
	
	PS2SpriteT(const float x, const float y, const float width, const float height);
	
	PS2SpriteT(const float x, const float y, const float width, const float height, const int textwidth, const int textheight, const int textxcoord, const int textycoord);
	
	PS2SpriteT(const float x, const float y, const float z, const float width, const float height, const int textwidth, const int textheight, const int textxcoord, const int textycoord);
	
	~PS2SpriteT();
	
	void SprEdit(const float x, const float y, const float z, const float width, const float height, const int textwidth, const int textheight, const int textxcoord, const int textycoord);
	
	void SetAni(const bool cycle, const int numframes, const int f1xcoor, const int f1ycoor, const int cyclestochange, const int startoffsetframes);
	
	void PlayAni();
	
	void StopAni();
	
	void ChangeAniSpd(const int cyclestochange);
	
	void SetStatic(const int textxcoord, const int textycoord);
	
	void Animate(void);
	
	void Render(void) const;
	
	
protected:
	
	int m_tu, m_tv;				// Texture coordinates (top left)
	int m_tw, m_th;				// Texture coordinates (width and height)
	int m_AnimCounter;			// Animation counter
	int m_AnimCountMax;			// Number of cycles required before the next frame is displayed (set to 50 to display 1 frame of the naimation every second)
	int m_CurrentFrame;			// Current frame counter
	int m_NumFrames;			// N of frames in the animation
	bool m_AniFlag;				//Sets whether the sprite is animated or not
	bool m_AniCyclic;			//Sets whether the animation is cyclic
	
};
#endif