#ifndef _TERRENDERER_H_
#define _TERRENDERER_H_

#include <sps2lib.h>
#include <sps2tags.h>
#include <sps2util.h>
#include <sps2regstructs.h>
#include <assert.h>
#include <memory.h>
#include <signal.h>
#include "PS2Defines.h"
#include "texture.h"
#include "ps2maths.h"
#include "ps2matrix4x4.h"
#include "3dprimitives.h"
#include "customdefines.h"
#include "sps2wrap.h"

class TerRenderer
{

public:
	
	TerRenderer(const int x, const int y, const int sub, const float side, const float sd, const float eh, const float ed);
	
	~TerRenderer();
	
	void GetPlayerPos(const Position x);
	
	bool LoadTextures(char * Fileroot);
	
	void Render();
	
	void debList();
	
protected:

	Position PlayerPos;
	float SideLenght;
	float ScreenDistance;
	float EyeHeight;
	float EyeDistance;
	int NumTiles;
	int SubDivs;
	float TextureStep;
	Tile *TileArray;
	Tile *TempTiles;
	CTexture *TextureArray;
	PS23DSpriteT *SPRarray;
	int *TextureIndex;
};

#endif