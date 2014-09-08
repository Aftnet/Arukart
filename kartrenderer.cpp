#include <sps2lib.h>
#include <sps2tags.h>
#include <sps2util.h>
#include <sps2regstructs.h>
#include <assert.h>
#include <memory.h>
#include <signal.h>
#include "PS2Defines.h"
#include "kartrenderer.h"
#include "texture.h"
#include "ps2maths.h"
#include "ps2matrix4x4.h"
#include "customdefines.h"
#include "sps2wrap.h"
#include "primitives.h"
#include "audio.h"
#include <stdio.h>
#include <iostream.h>
#include <string.h>

extern AudioDevice DSP0;

KartRenderer::KartRenderer(Kart *Kptr, const int nofKarts, const float Ksize, const float sd, const float eh, const float ed): Sprite(0,0,Ksize,Ksize,32,32,0,0)
{
	Karts = Kptr;
	nKarts = nofKarts;
	KartSize = Ksize;
	ScreenDistance = sd;
	EyeHeight = eh;
	EyeDistance = ed;
	Tempkarts = new Kart[nofKarts];
	hrefpts = new Position[nofKarts];
}

KartRenderer::~KartRenderer()
{
	delete Tempkarts;
	delete hrefpts;
}

void KartRenderer::GetPlayerPos(const Position x)
{
	PlayerPos = x;
}

bool KartRenderer::LoadTextures(const char *tx1, const char *tx2)
{
	bool success = true;
	if(!KTP[0].LoadBitmap(tx1, true, false))
	{
		success = false;
	}
	if(!KTP[1].LoadBitmap(tx2, true, false))
	{
		success = false;
	}
	return success;
}

void KartRenderer::Render()
{
	int i,j;
	float alpha = (0 - PlayerPos.alpha);
	float beta = (0 - PlayerPos.beta);
	float gamma = (0 - PlayerPos.gamma);
	float sinalpha = Sin(alpha);
	float cosalpha = Cos(alpha);
	float sinbeta = Sin(beta);
	float cosbeta = Cos(beta);
	float singamma = Sin(gamma);
	float cosgamma = Cos(gamma);
	float tmpx,tmpy,tmpz;
	float sprsize;
	int hoffset,voffset;
	bool revflag;
	Kart ksorthelper;
	float playeralpha=PlayerPos.alpha;
	int ash;
	Position hrsorthelper;
	for(i=0;i<nKarts;i++)
	{
		Tempkarts[i]=Karts[i];
	}
	//Translate according to player position
	for(i=0;i<nKarts;i++)
	{
		Tempkarts[i].x = (Tempkarts[i].x - PlayerPos.x);
		Tempkarts[i].y = (Tempkarts[i].y - PlayerPos.y);
		Tempkarts[i].z = (Tempkarts[i].z - PlayerPos.z);
	}
	//Rotate according to which direction the player is facing
	for(i=0;i<nKarts;i++)
	{
		tmpx = Tempkarts[i].x;
		tmpy = Tempkarts[i].y;
		tmpz = Tempkarts[i].z;
		Tempkarts[i].x = (((cosalpha*cosgamma - sinalpha*sinbeta*singamma)*tmpx) - ((cosbeta*singamma)*tmpy) + ((sinalpha*cosbeta + cosalpha*sinbeta*singamma)*tmpz));
		Tempkarts[i].y = (((cosalpha*singamma + sinalpha*sinbeta*cosgamma)*tmpx) + ((cosbeta*cosgamma)*tmpy) + ((sinalpha*sinbeta - cosalpha*sinbeta*cosgamma)*tmpz));
		Tempkarts[i].z = (((0 - sinalpha*cosbeta)*tmpx + (sinbeta*tmpy) + (cosalpha*cosbeta)*tmpz));
	}
	//Translate according to screen position relative to player
	for(i=0;i<nKarts;i++)
	{
		Tempkarts[i].z = (Tempkarts[i].z + ScreenDistance);
		Tempkarts[i].y = (Tempkarts[i].y - EyeHeight);
	}
	//Create height reference points
	for(i=0;i<nKarts;i++)
	{
		hrefpts[i].x = Tempkarts[i].x;
		hrefpts[i].y = (Tempkarts[i].y + KartSize);
		hrefpts[i].z = Tempkarts[i].z;
	}
	DSP0.HandleAudio();
	//Discard Karts outside field of vision
	for(i=0;i<nKarts;i++)
	{
		if(Tempkarts[i].z<0)
		{
			Tempkarts[i].visflag=false;
		}
		if(!(Tempkarts[i].z>=(((EyeDistance/320.0)*Tempkarts[i].x)-EyeDistance)))
		{
			Tempkarts[i].visflag=false;
		}
		if(!(Tempkarts[i].z>=((0-((EyeDistance/320.0)*Tempkarts[i].x))-EyeDistance)))
		{
			Tempkarts[i].visflag=false;
		}
		if(!(Tempkarts[i].y>=((0-((EyeDistance/320.0)*Tempkarts[i].z))-EyeDistance)))		
		{
			Tempkarts[i].visflag=false;
		}
		if(!(Tempkarts[i].y<=(((256/EyeDistance)*Tempkarts[i].z)+256)))			
		{
			Tempkarts[i].visflag=false;
		}
	}
	//Apply perspective transformation
	for(i=0;i<nKarts;i++)
	{
		if(Tempkarts[i].visflag==true)
		{
			Tempkarts[i].x = (Tempkarts[i].x*EyeDistance)/(Tempkarts[i].z+EyeDistance);
			Tempkarts[i].y = 0-((Tempkarts[i].y*EyeDistance)/(Tempkarts[i].z+EyeDistance));
			hrefpts[i].x = (hrefpts[i].x*EyeDistance)/(hrefpts[i].z+EyeDistance);
			hrefpts[i].y = 0-((hrefpts[i].y*EyeDistance)/(hrefpts[i].z+EyeDistance));
		}
	}
	//Pre-Render Angle Normalization
	for(i=0;i<nKarts;i++)
	{
		if(Tempkarts[i].alpha>0)
		{
			ash=(int)((Tempkarts[i].alpha+PI)/(2*PI));
			Tempkarts[i].alpha-=(((float)ash)*2*PI);
		}
		else
		{
			ash=(int)((Tempkarts[i].alpha-PI)/(2*PI));
			Tempkarts[i].alpha-=(((float)ash)*2*PI);
		}
	}
	DSP0.HandleAudio();
	playeralpha=Tempkarts[0].alpha;
	//Sort by z-depth
	for(i=0;i<nKarts;i++)
	{
		for(j=i;j<nKarts;j++)
		{
			if(Tempkarts[i].z<Tempkarts[j].z)
			{
				ksorthelper = Tempkarts[i];
				Tempkarts[i] = Tempkarts[j];
				Tempkarts[j] = ksorthelper;
				hrsorthelper = hrefpts[i];
				hrefpts[i] = hrefpts[j];
				hrefpts[j] = hrsorthelper;
			}
		}
	}
	//Render
	for(i=0;i<nKarts;i++)
	{
		//Angle calculation and renormalization
		Tempkarts[i].alpha-=playeralpha;
		if(Tempkarts[i].alpha>0)
		{
			ash=(int)((Tempkarts[i].alpha+PI)/(2*PI));
			Tempkarts[i].alpha-=(((float)ash)*2*PI);
		}
		else
		{
			ash=(int)((Tempkarts[i].alpha-PI)/(2*PI));
			Tempkarts[i].alpha-=(((float)ash)*2*PI);
		}
		//Rendering
		if(Tempkarts[i].visflag==true)
		{
			revflag=false;
			sprsize = Tempkarts[i].y - hrefpts[i].y;
			if(Tempkarts[i].charid<4)
			{
				KTP[0].Upload(TEXBUF480);
				KTP[0].Select();
			}
			else
			{
				KTP[1].Upload(TEXBUF480);
				KTP[1].Select();
			}
			Tempkarts[i].charid=Tempkarts[i].charid%4;
			voffset=64*Tempkarts[i].charid;
			hoffset=0;
			if(Tempkarts[i].alpha<0)
			{
				Tempkarts[i].alpha=(0-Tempkarts[i].alpha);
				revflag=true;
			}
			if(Tempkarts[i].alpha<(PI/28))
			{
				hoffset=0;
			}
			else if(Tempkarts[i].alpha<(3*(PI/28)))
			{
				hoffset=32;
			}
			else if(Tempkarts[i].alpha<(5*(PI/28)))
			{
				hoffset=64;
			}
			else if(Tempkarts[i].alpha<(7*(PI/28)))
			{
				hoffset=96;
			}
			else if(Tempkarts[i].alpha<(9*(PI/28)))
			{
				hoffset=128;
			}
			else if(Tempkarts[i].alpha<(11*(PI/28)))
			{
				hoffset=160;
			}
			else if(Tempkarts[i].alpha<(13*(PI/28)))
			{
				hoffset=192;
			}
			else if(Tempkarts[i].alpha<(9*(PI/16)))
			{
				hoffset=224;
			}
			else if(Tempkarts[i].alpha<(11*(PI/16)))
			{
				hoffset=0;
				voffset+=32;
			}
			else if(Tempkarts[i].alpha<(13*(PI/16)))
			{
				hoffset=32;
				voffset+=32;
			}
			else if(Tempkarts[i].alpha<(15*(PI/16)))
			{
				hoffset=64;
				voffset+=32;
			}
			else
			{
				hoffset=96;
				voffset+=32;
			}
			if(revflag==true)
			{
				Sprite.SprEdit(Tempkarts[i].x,(Tempkarts[i].y-(sprsize/2)),(10000-Tempkarts[i].z),(0-sprsize),sprsize,32,32,hoffset,voffset);
			}
			else
			{
				Sprite.SprEdit(Tempkarts[i].x,(Tempkarts[i].y-(sprsize/2)),(10000-Tempkarts[i].z),sprsize,sprsize,32,32,hoffset,voffset);
			}
			DSP0.HandleAudio();
			Sprite.Render();
		}
	}
}