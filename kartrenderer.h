#ifndef _KARTRENDERER_H_
#define _KARTRENDERER_H_

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
#include "primitives.h"
#include "customdefines.h"
#include "sps2wrap.h"

class KartRenderer
{

public:

	KartRenderer(Kart *Kptr, const int nofKarts, const float Ksize, const float sd, const float eh, const float ed);
	
	~KartRenderer();
	
	void GetPlayerPos(const Position x);
	
	bool LoadTextures(const char *tx1, const char *tx2);
	
	void Render();

protected:

	Position PlayerPos;
	Kart *Karts;
	Kart *Tempkarts;
	Position *hrefpts;
	int nKarts;
	float KartSize;
	CTexture KTP[2];
	float ScreenDistance;
	float EyeHeight;
	float EyeDistance;
	PS2SpriteT Sprite;
};

#endif