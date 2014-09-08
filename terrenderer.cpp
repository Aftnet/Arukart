#include <sps2lib.h>
#include <sps2tags.h>
#include <sps2util.h>
#include <sps2regstructs.h>
#include <assert.h>
#include <memory.h>
#include <signal.h>
#include "PS2Defines.h"
#include "terrenderer.h"
#include "texture.h"
#include "ps2maths.h"
#include "ps2matrix4x4.h"
#include "customdefines.h"
#include "sps2wrap.h"
#include "audio.h"
#include <stdio.h>
#include <iostream.h>
#include <string.h>


extern AudioDevice DSP0;

TerRenderer::TerRenderer(const int x, const int y, const int sub, const float side, const float sd, const float eh, const float ed)
{
	int i,j,k,l;
	NumTiles = x*y;
	SubDivs = sub*sub;
	TextureStep=float(1/float(sub));
	SideLenght=side;
	TileArray = new Tile[NumTiles*SubDivs];
	TempTiles = new Tile[NumTiles*SubDivs];
	for(i=0;i<y;i++)
	{
		for(j=0;j<x;j++)
		{
			for(k=0;k<sub;k++)
			{
				for(l=0;l<sub;l++)
				{
					TileArray[(x*i+j)*(SubDivs)+(sub*k+l)].Vertex[0].x = (sub*side*j)+l*side;
					TileArray[(x*i+j)*(SubDivs)+(sub*k+l)].Vertex[0].y = 0;
					TileArray[(x*i+j)*(SubDivs)+(sub*k+l)].Vertex[0].z = (sub*side*i)+k*side;
					TileArray[(x*i+j)*(SubDivs)+(sub*k+l)].Vertex[0].tnx = TextureStep*l;
					TileArray[(x*i+j)*(SubDivs)+(sub*k+l)].Vertex[0].tny = 1-(TextureStep*k);
					TileArray[(x*i+j)*(SubDivs)+(sub*k+l)].Vertex[1].x = (sub*side*j)+(l+1)*side;
					TileArray[(x*i+j)*(SubDivs)+(sub*k+l)].Vertex[1].y = 0;
					TileArray[(x*i+j)*(SubDivs)+(sub*k+l)].Vertex[1].z = (sub*side*i)+k*side;
					TileArray[(x*i+j)*(SubDivs)+(sub*k+l)].Vertex[1].tnx = TextureStep*(l+1);
					TileArray[(x*i+j)*(SubDivs)+(sub*k+l)].Vertex[1].tny = 1-(TextureStep*k);
					TileArray[(x*i+j)*(SubDivs)+(sub*k+l)].Vertex[2].x = (sub*side*j)+(l+1)*side;
					TileArray[(x*i+j)*(SubDivs)+(sub*k+l)].Vertex[2].y = 0;
					TileArray[(x*i+j)*(SubDivs)+(sub*k+l)].Vertex[2].z = (sub*side*i)+(k+1)*side;
					TileArray[(x*i+j)*(SubDivs)+(sub*k+l)].Vertex[2].tnx = TextureStep*(l+1);
					TileArray[(x*i+j)*(SubDivs)+(sub*k+l)].Vertex[2].tny = 1-(TextureStep*(k+1));
					TileArray[(x*i+j)*(SubDivs)+(sub*k+l)].Vertex[3].x = (sub*side*j)+l*side;
					TileArray[(x*i+j)*(SubDivs)+(sub*k+l)].Vertex[3].y = 0;
					TileArray[(x*i+j)*(SubDivs)+(sub*k+l)].Vertex[3].z = (sub*side*i)+(k+1)*side;
					TileArray[(x*i+j)*(SubDivs)+(sub*k+l)].Vertex[3].tnx = TextureStep*l;
					TileArray[(x*i+j)*(SubDivs)+(sub*k+l)].Vertex[3].tny = 1-(TextureStep*(k+1));
				}
			}
		}
	}
	TextureArray = new CTexture[NumTiles];
	SPRarray = new PS23DSpriteT[2*NumTiles*SubDivs];
	TextureIndex = new int [2*NumTiles*SubDivs];
	ScreenDistance = sd;
	EyeHeight = eh;
	EyeDistance = ed;
}

TerRenderer::~TerRenderer()
{
	delete[] TileArray;
	delete[] TextureArray;
	delete[] TempTiles;
	delete[] SPRarray; 
	delete[] TextureIndex;
}

void TerRenderer::GetPlayerPos(const Position x)
{
	PlayerPos = x;
}

bool TerRenderer::LoadTextures(char * Fileroot)
{
	bool success = true;
	int i,j;
	char tmp[20];
	char digits[3];
	i=0;
	while(i<19 && Fileroot[i]!='\0')
	{
		tmp[i]=Fileroot[i];
		i++;	
	}
	while(i<20)
	{
		tmp[i]='\0';
		i++;
	}
	i=0;
	while(tmp[i]!='\0')
	{
		i++;
	}
	for(j=0;j<NumTiles;j++)
	{
		sprintf(digits,"%d",j);
		if(i<12)
		{
			tmp[i]='_';
			if(j<10)
			{
				tmp[i+1]='0';
				tmp[i+2]='0';
				tmp[i+3]=digits[0];
				tmp[i+4]='.';
				tmp[i+5]='b';
				tmp[i+6]='m';
				tmp[i+7]='p';
				tmp[i+8]='\0';
			}
			else if(j<100)
			{
				tmp[i+1]='0';
				tmp[i+2]=digits[0];
				tmp[i+3]=digits[1];
				tmp[i+4]='.';
				tmp[i+5]='b';
				tmp[i+6]='m';
				tmp[i+7]='p';
				tmp[i+8]='\0';
			}
			else
			{
				tmp[i+1]=digits[0];
				tmp[i+2]=digits[1];
				tmp[i+3]=digits[2];
				tmp[i+4]='.';
				tmp[i+5]='b';
				tmp[i+6]='m';
				tmp[i+7]='p';
				tmp[i+8]='\0';
			}
		}
		else
		{
			tmp[11]='_';
			if(j<10)
			{
				tmp[12]='0';
				tmp[13]='0';
				tmp[14]=digits[0];
				tmp[15]='.';
				tmp[16]='b';
				tmp[17]='m';
				tmp[18]='p';
				tmp[19]='\0';
			}
			else if(j<100)
			{
				tmp[12]='0';
				tmp[13]=digits[0];
				tmp[14]=digits[1];
				tmp[15]='.';
				tmp[16]='b';
				tmp[17]='m';
				tmp[18]='p';
				tmp[19]='\0';
			}
			else
			{
				tmp[12]=digits[0];
				tmp[13]=digits[1];
				tmp[14]=digits[2];
				tmp[15]='.';
				tmp[16]='b';
				tmp[17]='m';
				tmp[18]='p';
				tmp[19]='\0';
			}
		}
		if(!TextureArray[j].LoadBitmap(tmp, true, false))
		{
			success = false;
		}
	}
	return success;
}

void TerRenderer::Render()
{

	int i,j,k;
	int tmpindex;
	float t;
	int validvertices;
	int sprno = 0;
	float tmpdst;
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
	float tmpvx[4];
	float tmpvy[4];
	float tmpvz[4];
	float tmpvdistance[4];
	tVertex AuxV[2];
	float c1,c2,c3,c4,c5,c6,c7,c8,c9;
	int Totaliterates=NumTiles*SubDivs;
	for(i=0;i<Totaliterates;i++)
	{
		for(j=0;j<4;j++)
		{
			TempTiles[i].Vertex[j].tnx = TileArray[i].Vertex[j].tnx;
			TempTiles[i].Vertex[j].tny = TileArray[i].Vertex[j].tny;
		}
	}
	//Translate according to Player Position
	for(i=0;i<Totaliterates;i++)
	{
		for(j=0;j<4;j++)
		{
			TempTiles[i].Vertex[j].x = (TileArray[i].Vertex[j].x - PlayerPos.x);
			TempTiles[i].Vertex[j].y = (TileArray[i].Vertex[j].y - PlayerPos.y);
			TempTiles[i].Vertex[j].z = (TileArray[i].Vertex[j].z - PlayerPos.z);
		}
	}
	//Rotate according to which direction the player is facing
	if(beta==0 && gamma==0)
	{
		for(i=0;i<Totaliterates;i++)
		{
			for(j=0;j<4;j++)
			{
			tmpx = TempTiles[i].Vertex[j].x;
			tmpz = TempTiles[i].Vertex[j].z;
			TempTiles[i].Vertex[j].x = (cosalpha*tmpx) + (sinalpha*tmpz);
			TempTiles[i].Vertex[j].z = ((0-sinalpha)*tmpx) + (cosalpha*tmpz);
			}
		}
	}
	else
	{
		c1=(cosalpha*cosgamma - sinalpha*sinbeta*singamma);
		c2=(cosbeta*singamma);
		c3=(sinalpha*cosbeta + cosalpha*sinbeta*singamma);
		c4=(cosalpha*singamma + sinalpha*sinbeta*cosgamma);
		c5=(cosbeta*cosgamma);
		c6=(sinalpha*sinbeta - cosalpha*sinbeta*cosgamma);
		c7=(0 - sinalpha*cosbeta);
		c8=sinbeta;
		c9=(cosalpha*cosbeta);
		for(i=0;i<Totaliterates;i++)
		{
			for(j=0;j<4;j++)
			{
				tmpx = TempTiles[i].Vertex[j].x;
				tmpy = TempTiles[i].Vertex[j].y;
				tmpz = TempTiles[i].Vertex[j].z;
				TempTiles[i].Vertex[j].x = ((c1*tmpx) - (c2*tmpy) + (c3*tmpz));
				TempTiles[i].Vertex[j].y = ((c4*tmpx) + (c5*tmpy) + (c6*tmpz));
				TempTiles[i].Vertex[j].z = ((c7*tmpx) + (c8*tmpy) + (c9*tmpz));
			}
		}
	}
	//Translate according to screen position relative to player
	for(i=0;i<Totaliterates;i++)
	{
		for(j=0;j<4;j++)
		{
			TempTiles[i].Vertex[j].z = (TempTiles[i].Vertex[j].z + ScreenDistance);
			TempTiles[i].Vertex[j].y = (TempTiles[i].Vertex[j].y - EyeHeight);
		}
	}
	DSP0.HandleAudio();
	//Clip and create a list of sprites (this is going to cause me issues) working out the proper normalized texture coordinates
	for(i=0;i<NumTiles;i++)
	{
		for(k=0;k<SubDivs;k++)
		{
			tmpindex=i*SubDivs+k;
			validvertices=0;
			for(j=0;j<4;j++)
			{
				if(TempTiles[tmpindex].Vertex[j].z>=(((EyeDistance/320.0)*TempTiles[tmpindex].Vertex[j].x)-EyeDistance))
				{
					validvertices++;
				}
			}
			if(validvertices>0)
			{
				validvertices=0;
				for(j=0;j<4;j++)
				{
					if(TempTiles[tmpindex].Vertex[j].z>=((0-((EyeDistance/320.0)*TempTiles[tmpindex].Vertex[j].x))-EyeDistance))
					{
						validvertices++;
					}
				}
				if(validvertices>0)
				{
					validvertices=0;
					for(j=0;j<4;j++)
					{
						if(TempTiles[tmpindex].Vertex[j].y>=((0-((EyeDistance/320.0)*TempTiles[tmpindex].Vertex[j].z))-EyeDistance))					
						{
							validvertices++;
						}
					}
					if(validvertices>0)
					{
						validvertices=0;
						for(j=0;j<4;j++)
						{
							if(TempTiles[tmpindex].Vertex[j].y<=(((256/EyeDistance)*TempTiles[tmpindex].Vertex[j].z)+256))						
							{
								validvertices++;
							}
						}
						if(validvertices>0)
						{
							validvertices=0;
							for(j=0;j<4;j++)
							{
								if(TempTiles[tmpindex].Vertex[j].z>=0)
								{
									validvertices++;
								}
							}
							// 4 valid vertices: no clipping needed, display as is.
							if(validvertices==4)
							{
								SPRarray[sprno].Edit(TempTiles[tmpindex].Vertex[3].x, TempTiles[tmpindex].Vertex[3].y, TempTiles[tmpindex].Vertex[3].z, TempTiles[tmpindex].Vertex[3].tnx, TempTiles[tmpindex].Vertex[3].tny, TempTiles[tmpindex].Vertex[0].x, TempTiles[tmpindex].Vertex[0].y, TempTiles[tmpindex].Vertex[0].z, TempTiles[tmpindex].Vertex[0].tnx, TempTiles[tmpindex].Vertex[0].tny, TempTiles[tmpindex].Vertex[2].x, TempTiles[tmpindex].Vertex[2].y, TempTiles[tmpindex].Vertex[2].z, TempTiles[tmpindex].Vertex[2].tnx, TempTiles[tmpindex].Vertex[2].tny, TempTiles[tmpindex].Vertex[1].x, TempTiles[tmpindex].Vertex[1].y, TempTiles[tmpindex].Vertex[1].z, TempTiles[tmpindex].Vertex[1].tnx, TempTiles[tmpindex].Vertex[1].tny);
								TextureIndex[sprno] = i;
								sprno++;
							}
							// 3 valid vertices
							else if(validvertices==3)
							{
								if(TempTiles[tmpindex].Vertex[0].z<0)
								{
									t=TempTiles[tmpindex].Vertex[3].z/(TempTiles[tmpindex].Vertex[3].z-TempTiles[tmpindex].Vertex[0].z);
									AuxV[0].x=TempTiles[tmpindex].Vertex[3].x+t*(TempTiles[tmpindex].Vertex[0].x-TempTiles[tmpindex].Vertex[3].x);
									AuxV[0].y=TempTiles[tmpindex].Vertex[3].y+t*(TempTiles[tmpindex].Vertex[0].y-TempTiles[tmpindex].Vertex[3].y);
									AuxV[0].z=0;
									AuxV[0].tnx=TempTiles[tmpindex].Vertex[3].tnx;
									AuxV[0].tny=TempTiles[tmpindex].Vertex[3].tny+(t*TextureStep);
									t=TempTiles[tmpindex].Vertex[1].z/(TempTiles[tmpindex].Vertex[1].z-TempTiles[tmpindex].Vertex[0].z);
									AuxV[1].x=TempTiles[tmpindex].Vertex[1].x+t*(TempTiles[tmpindex].Vertex[0].x-TempTiles[tmpindex].Vertex[1].x);
									AuxV[1].y=TempTiles[tmpindex].Vertex[1].y+t*(TempTiles[tmpindex].Vertex[0].y-TempTiles[tmpindex].Vertex[1].y);
									AuxV[1].z=0;
									AuxV[1].tnx=TempTiles[tmpindex].Vertex[3].tnx+(1-t)*TextureStep;
									AuxV[1].tny=TempTiles[tmpindex].Vertex[3].tny+TextureStep;
									t=TempTiles[tmpindex].Vertex[2].z/(TempTiles[tmpindex].Vertex[2].z-TempTiles[tmpindex].Vertex[0].z);
									TempTiles[tmpindex].Vertex[0].x=TempTiles[tmpindex].Vertex[2].x+t*(TempTiles[tmpindex].Vertex[0].x-TempTiles[tmpindex].Vertex[2].x);
									TempTiles[tmpindex].Vertex[0].y=TempTiles[tmpindex].Vertex[2].y+t*(TempTiles[tmpindex].Vertex[0].y-TempTiles[tmpindex].Vertex[2].y);
									TempTiles[tmpindex].Vertex[0].z=0;
									TempTiles[tmpindex].Vertex[0].tnx=TempTiles[tmpindex].Vertex[3].tnx+(1-t)*TextureStep;
									TempTiles[tmpindex].Vertex[0].tny=TempTiles[tmpindex].Vertex[3].tny+t*TextureStep;
									
									SPRarray[sprno].Edit(TempTiles[tmpindex].Vertex[3].x, TempTiles[tmpindex].Vertex[3].y, TempTiles[tmpindex].Vertex[3].z, TempTiles[tmpindex].Vertex[3].tnx, TempTiles[tmpindex].Vertex[3].tny, AuxV[0].x, AuxV[0].y, AuxV[0].z, AuxV[0].tnx, AuxV[0].tny, TempTiles[tmpindex].Vertex[2].x, TempTiles[tmpindex].Vertex[2].y, TempTiles[tmpindex].Vertex[2].z, TempTiles[tmpindex].Vertex[2].tnx, TempTiles[tmpindex].Vertex[2].tny, TempTiles[tmpindex].Vertex[0].x, TempTiles[tmpindex].Vertex[0].y, TempTiles[tmpindex].Vertex[0].z, TempTiles[tmpindex].Vertex[0].tnx, TempTiles[tmpindex].Vertex[0].tny);
									TextureIndex[sprno] = i;
									sprno++;
									SPRarray[sprno].Edit(TempTiles[tmpindex].Vertex[0].x, TempTiles[tmpindex].Vertex[0].y, TempTiles[tmpindex].Vertex[0].z, TempTiles[tmpindex].Vertex[0].tnx, TempTiles[tmpindex].Vertex[0].tny, AuxV[1].x, AuxV[1].y, AuxV[1].z, AuxV[1].tnx, AuxV[1].tny, TempTiles[tmpindex].Vertex[2].x, TempTiles[tmpindex].Vertex[2].y, TempTiles[tmpindex].Vertex[2].z, TempTiles[tmpindex].Vertex[2].tnx, TempTiles[tmpindex].Vertex[2].tny, TempTiles[tmpindex].Vertex[1].x, TempTiles[tmpindex].Vertex[1].y, TempTiles[tmpindex].Vertex[1].z, TempTiles[tmpindex].Vertex[1].tnx, TempTiles[tmpindex].Vertex[1].tny);
									TextureIndex[sprno] = i;
									sprno++;
									
								}
								else if(TempTiles[tmpindex].Vertex[1].z<0)
								{
									t=TempTiles[tmpindex].Vertex[0].z/(TempTiles[tmpindex].Vertex[0].z-TempTiles[tmpindex].Vertex[1].z);
									AuxV[0].x=TempTiles[tmpindex].Vertex[0].x+t*(TempTiles[tmpindex].Vertex[1].x-TempTiles[tmpindex].Vertex[0].x);
									AuxV[0].y=TempTiles[tmpindex].Vertex[0].y+t*(TempTiles[tmpindex].Vertex[1].y-TempTiles[tmpindex].Vertex[0].y);
									AuxV[0].z=0;
									AuxV[0].tnx=TempTiles[tmpindex].Vertex[3].tnx+t*TextureStep;
									AuxV[0].tny=TempTiles[tmpindex].Vertex[3].tny+TextureStep;
									t=TempTiles[tmpindex].Vertex[2].z/(TempTiles[tmpindex].Vertex[2].z-TempTiles[tmpindex].Vertex[1].z);
									AuxV[1].x=TempTiles[tmpindex].Vertex[2].x+t*(TempTiles[tmpindex].Vertex[1].x-TempTiles[tmpindex].Vertex[2].x);
									AuxV[1].y=TempTiles[tmpindex].Vertex[2].y+t*(TempTiles[tmpindex].Vertex[1].y-TempTiles[tmpindex].Vertex[2].y);
									AuxV[1].z=0;
									AuxV[1].tnx=TempTiles[tmpindex].Vertex[3].tnx+TextureStep;
									AuxV[1].tny=TempTiles[tmpindex].Vertex[3].tny+t*TextureStep;
									t=TempTiles[tmpindex].Vertex[3].z/(TempTiles[tmpindex].Vertex[3].z-TempTiles[tmpindex].Vertex[1].z);
									TempTiles[tmpindex].Vertex[1].x=TempTiles[tmpindex].Vertex[3].x+t*(TempTiles[tmpindex].Vertex[1].x-TempTiles[tmpindex].Vertex[3].x);
									TempTiles[tmpindex].Vertex[1].y=TempTiles[tmpindex].Vertex[3].y+t*(TempTiles[tmpindex].Vertex[1].y-TempTiles[tmpindex].Vertex[3].y);
									TempTiles[tmpindex].Vertex[1].z=0;
									TempTiles[tmpindex].Vertex[1].tnx=TempTiles[tmpindex].Vertex[3].tnx+t*TextureStep;
									TempTiles[tmpindex].Vertex[1].tny=TempTiles[tmpindex].Vertex[3].tny+t*TextureStep;
									
									SPRarray[sprno].Edit(TempTiles[tmpindex].Vertex[0].x, TempTiles[tmpindex].Vertex[0].y, TempTiles[tmpindex].Vertex[0].z, TempTiles[tmpindex].Vertex[0].tnx, TempTiles[tmpindex].Vertex[0].tny, AuxV[0].x, AuxV[0].y, AuxV[0].z, AuxV[0].tnx, AuxV[0].tny, TempTiles[tmpindex].Vertex[3].x, TempTiles[tmpindex].Vertex[3].y, TempTiles[tmpindex].Vertex[3].z, TempTiles[tmpindex].Vertex[3].tnx, TempTiles[tmpindex].Vertex[3].tny, TempTiles[tmpindex].Vertex[1].x, TempTiles[tmpindex].Vertex[1].y, TempTiles[tmpindex].Vertex[1].z, TempTiles[tmpindex].Vertex[1].tnx, TempTiles[tmpindex].Vertex[1].tny);
									TextureIndex[sprno] = i;
									sprno++;
									SPRarray[sprno].Edit(TempTiles[tmpindex].Vertex[1].x, TempTiles[tmpindex].Vertex[1].y, TempTiles[tmpindex].Vertex[1].z, TempTiles[tmpindex].Vertex[1].tnx, TempTiles[tmpindex].Vertex[1].tny, AuxV[1].x, AuxV[1].y, AuxV[1].z, AuxV[1].tnx, AuxV[1].tny, TempTiles[tmpindex].Vertex[3].x, TempTiles[tmpindex].Vertex[3].y, TempTiles[tmpindex].Vertex[3].z, TempTiles[tmpindex].Vertex[3].tnx, TempTiles[tmpindex].Vertex[3].tny, TempTiles[tmpindex].Vertex[2].x, TempTiles[tmpindex].Vertex[2].y, TempTiles[tmpindex].Vertex[2].z, TempTiles[tmpindex].Vertex[2].tnx, TempTiles[tmpindex].Vertex[2].tny);
									TextureIndex[sprno] = i;
									sprno++;
								}
								else if(TempTiles[tmpindex].Vertex[2].z<0)
								{
									t=TempTiles[tmpindex].Vertex[1].z/(TempTiles[tmpindex].Vertex[1].z-TempTiles[tmpindex].Vertex[2].z);
									AuxV[0].x=TempTiles[tmpindex].Vertex[1].x+t*(TempTiles[tmpindex].Vertex[2].x-TempTiles[tmpindex].Vertex[1].x);
									AuxV[0].y=TempTiles[tmpindex].Vertex[1].y+t*(TempTiles[tmpindex].Vertex[2].y-TempTiles[tmpindex].Vertex[1].y);
									AuxV[0].z=0;
									AuxV[0].tnx=TempTiles[tmpindex].Vertex[3].tnx+TextureStep;
									AuxV[0].tny=TempTiles[tmpindex].Vertex[3].tny+(1-t)*TextureStep;
									t=TempTiles[tmpindex].Vertex[3].z/(TempTiles[tmpindex].Vertex[3].z-TempTiles[tmpindex].Vertex[2].z);
									AuxV[1].x=TempTiles[tmpindex].Vertex[3].x+t*(TempTiles[tmpindex].Vertex[2].x-TempTiles[tmpindex].Vertex[3].x);
									AuxV[1].y=TempTiles[tmpindex].Vertex[3].y+t*(TempTiles[tmpindex].Vertex[2].y-TempTiles[tmpindex].Vertex[3].y);
									AuxV[1].z=0;
									AuxV[1].tnx=TempTiles[tmpindex].Vertex[3].tnx+t*TextureStep;
									AuxV[1].tny=TempTiles[tmpindex].Vertex[3].tny;
									t=TempTiles[tmpindex].Vertex[0].z/(TempTiles[tmpindex].Vertex[0].z-TempTiles[tmpindex].Vertex[2].z);
									TempTiles[tmpindex].Vertex[2].x=TempTiles[tmpindex].Vertex[0].x+t*(TempTiles[tmpindex].Vertex[2].x-TempTiles[tmpindex].Vertex[0].x);
									TempTiles[tmpindex].Vertex[2].y=TempTiles[tmpindex].Vertex[0].y+t*(TempTiles[tmpindex].Vertex[2].y-TempTiles[tmpindex].Vertex[0].y);
									TempTiles[tmpindex].Vertex[2].z=0;
									TempTiles[tmpindex].Vertex[2].tnx=TempTiles[tmpindex].Vertex[3].tnx+t*TextureStep;
									TempTiles[tmpindex].Vertex[2].tny=TempTiles[tmpindex].Vertex[3].tny+(1-t)*TextureStep;
									
									SPRarray[sprno].Edit(TempTiles[tmpindex].Vertex[1].x, TempTiles[tmpindex].Vertex[1].y, TempTiles[tmpindex].Vertex[1].z, TempTiles[tmpindex].Vertex[1].tnx, TempTiles[tmpindex].Vertex[1].tny, AuxV[0].x, AuxV[0].y, AuxV[0].z, AuxV[0].tnx, AuxV[0].tny, TempTiles[tmpindex].Vertex[0].x, TempTiles[tmpindex].Vertex[0].y, TempTiles[tmpindex].Vertex[0].z, TempTiles[tmpindex].Vertex[0].tnx, TempTiles[tmpindex].Vertex[0].tny, TempTiles[tmpindex].Vertex[2].x, TempTiles[tmpindex].Vertex[2].y, TempTiles[tmpindex].Vertex[2].z, TempTiles[tmpindex].Vertex[2].tnx, TempTiles[tmpindex].Vertex[2].tny);
									TextureIndex[sprno] = i;
									sprno++;
									SPRarray[sprno].Edit(TempTiles[tmpindex].Vertex[2].x, TempTiles[tmpindex].Vertex[2].y, TempTiles[tmpindex].Vertex[2].z, TempTiles[tmpindex].Vertex[2].tnx, TempTiles[tmpindex].Vertex[2].tny, AuxV[1].x, AuxV[1].y, AuxV[1].z, AuxV[1].tnx, AuxV[1].tny, TempTiles[tmpindex].Vertex[0].x, TempTiles[tmpindex].Vertex[0].y, TempTiles[tmpindex].Vertex[0].z, TempTiles[tmpindex].Vertex[0].tnx, TempTiles[tmpindex].Vertex[0].tny, TempTiles[tmpindex].Vertex[3].x, TempTiles[tmpindex].Vertex[3].y, TempTiles[tmpindex].Vertex[3].z, TempTiles[tmpindex].Vertex[3].tnx, TempTiles[tmpindex].Vertex[3].tny);
									TextureIndex[sprno] = i;
									sprno++;
								}
								else if(TempTiles[tmpindex].Vertex[3].z<0)
								{
									t=TempTiles[tmpindex].Vertex[2].z/(TempTiles[tmpindex].Vertex[2].z-TempTiles[tmpindex].Vertex[3].z);
									AuxV[0].x=TempTiles[tmpindex].Vertex[2].x+t*(TempTiles[tmpindex].Vertex[3].x-TempTiles[tmpindex].Vertex[2].x);
									AuxV[0].y=TempTiles[tmpindex].Vertex[2].y+t*(TempTiles[tmpindex].Vertex[3].y-TempTiles[tmpindex].Vertex[2].y);
									AuxV[0].z=0;
									AuxV[0].tnx=TempTiles[tmpindex].Vertex[3].tnx+(1-t)*TextureStep;
									AuxV[0].tny=TempTiles[tmpindex].Vertex[3].tny;
									t=TempTiles[tmpindex].Vertex[0].z/(TempTiles[tmpindex].Vertex[0].z-TempTiles[tmpindex].Vertex[3].z);
									AuxV[1].x=TempTiles[tmpindex].Vertex[0].x+t*(TempTiles[tmpindex].Vertex[3].x-TempTiles[tmpindex].Vertex[0].x);
									AuxV[1].y=TempTiles[tmpindex].Vertex[0].y+t*(TempTiles[tmpindex].Vertex[3].y-TempTiles[tmpindex].Vertex[0].y);
									AuxV[1].z=0;
									AuxV[1].tnx=TempTiles[tmpindex].Vertex[3].tnx;
									AuxV[1].tny=TempTiles[tmpindex].Vertex[3].tny+(1-t)*TextureStep;
									t=TempTiles[tmpindex].Vertex[1].z/(TempTiles[tmpindex].Vertex[1].z-TempTiles[tmpindex].Vertex[3].z);
									TempTiles[tmpindex].Vertex[3].x=TempTiles[tmpindex].Vertex[1].x+t*(TempTiles[tmpindex].Vertex[3].x-TempTiles[tmpindex].Vertex[1].x);
									TempTiles[tmpindex].Vertex[3].y=TempTiles[tmpindex].Vertex[1].y+t*(TempTiles[tmpindex].Vertex[3].y-TempTiles[tmpindex].Vertex[1].y);
									TempTiles[tmpindex].Vertex[3].z=0;
									TempTiles[tmpindex].Vertex[3].tnx=TempTiles[tmpindex].Vertex[3].tnx+(1-t)*TextureStep;
									TempTiles[tmpindex].Vertex[3].tny=TempTiles[tmpindex].Vertex[3].tny+(1-t)*TextureStep;
									
									SPRarray[sprno].Edit(TempTiles[tmpindex].Vertex[2].x, TempTiles[tmpindex].Vertex[2].y, TempTiles[tmpindex].Vertex[2].z, TempTiles[tmpindex].Vertex[2].tnx, TempTiles[tmpindex].Vertex[2].tny, AuxV[0].x, AuxV[0].y, AuxV[0].z, AuxV[0].tnx, AuxV[0].tny, TempTiles[tmpindex].Vertex[1].x, TempTiles[tmpindex].Vertex[1].y, TempTiles[tmpindex].Vertex[1].z, TempTiles[tmpindex].Vertex[1].tnx, TempTiles[tmpindex].Vertex[1].tny, TempTiles[tmpindex].Vertex[3].x, TempTiles[tmpindex].Vertex[3].y, TempTiles[tmpindex].Vertex[3].z, TempTiles[tmpindex].Vertex[3].tnx, TempTiles[tmpindex].Vertex[3].tny);
									TextureIndex[sprno] = i;
									sprno++;
									SPRarray[sprno].Edit(TempTiles[tmpindex].Vertex[3].x, TempTiles[tmpindex].Vertex[3].y, TempTiles[tmpindex].Vertex[3].z, TempTiles[tmpindex].Vertex[3].tnx, TempTiles[tmpindex].Vertex[3].tny, AuxV[1].x, AuxV[1].y, AuxV[1].z, AuxV[1].tnx, AuxV[1].tny, TempTiles[tmpindex].Vertex[1].x, TempTiles[tmpindex].Vertex[1].y, TempTiles[tmpindex].Vertex[1].z, TempTiles[tmpindex].Vertex[1].tnx, TempTiles[tmpindex].Vertex[1].tny, TempTiles[tmpindex].Vertex[0].x, TempTiles[tmpindex].Vertex[0].y, TempTiles[tmpindex].Vertex[0].z, TempTiles[tmpindex].Vertex[0].tnx, TempTiles[tmpindex].Vertex[0].tny);
									TextureIndex[sprno] = i;
									sprno++;
								}
							DSP0.HandleAudio();
							}
							// 2 valid vertices
							else if(validvertices==2)
							{
								if(TempTiles[tmpindex].Vertex[0].z>=0)
								{
									if(TempTiles[tmpindex].Vertex[1].z>=0)
									{
										t=TempTiles[tmpindex].Vertex[1].z/(TempTiles[tmpindex].Vertex[1].z-TempTiles[tmpindex].Vertex[2].z);
										TempTiles[tmpindex].Vertex[2].x=TempTiles[tmpindex].Vertex[1].x+t*(TempTiles[tmpindex].Vertex[2].x-TempTiles[tmpindex].Vertex[1].x);
										TempTiles[tmpindex].Vertex[2].y=TempTiles[tmpindex].Vertex[1].y+t*(TempTiles[tmpindex].Vertex[2].y-TempTiles[tmpindex].Vertex[1].y);
										TempTiles[tmpindex].Vertex[2].z=0;
										TempTiles[tmpindex].Vertex[2].tny=TempTiles[tmpindex].Vertex[3].tny+(1-t)*TextureStep;
										t=TempTiles[tmpindex].Vertex[0].z/(TempTiles[tmpindex].Vertex[0].z-TempTiles[tmpindex].Vertex[3].z);
										TempTiles[tmpindex].Vertex[3].x=TempTiles[tmpindex].Vertex[0].x+t*(TempTiles[tmpindex].Vertex[3].x-TempTiles[tmpindex].Vertex[0].x);
										TempTiles[tmpindex].Vertex[3].y=TempTiles[tmpindex].Vertex[0].y+t*(TempTiles[tmpindex].Vertex[3].y-TempTiles[tmpindex].Vertex[0].y);
										TempTiles[tmpindex].Vertex[3].z=0;
										TempTiles[tmpindex].Vertex[3].tny=TempTiles[tmpindex].Vertex[3].tny+(1-t)*TextureStep;
										
										SPRarray[sprno].Edit(TempTiles[tmpindex].Vertex[3].x, TempTiles[tmpindex].Vertex[3].y, TempTiles[tmpindex].Vertex[3].z, TempTiles[tmpindex].Vertex[3].tnx, TempTiles[tmpindex].Vertex[3].tny, TempTiles[tmpindex].Vertex[0].x, TempTiles[tmpindex].Vertex[0].y, TempTiles[tmpindex].Vertex[0].z, TempTiles[tmpindex].Vertex[0].tnx, TempTiles[tmpindex].Vertex[0].tny, TempTiles[tmpindex].Vertex[2].x, TempTiles[tmpindex].Vertex[2].y, TempTiles[tmpindex].Vertex[2].z, TempTiles[tmpindex].Vertex[2].tnx, TempTiles[tmpindex].Vertex[2].tny, TempTiles[tmpindex].Vertex[1].x, TempTiles[tmpindex].Vertex[1].y, TempTiles[tmpindex].Vertex[1].z, TempTiles[tmpindex].Vertex[1].tnx, TempTiles[tmpindex].Vertex[1].tny);
										TextureIndex[sprno] = i;
										sprno++;
									}
									else
									{
										t=TempTiles[tmpindex].Vertex[0].z/(TempTiles[tmpindex].Vertex[0].z-TempTiles[tmpindex].Vertex[1].z);
										TempTiles[tmpindex].Vertex[1].x=TempTiles[tmpindex].Vertex[0].x+t*(TempTiles[tmpindex].Vertex[1].x-TempTiles[tmpindex].Vertex[0].x);
										TempTiles[tmpindex].Vertex[1].y=TempTiles[tmpindex].Vertex[0].y+t*(TempTiles[tmpindex].Vertex[1].y-TempTiles[tmpindex].Vertex[0].y);
										TempTiles[tmpindex].Vertex[1].z=0;
										TempTiles[tmpindex].Vertex[1].tnx=TempTiles[tmpindex].Vertex[3].tnx+t*TextureStep;
										t=TempTiles[tmpindex].Vertex[3].z/(TempTiles[tmpindex].Vertex[3].z-TempTiles[tmpindex].Vertex[2].z);
										TempTiles[tmpindex].Vertex[2].x=TempTiles[tmpindex].Vertex[3].x+t*(TempTiles[tmpindex].Vertex[2].x-TempTiles[tmpindex].Vertex[3].x);
										TempTiles[tmpindex].Vertex[2].y=TempTiles[tmpindex].Vertex[3].y+t*(TempTiles[tmpindex].Vertex[2].y-TempTiles[tmpindex].Vertex[3].y);
										TempTiles[tmpindex].Vertex[2].z=0;
										TempTiles[tmpindex].Vertex[2].tnx=TempTiles[tmpindex].Vertex[3].tnx+t*TextureStep;
										
										SPRarray[sprno].Edit(TempTiles[tmpindex].Vertex[3].x, TempTiles[tmpindex].Vertex[3].y, TempTiles[tmpindex].Vertex[3].z, TempTiles[tmpindex].Vertex[3].tnx, TempTiles[tmpindex].Vertex[3].tny, TempTiles[tmpindex].Vertex[0].x, TempTiles[tmpindex].Vertex[0].y, TempTiles[tmpindex].Vertex[0].z, TempTiles[tmpindex].Vertex[0].tnx, TempTiles[tmpindex].Vertex[0].tny, TempTiles[tmpindex].Vertex[2].x, TempTiles[tmpindex].Vertex[2].y, TempTiles[tmpindex].Vertex[2].z, TempTiles[tmpindex].Vertex[2].tnx, TempTiles[tmpindex].Vertex[2].tny, TempTiles[tmpindex].Vertex[1].x, TempTiles[tmpindex].Vertex[1].y, TempTiles[tmpindex].Vertex[1].z, TempTiles[tmpindex].Vertex[1].tnx, TempTiles[tmpindex].Vertex[1].tny);
										TextureIndex[sprno] = i;
										sprno++;
									}
								}
								else
								{
									if(TempTiles[tmpindex].Vertex[1].z>=0)
									{
										t=TempTiles[tmpindex].Vertex[1].z/(TempTiles[tmpindex].Vertex[1].z-TempTiles[tmpindex].Vertex[0].z);
										TempTiles[tmpindex].Vertex[0].x=TempTiles[tmpindex].Vertex[1].x+t*(TempTiles[tmpindex].Vertex[0].x-TempTiles[tmpindex].Vertex[1].x);
										TempTiles[tmpindex].Vertex[0].y=TempTiles[tmpindex].Vertex[1].y+t*(TempTiles[tmpindex].Vertex[0].y-TempTiles[tmpindex].Vertex[1].y);
										TempTiles[tmpindex].Vertex[0].z=0;
										TempTiles[tmpindex].Vertex[0].tnx=TempTiles[tmpindex].Vertex[3].tnx+(1-t)*TextureStep;
										t=TempTiles[tmpindex].Vertex[2].z/(TempTiles[tmpindex].Vertex[2].z-TempTiles[tmpindex].Vertex[3].z);
										TempTiles[tmpindex].Vertex[3].x=TempTiles[tmpindex].Vertex[2].x+t*(TempTiles[tmpindex].Vertex[3].x-TempTiles[tmpindex].Vertex[2].x);
										TempTiles[tmpindex].Vertex[3].y=TempTiles[tmpindex].Vertex[2].y+t*(TempTiles[tmpindex].Vertex[3].y-TempTiles[tmpindex].Vertex[2].y);
										TempTiles[tmpindex].Vertex[3].z=0;
										TempTiles[tmpindex].Vertex[3].tnx=TempTiles[tmpindex].Vertex[3].tnx+(1-t)*TextureStep;
										
										SPRarray[sprno].Edit(TempTiles[tmpindex].Vertex[3].x, TempTiles[tmpindex].Vertex[3].y, TempTiles[tmpindex].Vertex[3].z, TempTiles[tmpindex].Vertex[3].tnx, TempTiles[tmpindex].Vertex[3].tny, TempTiles[tmpindex].Vertex[0].x, TempTiles[tmpindex].Vertex[0].y, TempTiles[tmpindex].Vertex[0].z, TempTiles[tmpindex].Vertex[0].tnx, TempTiles[tmpindex].Vertex[0].tny, TempTiles[tmpindex].Vertex[2].x, TempTiles[tmpindex].Vertex[2].y, TempTiles[tmpindex].Vertex[2].z, TempTiles[tmpindex].Vertex[2].tnx, TempTiles[tmpindex].Vertex[2].tny, TempTiles[tmpindex].Vertex[1].x, TempTiles[tmpindex].Vertex[1].y, TempTiles[tmpindex].Vertex[1].z, TempTiles[tmpindex].Vertex[1].tnx, TempTiles[tmpindex].Vertex[1].tny);
										TextureIndex[sprno] = i;
										sprno++;
									}
									else
									{
										t=TempTiles[tmpindex].Vertex[3].z/(TempTiles[tmpindex].Vertex[3].z-TempTiles[tmpindex].Vertex[0].z);
										TempTiles[tmpindex].Vertex[0].x=TempTiles[tmpindex].Vertex[3].x+t*(TempTiles[tmpindex].Vertex[0].x-TempTiles[tmpindex].Vertex[3].x);
										TempTiles[tmpindex].Vertex[0].y=TempTiles[tmpindex].Vertex[3].y+t*(TempTiles[tmpindex].Vertex[0].y-TempTiles[tmpindex].Vertex[3].y);
										TempTiles[tmpindex].Vertex[0].z=0;
										TempTiles[tmpindex].Vertex[0].tny=TempTiles[tmpindex].Vertex[3].tny+t*TextureStep;
										t=TempTiles[tmpindex].Vertex[2].z/(TempTiles[tmpindex].Vertex[2].z-TempTiles[tmpindex].Vertex[1].z);
										TempTiles[tmpindex].Vertex[1].x=TempTiles[tmpindex].Vertex[2].x+t*(TempTiles[tmpindex].Vertex[1].x-TempTiles[tmpindex].Vertex[2].x);
										TempTiles[tmpindex].Vertex[1].y=TempTiles[tmpindex].Vertex[2].y+t*(TempTiles[tmpindex].Vertex[1].y-TempTiles[tmpindex].Vertex[2].y);
										TempTiles[tmpindex].Vertex[1].z=0;
										TempTiles[tmpindex].Vertex[1].tny=TempTiles[tmpindex].Vertex[3].tny+t*TextureStep;
										
										SPRarray[sprno].Edit(TempTiles[tmpindex].Vertex[3].x, TempTiles[tmpindex].Vertex[3].y, TempTiles[tmpindex].Vertex[3].z, TempTiles[tmpindex].Vertex[3].tnx, TempTiles[tmpindex].Vertex[3].tny, TempTiles[tmpindex].Vertex[0].x, TempTiles[tmpindex].Vertex[0].y, TempTiles[tmpindex].Vertex[0].z, TempTiles[tmpindex].Vertex[0].tnx, TempTiles[tmpindex].Vertex[0].tny, TempTiles[tmpindex].Vertex[2].x, TempTiles[tmpindex].Vertex[2].y, TempTiles[tmpindex].Vertex[2].z, TempTiles[tmpindex].Vertex[2].tnx, TempTiles[tmpindex].Vertex[2].tny, TempTiles[tmpindex].Vertex[1].x, TempTiles[tmpindex].Vertex[1].y, TempTiles[tmpindex].Vertex[1].z, TempTiles[tmpindex].Vertex[1].tnx, TempTiles[tmpindex].Vertex[1].tny);
										TextureIndex[sprno] = i;
										sprno++;
									}
								}
							DSP0.HandleAudio();
							}
							//1 valid vertex
							else if(validvertices==1)
							{
								if(TempTiles[tmpindex].Vertex[0].z>=0)
								{
									t=TempTiles[tmpindex].Vertex[0].z/(TempTiles[tmpindex].Vertex[0].z-TempTiles[tmpindex].Vertex[1].z);
									TempTiles[tmpindex].Vertex[1].x=TempTiles[tmpindex].Vertex[0].x+t*(TempTiles[tmpindex].Vertex[1].x-TempTiles[tmpindex].Vertex[0].x);
									TempTiles[tmpindex].Vertex[1].y=TempTiles[tmpindex].Vertex[0].y+t*(TempTiles[tmpindex].Vertex[1].y-TempTiles[tmpindex].Vertex[0].y);
									TempTiles[tmpindex].Vertex[1].z=0;
									TempTiles[tmpindex].Vertex[1].tnx=TempTiles[tmpindex].Vertex[3].tnx+t*TextureStep;
									t=TempTiles[tmpindex].Vertex[0].z/(TempTiles[tmpindex].Vertex[0].z-TempTiles[tmpindex].Vertex[2].z);
									TempTiles[tmpindex].Vertex[2].x=TempTiles[tmpindex].Vertex[0].x+t*(TempTiles[tmpindex].Vertex[2].x-TempTiles[tmpindex].Vertex[0].x);
									TempTiles[tmpindex].Vertex[2].y=TempTiles[tmpindex].Vertex[0].y+t*(TempTiles[tmpindex].Vertex[2].y-TempTiles[tmpindex].Vertex[0].y);
									TempTiles[tmpindex].Vertex[2].z=0;
									TempTiles[tmpindex].Vertex[2].tnx=TempTiles[tmpindex].Vertex[3].tnx+t*TextureStep;
									TempTiles[tmpindex].Vertex[2].tny=TempTiles[tmpindex].Vertex[3].tny+(1-t)*TextureStep;
									t=TempTiles[tmpindex].Vertex[0].z/(TempTiles[tmpindex].Vertex[0].z-TempTiles[tmpindex].Vertex[3].z);
									TempTiles[tmpindex].Vertex[3].x=TempTiles[tmpindex].Vertex[0].x+t*(TempTiles[tmpindex].Vertex[3].x-TempTiles[tmpindex].Vertex[0].x);
									TempTiles[tmpindex].Vertex[3].y=TempTiles[tmpindex].Vertex[0].y+t*(TempTiles[tmpindex].Vertex[3].y-TempTiles[tmpindex].Vertex[0].y);
									TempTiles[tmpindex].Vertex[3].z=0;
									TempTiles[tmpindex].Vertex[3].tny=TempTiles[tmpindex].Vertex[3].tny+(1-t)*TextureStep;
									
									SPRarray[sprno].Edit(TempTiles[tmpindex].Vertex[3].x, TempTiles[tmpindex].Vertex[3].y, TempTiles[tmpindex].Vertex[3].z, TempTiles[tmpindex].Vertex[3].tnx, TempTiles[tmpindex].Vertex[3].tny, TempTiles[tmpindex].Vertex[0].x, TempTiles[tmpindex].Vertex[0].y, TempTiles[tmpindex].Vertex[0].z, TempTiles[tmpindex].Vertex[0].tnx, TempTiles[tmpindex].Vertex[0].tny, TempTiles[tmpindex].Vertex[2].x, TempTiles[tmpindex].Vertex[2].y, TempTiles[tmpindex].Vertex[2].z, TempTiles[tmpindex].Vertex[2].tnx, TempTiles[tmpindex].Vertex[2].tny, TempTiles[tmpindex].Vertex[1].x, TempTiles[tmpindex].Vertex[1].y, TempTiles[tmpindex].Vertex[1].z, TempTiles[tmpindex].Vertex[1].tnx, TempTiles[tmpindex].Vertex[1].tny);
									TextureIndex[sprno] = i;
									sprno++;
								}
								else if(TempTiles[tmpindex].Vertex[1].z>=0)
								{
									t=TempTiles[tmpindex].Vertex[1].z/(TempTiles[tmpindex].Vertex[1].z-TempTiles[tmpindex].Vertex[0].z);
									TempTiles[tmpindex].Vertex[0].x=TempTiles[tmpindex].Vertex[1].x+t*(TempTiles[tmpindex].Vertex[0].x-TempTiles[tmpindex].Vertex[1].x);
									TempTiles[tmpindex].Vertex[0].y=TempTiles[tmpindex].Vertex[1].y+t*(TempTiles[tmpindex].Vertex[0].y-TempTiles[tmpindex].Vertex[1].y);
									TempTiles[tmpindex].Vertex[0].z=0;
									TempTiles[tmpindex].Vertex[0].tnx=TempTiles[tmpindex].Vertex[3].tnx+(1-t)*TextureStep;
									t=TempTiles[tmpindex].Vertex[1].z/(TempTiles[tmpindex].Vertex[1].z-TempTiles[tmpindex].Vertex[2].z);
									TempTiles[tmpindex].Vertex[2].x=TempTiles[tmpindex].Vertex[1].x+t*(TempTiles[tmpindex].Vertex[2].x-TempTiles[tmpindex].Vertex[1].x);
									TempTiles[tmpindex].Vertex[2].y=TempTiles[tmpindex].Vertex[1].y+t*(TempTiles[tmpindex].Vertex[2].y-TempTiles[tmpindex].Vertex[1].y);
									TempTiles[tmpindex].Vertex[2].z=0;
									TempTiles[tmpindex].Vertex[2].tny=TempTiles[tmpindex].Vertex[3].tny+(1-t)*TextureStep;
									t=TempTiles[tmpindex].Vertex[1].z/(TempTiles[tmpindex].Vertex[1].z-TempTiles[tmpindex].Vertex[3].z);
									TempTiles[tmpindex].Vertex[3].x=TempTiles[tmpindex].Vertex[1].x+t*(TempTiles[tmpindex].Vertex[3].x-TempTiles[tmpindex].Vertex[1].x);
									TempTiles[tmpindex].Vertex[3].y=TempTiles[tmpindex].Vertex[1].y+t*(TempTiles[tmpindex].Vertex[3].y-TempTiles[tmpindex].Vertex[1].y);
									TempTiles[tmpindex].Vertex[3].z=0;
									TempTiles[tmpindex].Vertex[3].tnx=TempTiles[tmpindex].Vertex[3].tnx+(1-t)*TextureStep;
									TempTiles[tmpindex].Vertex[3].tny=TempTiles[tmpindex].Vertex[3].tny+(1-t)*TextureStep;
									
									SPRarray[sprno].Edit(TempTiles[tmpindex].Vertex[3].x, TempTiles[tmpindex].Vertex[3].y, TempTiles[tmpindex].Vertex[3].z, TempTiles[tmpindex].Vertex[3].tnx, TempTiles[tmpindex].Vertex[3].tny, TempTiles[tmpindex].Vertex[0].x, TempTiles[tmpindex].Vertex[0].y, TempTiles[tmpindex].Vertex[0].z, TempTiles[tmpindex].Vertex[0].tnx, TempTiles[tmpindex].Vertex[0].tny, TempTiles[tmpindex].Vertex[2].x, TempTiles[tmpindex].Vertex[2].y, TempTiles[tmpindex].Vertex[2].z, TempTiles[tmpindex].Vertex[2].tnx, TempTiles[tmpindex].Vertex[2].tny, TempTiles[tmpindex].Vertex[1].x, TempTiles[tmpindex].Vertex[1].y, TempTiles[tmpindex].Vertex[1].z, TempTiles[tmpindex].Vertex[1].tnx, TempTiles[tmpindex].Vertex[1].tny);
									TextureIndex[sprno] = i;
									sprno++;
								}
								else if(TempTiles[tmpindex].Vertex[2].z>=0)
								{
									t=TempTiles[tmpindex].Vertex[2].z/(TempTiles[tmpindex].Vertex[2].z-TempTiles[tmpindex].Vertex[0].z);
									TempTiles[tmpindex].Vertex[0].x=TempTiles[tmpindex].Vertex[2].x+t*(TempTiles[tmpindex].Vertex[0].x-TempTiles[tmpindex].Vertex[2].x);
									TempTiles[tmpindex].Vertex[0].y=TempTiles[tmpindex].Vertex[2].y+t*(TempTiles[tmpindex].Vertex[0].y-TempTiles[tmpindex].Vertex[2].y);
									TempTiles[tmpindex].Vertex[0].z=0;
									TempTiles[tmpindex].Vertex[0].tnx=TempTiles[tmpindex].Vertex[3].tnx+(1-t)*TextureStep;
									TempTiles[tmpindex].Vertex[0].tny=TempTiles[tmpindex].Vertex[3].tny+t*TextureStep;
									t=TempTiles[tmpindex].Vertex[2].z/(TempTiles[tmpindex].Vertex[2].z-TempTiles[tmpindex].Vertex[1].z);
									TempTiles[tmpindex].Vertex[1].x=TempTiles[tmpindex].Vertex[2].x+t*(TempTiles[tmpindex].Vertex[1].x-TempTiles[tmpindex].Vertex[2].x);
									TempTiles[tmpindex].Vertex[1].y=TempTiles[tmpindex].Vertex[2].y+t*(TempTiles[tmpindex].Vertex[1].y-TempTiles[tmpindex].Vertex[2].y);
									TempTiles[tmpindex].Vertex[1].z=0;
									TempTiles[tmpindex].Vertex[1].tny=TempTiles[tmpindex].Vertex[3].tny+t*TextureStep;
									t=TempTiles[tmpindex].Vertex[2].z/(TempTiles[tmpindex].Vertex[2].z-TempTiles[tmpindex].Vertex[3].z);
									TempTiles[tmpindex].Vertex[3].x=TempTiles[tmpindex].Vertex[2].x+t*(TempTiles[tmpindex].Vertex[3].x-TempTiles[tmpindex].Vertex[2].x);
									TempTiles[tmpindex].Vertex[3].y=TempTiles[tmpindex].Vertex[2].y+t*(TempTiles[tmpindex].Vertex[3].y-TempTiles[tmpindex].Vertex[2].y);
									TempTiles[tmpindex].Vertex[3].z=0;
									TempTiles[tmpindex].Vertex[3].tnx=TempTiles[tmpindex].Vertex[3].tnx+(1-t)*TextureStep;
									
									SPRarray[sprno].Edit(TempTiles[tmpindex].Vertex[3].x, TempTiles[tmpindex].Vertex[3].y, TempTiles[tmpindex].Vertex[3].z, TempTiles[tmpindex].Vertex[3].tnx, TempTiles[tmpindex].Vertex[3].tny, TempTiles[tmpindex].Vertex[0].x, TempTiles[tmpindex].Vertex[0].y, TempTiles[tmpindex].Vertex[0].z, TempTiles[tmpindex].Vertex[0].tnx, TempTiles[tmpindex].Vertex[0].tny, TempTiles[tmpindex].Vertex[2].x, TempTiles[tmpindex].Vertex[2].y, TempTiles[tmpindex].Vertex[2].z, TempTiles[tmpindex].Vertex[2].tnx, TempTiles[tmpindex].Vertex[2].tny, TempTiles[tmpindex].Vertex[1].x, TempTiles[tmpindex].Vertex[1].y, TempTiles[tmpindex].Vertex[1].z, TempTiles[tmpindex].Vertex[1].tnx, TempTiles[tmpindex].Vertex[1].tny);
									TextureIndex[sprno] = i;
									sprno++;
								}
								else if(TempTiles[tmpindex].Vertex[3].z>=0)
								{
									t=TempTiles[tmpindex].Vertex[3].z/(TempTiles[tmpindex].Vertex[3].z-TempTiles[tmpindex].Vertex[0].z);
									TempTiles[tmpindex].Vertex[0].x=TempTiles[tmpindex].Vertex[3].x+t*(TempTiles[tmpindex].Vertex[0].x-TempTiles[tmpindex].Vertex[3].x);
									TempTiles[tmpindex].Vertex[0].y=TempTiles[tmpindex].Vertex[3].y+t*(TempTiles[tmpindex].Vertex[0].y-TempTiles[tmpindex].Vertex[3].y);
									TempTiles[tmpindex].Vertex[0].z=0;
									TempTiles[tmpindex].Vertex[0].tny=TempTiles[tmpindex].Vertex[3].tny+t*TextureStep;
									t=TempTiles[tmpindex].Vertex[3].z/(TempTiles[tmpindex].Vertex[3].z-TempTiles[tmpindex].Vertex[2].z);
									TempTiles[tmpindex].Vertex[2].x=TempTiles[tmpindex].Vertex[3].x+t*(TempTiles[tmpindex].Vertex[2].x-TempTiles[tmpindex].Vertex[3].x);
									TempTiles[tmpindex].Vertex[2].y=TempTiles[tmpindex].Vertex[3].y+t*(TempTiles[tmpindex].Vertex[2].y-TempTiles[tmpindex].Vertex[3].y);
									TempTiles[tmpindex].Vertex[2].z=0;
									TempTiles[tmpindex].Vertex[2].tnx=TempTiles[tmpindex].Vertex[3].tnx+t*TextureStep;
									t=TempTiles[tmpindex].Vertex[3].z/(TempTiles[tmpindex].Vertex[3].z-TempTiles[tmpindex].Vertex[1].z);
									TempTiles[tmpindex].Vertex[1].x=TempTiles[tmpindex].Vertex[3].x+t*(TempTiles[tmpindex].Vertex[1].x-TempTiles[tmpindex].Vertex[3].x);
									TempTiles[tmpindex].Vertex[1].y=TempTiles[tmpindex].Vertex[3].y+t*(TempTiles[tmpindex].Vertex[1].y-TempTiles[tmpindex].Vertex[3].y);
									TempTiles[tmpindex].Vertex[1].z=0;
									TempTiles[tmpindex].Vertex[1].tnx=TempTiles[tmpindex].Vertex[3].tnx+t*TextureStep;
									TempTiles[tmpindex].Vertex[1].tny=TempTiles[tmpindex].Vertex[3].tny+t*TextureStep;
									
									SPRarray[sprno].Edit(TempTiles[tmpindex].Vertex[3].x, TempTiles[tmpindex].Vertex[3].y, TempTiles[tmpindex].Vertex[3].z, TempTiles[tmpindex].Vertex[3].tnx, TempTiles[tmpindex].Vertex[3].tny, TempTiles[tmpindex].Vertex[0].x, TempTiles[tmpindex].Vertex[0].y, TempTiles[tmpindex].Vertex[0].z, TempTiles[tmpindex].Vertex[0].tnx, TempTiles[tmpindex].Vertex[0].tny, TempTiles[tmpindex].Vertex[2].x, TempTiles[tmpindex].Vertex[2].y, TempTiles[tmpindex].Vertex[2].z, TempTiles[tmpindex].Vertex[2].tnx, TempTiles[tmpindex].Vertex[2].tny, TempTiles[tmpindex].Vertex[1].x, TempTiles[tmpindex].Vertex[1].y, TempTiles[tmpindex].Vertex[1].z, TempTiles[tmpindex].Vertex[1].tnx, TempTiles[tmpindex].Vertex[1].tny);
									TextureIndex[sprno] = i;
									sprno++;
								}
							DSP0.HandleAudio();
							}
							// no valid vertices: discard
							else
							{}
						}
						else
						{}
					}
					else
					{}
				}
				else
				{}
			}
			else
			{}
		}
	}
	DSP0.HandleAudio();
	//Apply perspective transformation to sprites coordinates and work out distance of each vertex to the eye
	for(i=0;i<sprno;i++)
	{
		for(j=0;j<4;j++)
		{
			tmpvx[j]=SPRarray[i].GetVertexData(j,'x');
			tmpvy[j]=SPRarray[i].GetVertexData(j,'y');
			tmpvz[j]=SPRarray[i].GetVertexData(j,'z');
			tmpvdistance[j]=(1/(tmpvz[j]+EyeDistance));
			tmpvx[j]=(tmpvx[j]*EyeDistance)/(tmpvz[j]+EyeDistance);
			tmpvy[j]=0-((tmpvy[j]*EyeDistance)/(tmpvz[j]+EyeDistance));
			tmpvz[j]=0;
		}
		SPRarray[i].Edit(tmpvx[0],tmpvy[0],tmpvdistance[0],SPRarray[i].GetVertexData(0,'h'),SPRarray[i].GetVertexData(0,'k'),tmpvx[1],tmpvy[1],tmpvdistance[1],SPRarray[i].GetVertexData(1,'h'),SPRarray[i].GetVertexData(1,'k'),tmpvx[2],tmpvy[2],tmpvdistance[2],SPRarray[i].GetVertexData(2,'h'),SPRarray[i].GetVertexData(2,'k'),tmpvx[3],tmpvy[3],tmpvdistance[3],SPRarray[i].GetVertexData(3,'h'),SPRarray[i].GetVertexData(3,'k'));
	}
	//Render Sprites
	for(i=0;i<sprno;i++)
	{
		if(i>1)
		{
			if(TextureIndex[i]!=TextureIndex[(i-1)])
			{
				TextureArray[TextureIndex[i]].Upload(TEXBUF480);
				TextureArray[TextureIndex[i]].Select();
			}
		}
		else
		{
			TextureArray[TextureIndex[i]].Upload(TEXBUF480);
			TextureArray[TextureIndex[i]].Select();
		}
		SPRarray[i].Render();
	}
	DSP0.HandleAudio();
}

void TerRenderer::debList()
{
	int i,j;
	for(i=0;i<NumTiles;i++)
	{
		for(j=0;j<4;j++)
		{
			printf("Tile %d vertex %d xpos = %f\n", i,j,TileArray[i].Vertex[j].x);
			printf("Tile %d vertex %d ypos = %f\n", i,j,TileArray[i].Vertex[j].y);
			printf("Tile %d vertex %d zpos = %f\n", i,j,TileArray[i].Vertex[j].z);
			
		}
	}
}