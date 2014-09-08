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


PS2Sprite::PS2Sprite()
{
	m_x = m_y = 0.0f;
	m_w = m_h = 32.0f;
	m_Vx = m_Vy = 0.0f;
	m_r = m_g = m_b = m_a = 0x80;
	Initialise();
}

PS2Sprite::PS2Sprite(const float x, const float y)
{
	m_x = x; m_y = y;
	m_w = m_h = 32.0f;
	m_Vx = m_Vy = 0.0f;
	m_r = m_g = m_b = m_a = 0x80;
	Initialise();	
}

PS2Sprite::PS2Sprite(const float x, const float y, const float width, const float height)
{
	m_x = x; m_y = y;
	m_w = width;
	m_h = height;
	m_Vx = m_Vy = 0.0f;
	m_r = m_g = m_b = m_a = 0x80;
	Initialise();	
}

PS2Sprite::PS2Sprite(const float x, const float y, const float z, const float width, const float height)
{
	m_x = x; m_y = y;
	m_w = width;
	m_h = height;
	m_Vx = m_Vy = 0.0f;
	m_r = m_g = m_b = m_a = 0x80;
	Initialise();	
	m_z = z;
}

PS2Sprite::PS2Sprite(const float x, const float y, const uint8 rcolor, const uint8 gcolor, const uint8 bcolor, const uint8 alpha)
{
	m_x = x; m_y = y;
	m_w = m_h = 32.0f;
	m_Vx = m_Vy = 0.0f;
	m_r = rcolor;
	m_g = gcolor;
	m_b = bcolor;
	m_a = alpha;
	Initialise();	
}

PS2Sprite::PS2Sprite(const float x, const float y, const float width, const float height, const uint8 rcolor, const uint8 gcolor, const uint8 bcolor, const uint8 alpha)
{
	m_x = x; m_y = y;
	m_w = width;
	m_h = height;
	m_Vx = m_Vy = 0.0f;
	m_r = rcolor;
	m_g = gcolor;
	m_b = bcolor;
	m_a = alpha;
	Initialise();
	
}

PS2Sprite::~PS2Sprite()
{
	// Nothing to clean-up
}

void PS2Sprite::Initialise(void)
{
	m_z = 128.0f;
	m_xangle = m_yangle = m_zangle = 0.0f;
	m_xscale = m_yscale = 1.0f;
	m_radius = sqrt(((m_w*m_w)+(m_h*m_h))/4);
	
	m_TopLeft.x 		= -m_w/2.0f;
	m_TopLeft.y 		= -m_h/2.0f;
	m_BottomLeft.x 		= -m_w/2.0f;
	m_BottomLeft.y 		=  m_h/2.0f;
	m_TopRight.x 		=  m_w/2.0f;
	m_TopRight.y 		= -m_h/2.0f;
	m_BottomRight.x 	=  m_w/2.0f;
	m_BottomRight.y 	=  m_h/2.0f;
	m_bbhw = m_w/2.0f;
	m_bbhh = m_h/2.0f;
}

void PS2Sprite::ApplyTransforms(void)
{
	float tmpTopLeftx 		= -m_w/2.0f;
	float tmpTopLefty 		= -m_h/2.0f;
	float tmpBottomLeftx 	= -m_w/2.0f;
	float tmpBottomLefty 	=  m_h/2.0f;
	float tmpTopRightx 		=  m_w/2.0f;
	float tmpTopRighty 		= -m_h/2.0f;
	float tmpBottomRightx 	=  m_w/2.0f;
	float tmpBottomRighty 	=  m_h/2.0f;
	m_TopLeft.x = ((tmpTopLeftx*m_xscale*cos(m_zangle) - tmpTopLefty*m_yscale*sin(m_zangle))*cos(m_yangle));
	m_TopLeft.y = ((tmpTopLeftx*m_xscale*sin(m_zangle) + tmpTopLefty*m_yscale*cos(m_zangle))*cos(m_xangle));
	m_BottomLeft.x = ((tmpBottomLeftx*m_xscale*cos(m_zangle) - tmpBottomLefty*m_yscale*sin(m_zangle))*cos(m_yangle));
	m_BottomLeft.y = ((tmpBottomLeftx*m_xscale*sin(m_zangle) + tmpBottomLefty*m_yscale*cos(m_zangle))*cos(m_xangle));
	m_TopRight.x = ((tmpTopRightx*m_xscale*cos(m_zangle) - tmpTopRighty*m_yscale*sin(m_zangle))*cos(m_yangle));
	m_TopRight.y = ((tmpTopRightx*m_xscale*sin(m_zangle) + tmpTopRighty*m_yscale*cos(m_zangle))*cos(m_xangle));
	m_BottomRight.x = ((tmpBottomRightx*m_xscale*cos(m_zangle) - tmpBottomRighty*m_yscale*sin(m_zangle))*cos(m_yangle));
	m_BottomRight.y = ((tmpBottomRightx*m_xscale*sin(m_zangle) + tmpBottomRighty*m_yscale*cos(m_zangle))*cos(m_xangle));
	m_bbhw = m_TopLeft.x;
	if(m_BottomLeft.x > m_bbhw) {m_bbhw = m_BottomLeft.x;}
	if(m_TopRight.x > m_bbhw) {m_bbhw = m_TopRight.x;}
	if(m_BottomRight.x > m_bbhw) {m_bbhw = m_BottomRight.x;}
	m_bbhh = m_TopLeft.y;
	if(m_BottomLeft.y > m_bbhh) {m_bbhh = m_BottomLeft.y;}
	if(m_TopRight.y > m_bbhh) {m_bbhh = m_TopRight.y;}
	if(m_BottomRight.y > m_bbhh) {m_bbhh = m_BottomRight.y;}
}


// ********* Don't Change anything in this method
void PS2Sprite::Render(void) const
{
	VIFDynamicDMA.StartDirect();
	
	// Add the GIFTag
	VIFDynamicDMA.Add128(GS_GIFTAG_BATCH(	4,							// NLOOP
											1,							// EOP
											1,							// PRE
											GS_PRIM(PRIM_TRI_STRIP, 	// PRIM
												PRIM_IIP_FLAT, 
												PRIM_TME_OFF,
												PRIM_FGE_OFF, 
												PRIM_ABE_OFF, 
												PRIM_AA1_OFF, 
												PRIM_FST_UV, 
												PRIM_CTXT_CONTEXT1, 
												PRIM_FIX_NOFIXDDA),
											GIF_FLG_PACKED,					//FLG
											GS_BATCH_2(	GIF_REG_RGBAQ, 
														GIF_REG_XYZ2)));	//BATCH	
		
	// Add the colour (Top Left)
	VIFDynamicDMA.Add128(PACKED_RGBA(m_r ,m_g, m_b, m_a));
		    	
	// Add the position (Top Left)  	   	
	VIFDynamicDMA.Add128(PACKED_XYZ2((int)((2048.0f + m_x + m_TopLeft.x)*16.0f), 
									 (int)((2048.0f + m_y + m_TopLeft.y)*16.0f),  
									 (int)m_z, 
									  0));
	
	// Add the colour (Bottom Left)
	VIFDynamicDMA.Add128(PACKED_RGBA(m_r ,m_g, m_b, m_a));
		    	
	// Add the position (Bottom Left)  	
	VIFDynamicDMA.Add128(PACKED_XYZ2((int)((2048.0f + m_x + m_BottomLeft.x)*16.0f), 
									 (int)((2048.0f + m_y + m_BottomLeft.y)*16.0f),  
									 (int)m_z, 
									  0));
									  
	
	// Add the colour (Top Right)
	VIFDynamicDMA.Add128(PACKED_RGBA(m_r ,m_g, m_b, m_a));
		    	
	// Add the position (Top Right)  	
	VIFDynamicDMA.Add128(PACKED_XYZ2((int)((2048.0f + m_x + m_TopRight.x)*16.0f), 
									 (int)((2048.0f + m_y + m_TopRight.y)*16.0f),  
									 (int)m_z, 
									  0));

	// Add the colour (Bottom Right)
	VIFDynamicDMA.Add128(PACKED_RGBA(m_r ,m_g, m_b, m_a));
		    	
	// Add the position  (Bottom Right)  	
	VIFDynamicDMA.Add128(PACKED_XYZ2((int)((2048.0f + m_x + m_BottomRight.x)*16.0f), 
									 (int)((2048.0f + m_y + m_BottomRight.y)*16.0f),  
									 (int)m_z, 
									  0));
	
	
	VIFDynamicDMA.EndDirect();
}

void PS2Sprite::MoveTo(const float x, const float y)
{
	m_x = x; m_y = y;
}

void PS2Sprite::MoveDeltaX(const float x)
{
	m_x += x;
}

void PS2Sprite::MoveDeltaY(const float y)
{
	m_y += y;
}

void PS2Sprite::Move(void)
{
	m_x+=m_Vx;
	m_y+=m_Vy;
}

void PS2Sprite::SetSpd(const float Cx, const float Cy)
{
	m_Vx = Cx;
	m_Vy = Cy;
}

void PS2Sprite::SetColor(const uint8 rcolor, const uint8 gcolor, const uint8 bcolor, const uint8 alpha)
{
	m_r = rcolor;
	m_g = gcolor;
	m_b = bcolor;
	m_a = alpha;
}

void PS2Sprite::RotateX(const float nangle)
{
	m_xangle = nangle;
	this->ApplyTransforms();
}

void PS2Sprite::RotateY(const float nangle)
{
	m_yangle = nangle;
	this->ApplyTransforms();
}

void PS2Sprite::RotateZ(const float nangle)
{
	m_zangle = nangle;
	this->ApplyTransforms();
}

void PS2Sprite::Scale(const float nscale)
{
	m_xscale = m_yscale = nscale;
	this->ApplyTransforms();
}

void PS2Sprite::ScaleX(const float nxscale)
{
	m_xscale = nxscale;
	this->ApplyTransforms();
}

void PS2Sprite::ScaleY(const float nyscale)
{
	m_yscale = nyscale;
	this->ApplyTransforms();
}

float PS2Sprite::GetXPos(void) const
{
	return m_x;
}

float PS2Sprite::GetYPos(void) const
{
	return m_y;
}

float PS2Sprite::GetWidth(void) const
{
	return m_w;
}

float PS2Sprite::GetHeight(void) const
{
	return m_h;
}

float PS2Sprite::GetRadius(void) const
{
	return m_radius;
}

float PS2Sprite::GetBBhwidth(void) const
{
	return m_bbhw;
}

float PS2Sprite::GetBBhheight(void) const
{
	return m_bbhh;
}

float PS2Sprite::GetXSpd(void) const
{
	return m_Vx;
}

float PS2Sprite::GetYSpd(void) const
{
	return m_Vy;
}

float PS2Sprite::GetRColor(void) const
{
	return m_r;
}

float PS2Sprite::GetGColor(void) const
{
	return m_g;
}

float PS2Sprite::GetBColor(void) const
{
	return m_b;
}

float PS2Sprite::GetAlpha(void) const
{
	return m_a;
}

float PS2Sprite::GetXAngle(void) const
{
	return m_xangle;
}

float PS2Sprite::GetYAngle(void) const
{
	return m_yangle;
}

float PS2Sprite::GetZAngle(void) const
{
	return m_zangle;
}

float PS2Sprite::GetXScale(void) const
{
	return m_xscale;
}

float PS2Sprite::GetYScale(void) const
{
	return m_yscale;
}

float PS2Sprite::GetV1xpos(void) const
{
	return m_TopLeft.x;
}

float PS2Sprite::GetV1ypos(void) const
{
	return m_TopLeft.y;
}

float PS2Sprite::GetV2xpos(void) const
{
	return m_BottomLeft.x;
}

float PS2Sprite::GetV2ypos(void) const
{
	return m_BottomLeft.y;
}

float PS2Sprite::GetV3xpos(void) const
{
	return m_TopRight.x;
}

float PS2Sprite::GetV3ypos(void) const
{
	return m_TopRight.y;
}

float PS2Sprite::GetV4xpos(void) const
{
	return m_BottomRight.x;
}

float PS2Sprite::GetV4ypos(void) const
{
	return m_BottomRight.y;
}




PS2SpriteT::PS2SpriteT():
PS2Sprite()
{
	m_tu = 0;
	m_tv = 0;
	m_tw = 32;
	m_th = 32;	
	m_AniFlag = FALSE;
	m_AniCyclic = FALSE;
	m_AnimCounter = 0;			
	m_AnimCountMax = 0;
	m_NumFrames = 1;
	m_CurrentFrame = 0;
}
	
PS2SpriteT::PS2SpriteT(const float x, const float y):
PS2Sprite(x, y)
{
	m_tu = 0;
	m_tv = 0;
	m_tw = 32;
	m_th = 32;
	m_AniFlag = FALSE;
	m_AniCyclic = FALSE;
	m_AnimCounter = 0;			
	m_AnimCountMax = 0;
	m_NumFrames = 1;
	m_CurrentFrame = 0;
}

PS2SpriteT::PS2SpriteT(const float x, const float y, const float width, const float height):
PS2Sprite(x, y, width, height)
{
	m_tu = 0;
	m_tv = 0;
	m_tw = (int)width;
	m_th = (int)height;
	m_AniFlag = FALSE;
	m_AniCyclic = FALSE;
	m_AnimCounter = 0;			
	m_AnimCountMax = 0;
	m_NumFrames = 1;
	m_CurrentFrame = 0;
}

PS2SpriteT::PS2SpriteT(const float x, const float y, const float width, const float height, const int textwidth, const int textheight, const int textxcoord, const int textycoord):
PS2Sprite(x, y, width, height)
{
	m_tu = textxcoord;
	m_tv = textycoord;
	m_tw = textwidth;
	m_th = textheight;
	m_AniFlag = FALSE;
	m_AniCyclic = FALSE;
	m_AnimCounter = 0;			
	m_AnimCountMax = 0;
	m_NumFrames = 1;
	m_CurrentFrame = 0;
}

PS2SpriteT::PS2SpriteT(const float x, const float y, const float z, const float width, const float height, const int textwidth, const int textheight, const int textxcoord, const int textycoord):
PS2Sprite(x, y, z, width, height)
{
	m_tu = textxcoord;
	m_tv = textycoord;
	m_tw = textwidth;
	m_th = textheight;
	m_AniFlag = FALSE;
	m_AniCyclic = FALSE;
	m_AnimCounter = 0;			
	m_AnimCountMax = 0;
	m_NumFrames = 1;
	m_CurrentFrame = 0;
}

PS2SpriteT::~PS2SpriteT()
{
	// Nothing to destruct
}

void PS2SpriteT::SprEdit(const float x, const float y, const float z, const float width, const float height, const int textwidth, const int textheight, const int textxcoord, const int textycoord)
{
	m_x = x; m_y = y;
	m_w = width;
	m_h = height;
	m_Vx = m_Vy = 0.0f;
	m_tu = textxcoord;
	m_tv = textycoord;
	m_tw = textwidth;
	m_th = textheight;
	m_z = z;
	m_xangle = m_yangle = m_zangle = 0.0f;
	m_xscale = m_yscale = 1.0f;
	m_radius = sqrt(((m_w*m_w)+(m_h*m_h))/4);
	m_TopLeft.x 		= -m_w/2.0f;
	m_TopLeft.y 		= -m_h/2.0f;
	m_BottomLeft.x 		= -m_w/2.0f;
	m_BottomLeft.y 		=  m_h/2.0f;
	m_TopRight.x 		=  m_w/2.0f;
	m_TopRight.y 		= -m_h/2.0f;
	m_BottomRight.x 	=  m_w/2.0f;
	m_BottomRight.y 	=  m_h/2.0f;
	m_bbhw = m_w/2.0f;
	m_bbhh = m_h/2.0f;
	m_AniFlag = FALSE;
	m_AniCyclic = FALSE;
	m_AnimCounter = 0;			
	m_AnimCountMax = 0;
	m_NumFrames = 1;
	m_CurrentFrame = 0;
}
void PS2SpriteT::SetAni(const bool cycle, const int numframes, const int f1xcoor, const int f1ycoor, const int cyclestochange, const int startoffset)
{
	m_AniFlag = TRUE;
	m_AniCyclic = cycle;
	m_NumFrames = numframes;
	m_CurrentFrame = startoffset;
	m_tu = f1xcoor ;
	m_tv = f1ycoor + (m_th * startoffset);
	m_AnimCountMax = cyclestochange;
}

void PS2SpriteT::PlayAni()
{
	m_AniFlag = TRUE;
}

void PS2SpriteT::StopAni()
{
	m_AniFlag = FALSE;
}

void PS2SpriteT::ChangeAniSpd(const int cyclestochange)
{
	m_AnimCountMax = cyclestochange;
}

void PS2SpriteT::SetStatic(const int textxcoord, const int textycoord)
{
	m_AniFlag = FALSE;
	m_AniCyclic = FALSE;
	m_tu = textxcoord;
	m_tv = textycoord;
	m_AnimCountMax = 0;
	m_AnimCounter = 0;
	m_NumFrames = 1;
	m_CurrentFrame = 0;
}

void PS2SpriteT::Animate()
{
	if(m_AniFlag == TRUE)
	{
		if(m_AnimCounter >= m_AnimCountMax)
		{
			m_CurrentFrame++;
			m_AnimCounter = 0;
			if(m_CurrentFrame == m_NumFrames)
			{
				if(m_AniCyclic == TRUE)
				{
					m_tv -= m_th*(m_NumFrames - 1);
				}
				else
				{
					m_AniFlag == FALSE;
					m_NumFrames = 1;
					m_AnimCountMax = 0;
				}
				m_CurrentFrame = 0;
			}
			else
			{
				m_tv += m_th;
			}
		}
		m_AnimCounter++;
	}
}

// ********* Don't Change anything in this method
void PS2SpriteT::Render(void) const
{
	VIFDynamicDMA.StartDirect();
	
	// Add the GIFTag
	VIFDynamicDMA.Add128(GS_GIFTAG_BATCH(	4,							// NLOOP
											1,							// EOP
											1,							// PRE
											GS_PRIM(PRIM_TRI_STRIP, 	// PRIM
												PRIM_IIP_FLAT, 
												PRIM_TME_ON,
												PRIM_FGE_OFF, 
												PRIM_ABE_ON, 
												PRIM_AA1_OFF, 
												PRIM_FST_UV, 
												PRIM_CTXT_CONTEXT1, 
												PRIM_FIX_NOFIXDDA),
											GIF_FLG_PACKED,					//FLG
											GS_BATCH_3(	GIF_REG_RGBAQ,
														GIF_REG_UV, 
														GIF_REG_XYZ2)));	//BATCH	
		
	// Add the colour (Top Left)
	VIFDynamicDMA.Add128(PACKED_RGBA(m_r ,m_g, m_b, m_a));
	
	// Add UV texture coordinates (Top Left)
    VIFDynamicDMA.Add128(PACKED_UV(	((m_tu)<<4), ((m_tv)<<4)));
		    	
	// Add the position (Top Left)  	   	
	VIFDynamicDMA.Add128(PACKED_XYZ2((int)((2048.0f + m_x + m_TopLeft.x)*16.0f), 
									 (int)((2048.0f + m_y + m_TopLeft.y)*16.0f),  
									 (int)m_z, 
									  0));
	
	// Add the colour (Bottom Left)
	VIFDynamicDMA.Add128(PACKED_RGBA(m_r ,m_g, m_b, m_a));
	
	// Add UV texture coordinates (Bottom Left)
    VIFDynamicDMA.Add128(PACKED_UV(	(m_tu)<<4, (m_tv + m_th)<<4));
	
		    	
	// Add the position (Bottom Left)  	
	VIFDynamicDMA.Add128(PACKED_XYZ2((int)((2048.0f + m_x + m_BottomLeft.x)*16.0f), 
									 (int)((2048.0f + m_y + m_BottomLeft.y)*16.0f),  
									 (int)m_z, 
									  0));
									  
	
	// Add the colour (Top Right)
	VIFDynamicDMA.Add128(PACKED_RGBA(m_r ,m_g, m_b, m_a));
	
	// Add UV texture coordinates (Top Right)
    VIFDynamicDMA.Add128(PACKED_UV(	(m_tu + m_tw)<<4, (m_tv)<<4));
		    	
	// Add the position (Top Right)  	
	VIFDynamicDMA.Add128(PACKED_XYZ2((int)((2048.0f + m_x + m_TopRight.x)*16.0f), 
									 (int)((2048.0f + m_y + m_TopRight.y)*16.0f),  
									 (int)m_z, 
									  0));

	// Add the colour (Bottom Right)
	VIFDynamicDMA.Add128(PACKED_RGBA(m_r ,m_g, m_b, m_a));
	
	// Add UV texture coordinates
    VIFDynamicDMA.Add128(PACKED_UV(	(m_tu + m_tw)<<4, (m_tv + m_th)<<4));
		    	
	// Add the position  (Bottom Right)  	
	VIFDynamicDMA.Add128(PACKED_XYZ2((int)((2048.0f + m_x + m_BottomRight.x)*16.0f), 
									 (int)((2048.0f + m_y + m_BottomRight.y)*16.0f),  
									 (int)m_z, 
									  0));
	
	
	VIFDynamicDMA.EndDirect();
}


