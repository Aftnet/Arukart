#include <sps2lib.h>
#include <sps2tags.h>
#include <sps2util.h>
#include <sps2regstructs.h>
#include <stdio.h>
#include "PS2Defines.h"
#include "ps2matrix4x4.h"
#include "sps2wrap.h"
#include "dma.h"
#include "3dprimitives.h"

PS23DSprite::PS23DSprite()
{
	m_r = m_g = m_b = m_a = 0x80;
	m_z = 128.0f;
	
	m_TopLeft.x 		= 0;
	m_TopLeft.y 		= 0;
	m_TopLeft.z 		= 0;
	m_BottomLeft.x 		= 0;
	m_BottomLeft.y 		= 0;
	m_BottomLeft.z 		= 0;
	m_TopRight.x 		= 0;
	m_TopRight.y 		= 0;
	m_TopRight.z 		= 0;
	m_BottomRight.x 	= 0;
	m_BottomRight.y 	= 0;
	m_BottomRight.z 	= 0;
	
	m_x = m_y = 0;
}

PS23DSprite::PS23DSprite(const float v1x, const float v1y, const float v1z, const float v2x, const float v2y, const float v2z, const float v3x, const float v3y, const float v3z, const float v4x, const float v4y, const float v4z)
{
	m_r = m_g = m_b = m_a = 0x80;
	m_z = 128.0f;
	
	m_TopLeft.x 		= v1x;
	m_TopLeft.y 		= v1y;
	m_TopLeft.z 		= v1z;
	m_BottomLeft.x 		= v2x;
	m_BottomLeft.y 		= v2y;
	m_BottomLeft.z 		= v2z;
	m_TopRight.x 		= v3x;
	m_TopRight.y 		= v3y;
	m_TopRight.z 		= v3z;
	m_BottomRight.x 	= v4x;
	m_BottomRight.y 	= v4y;
	m_BottomRight.z 	= v4z;
	
	m_x = m_y = 0;
}

PS23DSprite::~PS23DSprite()
{
	// Nothing to destruct
}

void PS23DSprite::Initialise(void)
{
	m_z = 128.0f;
	m_w = m_h = 200.0f;
	m_r = m_g = m_b = m_a = 0x80;
	
	m_TopLeft.x 		= -m_w/4.0f;
	m_TopLeft.y 		= -m_h/2.0f;
	m_TopLeft.z 		= 0.5f;
	m_BottomLeft.x 		= -m_w/2.0f;
	m_BottomLeft.y 		=  m_h/2.0f;
	m_BottomLeft.z 		=  1.0f;
	m_TopRight.x 		=  m_w/4.0f;
	m_TopRight.y 		= -m_h/2.0f;
	m_TopRight.z 		= 0.5f;
	m_BottomRight.x 	=  m_w/2.0f;
	m_BottomRight.y 	=  m_h/2.0f;
	m_BottomRight.z 	=  1.0f;
}


// ********* Don't Change anything in this method
void PS23DSprite::Render(void) const
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

void PS23DSprite::Edit(const float v1x, const float v1y, const float v1z, const float v2x, const float v2y, const float v2z, const float v3x, const float v3y, const float v3z, const float v4x, const float v4y, const float v4z)
{
	m_TopLeft.x 		= v1x;
	m_TopLeft.y 		= v1y;
	m_TopLeft.z 		= v1z;
	m_BottomLeft.x 		= v2x;
	m_BottomLeft.y 		= v2y;
	m_BottomLeft.z 		= v2z;
	m_TopRight.x 		= v3x;
	m_TopRight.y 		= v3y;
	m_TopRight.z 		= v3z;
	m_BottomRight.x 	= v4x;
	m_BottomRight.y 	= v4y;
	m_BottomRight.z 	= v4z;
}

/*void PS23DSprite::MoveTo(const float x, const float y)
{
	m_x = x;
	m_y = y;
}*/

PS23DSpriteT::PS23DSpriteT():
PS23DSprite()
{
	m_TopLeft.tnx 		= 0.0f;
	m_TopLeft.tny 		= 0.0f;
	m_BottomLeft.tnx 	= 0.0f;
	m_BottomLeft.tny 	= 1.0f;
	m_TopRight.tnx 		= 1.0f;
	m_TopRight.tny 		= 0.0f;
	m_BottomRight.tnx 	= 1.0f;
	m_BottomRight.tny 	= 1.0f;
}

PS23DSpriteT::PS23DSpriteT(const float v1x, const float v1y, const float v1z, const float v1tx, const float v1ty, const float v2x, float v2y, const float v2z, const float v2tx, const float v2ty, const float v3x, const float v3y, const float v3z, const float v3tx, const float v3ty, const float v4x, const float v4y, const float v4z, const float v4tx, const float v4ty):
PS23DSprite(v1x, v1y, v1z, v2x, v2y, v2z, v3x, v3y, v3z, v4x, v4y, v4z)
{
	m_TopLeft.tnx 		= v1tx;
	m_TopLeft.tny 		= v1ty;
	m_BottomLeft.tnx 	= v2tx;
	m_BottomLeft.tny 	= v2ty;
	m_TopRight.tnx 		= v3tx;
	m_TopRight.tny 		= v3ty;
	m_BottomRight.tnx 	= v4tx;
	m_BottomRight.tny 	= v4ty;
}

PS23DSpriteT::~PS23DSpriteT()
{
	// Nothing to destruct
}

void PS23DSpriteT::Edit(const float v1x, const float v1y, const float v1z, const float v1tx, const float v1ty, const float v2x, float v2y, const float v2z, const float v2tx, const float v2ty, const float v3x, const float v3y, const float v3z, const float v3tx, const float v3ty, const float v4x, const float v4y, const float v4z, const float v4tx, const float v4ty)
{
	m_TopLeft.x 		= v1x;
	m_TopLeft.y 		= v1y;
	m_TopLeft.z 		= v1z;
	m_BottomLeft.x 		= v2x;
	m_BottomLeft.y 		= v2y;
	m_BottomLeft.z 		= v2z;
	m_TopRight.x 		= v3x;
	m_TopRight.y 		= v3y;
	m_TopRight.z 		= v3z;
	m_BottomRight.x 	= v4x;
	m_BottomRight.y 	= v4y;
	m_BottomRight.z 	= v4z;
	
	m_TopLeft.tnx 		= v1tx;
	m_TopLeft.tny 		= v1ty;
	m_BottomLeft.tnx 	= v2tx;
	m_BottomLeft.tny 	= v2ty;
	m_TopRight.tnx 		= v3tx;
	m_TopRight.tny 		= v3ty;
	m_BottomRight.tnx 	= v4tx;
	m_BottomRight.tny 	= v4ty;
}

float PS23DSpriteT::GetVertexData(const int vertexno, const char attr) const
{
	if(vertexno==0)
	{
		if(attr=='x')
		{
			return m_TopLeft.x;
		}
		else if(attr=='y')
		{
			return m_TopLeft.y;
		}
		else if(attr=='z')
		{
			return m_TopLeft.z;
		}
		else if(attr=='h')
		{
			return m_TopLeft.tnx;
		}
		else if(attr=='k')
		{
			return m_TopLeft.tny;
		}
		else
		{
			printf("error in attribute requested\n");
			return 0;
		}
	}
	else if(vertexno==1)
	{
		if(attr=='x')
		{
			return m_BottomLeft.x;
		}
		else if(attr=='y')
		{
			return m_BottomLeft.y;
		}
		else if(attr=='z')
		{
			return m_BottomLeft.z;
		}
		else if(attr=='h')
		{
			return m_BottomLeft.tnx;
		}
		else if(attr=='k')
		{
			return m_BottomLeft.tny;
		}
		else
		{
			printf("error in attribute requested\n");
			return 0;
		}
	}
	else if(vertexno==2)
	{
		if(attr=='x')
		{
			return m_TopRight.x;
		}
		else if(attr=='y')
		{
			return m_TopRight.y;
		}
		else if(attr=='z')
		{
			return m_TopRight.z;
		}
		else if(attr=='h')
		{
			return m_TopRight.tnx;
		}
		else if(attr=='k')
		{
			return m_TopRight.tny;
		}
		else
		{
			printf("error in attribute requested\n");
			return 0;
		}
	}
	else if(vertexno==3)
	{
		if(attr=='x')
		{
			return m_BottomRight.x;
		}
		else if(attr=='y')
		{
			return m_BottomRight.y;
		}
		else if(attr=='z')
		{
			return m_BottomRight.z;
		}
		else if(attr=='h')
		{
			return m_BottomRight.tnx;
		}
		else if(attr=='k')
		{
			return m_BottomRight.tny;
		}
		else
		{
			printf("error in attribute requested\n");
			return 0;
		}
	}
	else
	{
		printf("Error: request of a nonexistent vertex");
		return 0;
	}
}

// ********* Don't Change anything in this method
void PS23DSpriteT::Render(void) const
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
												PRIM_FST_STQ, 
												PRIM_CTXT_CONTEXT1, 
												PRIM_FIX_NOFIXDDA),
											GIF_FLG_PACKED,					//FLG
											GS_BATCH_3(	GIF_REG_ST,
														GIF_REG_RGBAQ, 
														GIF_REG_XYZ2)));	//BATCH	
		
	// Add STQ texture coordinates (Top Left)
	VIFDynamicDMA.AddVectorF(m_TopLeft.tnx*m_TopLeft.z, m_TopLeft.tny*m_TopLeft.z, m_TopLeft.z, 0.0f);
	
	// Add the colour (Top Left)
	VIFDynamicDMA.Add128(PACKED_RGBA(m_r ,m_g, m_b, m_a));
	    	
	// Add the position (Top Left)  	   	
	VIFDynamicDMA.Add128(PACKED_XYZ2((int)((2048.0f + m_x + m_TopLeft.x)*16.0f), 
									 (int)((2048.0f + m_y + m_TopLeft.y)*16.0f),  
									 (int)m_z, 
									  0));
	


	
	// Add STQ texture coordinates (Bottom Left)
	VIFDynamicDMA.AddVectorF(m_BottomLeft.tnx*m_BottomLeft.z, m_BottomLeft.tny*m_BottomLeft.z, m_BottomLeft.z, 0.0f);
    
	// Add the colour (Bottom Left)    
	VIFDynamicDMA.Add128(PACKED_RGBA(m_r ,m_g, m_b, m_a));
		    	
	// Add the position (Bottom Left)  	
	VIFDynamicDMA.Add128(PACKED_XYZ2((int)((2048.0f + m_x + m_BottomLeft.x)*16.0f), 
									 (int)((2048.0f + m_y + m_BottomLeft.y)*16.0f),  
									 (int)m_z, 
									  0));
									  
	


	
	// Add STQ texture coordinates (Top Right)
	VIFDynamicDMA.AddVectorF(m_TopRight.tnx*m_TopRight.z, m_TopRight.tny*m_TopRight.z, m_TopRight.z, 0.0f);
    
    // Add the colour (Top Right)
	VIFDynamicDMA.Add128(PACKED_RGBA(m_r ,m_g, m_b, m_a));
	    	
	// Add the position (Top Right)  	
	VIFDynamicDMA.Add128(PACKED_XYZ2((int)((2048.0f + m_x + m_TopRight.x)*16.0f), 
									 (int)((2048.0f + m_y + m_TopRight.y)*16.0f),  
									 (int)m_z, 
									  0));



	
	// Add STQ texture coordinates
	VIFDynamicDMA.AddVectorF(m_BottomRight.tnx*m_BottomRight.z, m_BottomRight.tny*m_BottomRight.z, m_BottomRight.z, 0.0f);

    // Add the colour (Bottom Right)
	VIFDynamicDMA.Add128(PACKED_RGBA(m_r ,m_g, m_b, m_a));	
	    	
	// Add the position  (Bottom Right)  	
	VIFDynamicDMA.Add128(PACKED_XYZ2((int)((2048.0f + m_x + m_BottomRight.x)*16.0f), 
									 (int)((2048.0f + m_y + m_BottomRight.y)*16.0f),  
									 (int)m_z, 
									  0));
	
	VIFDynamicDMA.EndDirect();
}