#include <sps2lib.h>
#include <sps2tags.h>
#include <sps2util.h>
#include <sps2regstructs.h>
#include <assert.h>
#include <memory.h>
#include <signal.h>
#include "PS2Defines.h"
#include "ps2maths.h"
#include "ps2matrix4x4.h"
#include "pad.h"
#include "sps2wrap.h"
#include "dma.h"
#include "texture.h"
#include "font.h"
#include "primitives.h"
#include "3dprimitives.h"
#include "terrenderer.h"
#include "kartrenderer.h"
#include "customdefines.h"
#include "cdtrack.h"
#include "AIcoor.h"
#include "audio.h"

bool g_bLoop = true;

void sig_handle(int sig)
{
	g_bLoop = false;
}


//Initialize Audio
AudioDevice DSP0(0);	
SoundSample MmBGM("menu", &DSP0);
SoundSample CdBGM("cdown", &DSP0);
SoundSample RaceBGM("race", &DSP0);
SoundSample StatsBGM("stats", &DSP0);

int main()
{
	//Game Environment Initialization
	int i,j,k;
	int slzone;
	int gscounter=0;
	int ifselector=0;
	//3D parameters
	float ScreenDistance = 50;
	float EyeHeight = 250;
	float EyeDistance = 300;
	float Kartsize=60;
	//Game Variables
	int maxlaps=3;
	int chosenchar=0;
	Position Player;
	Kart Karts[8];
	KartStats KS[8];
	float wpdst;
	float mallowance;
	int arrindex;
	int positions[8];
	int finalrankings[8];
	int frindex=0;
	int playerposition;
	int gamestate=0;
	int poshelper;
	CTexture Background;
	CTexture Auxiliary;
	CTexture Skytex;

	//Initialize background sprite
	PS2SpriteT BGS(0,0,20,640,512,256,256,0,0);
	PS2SpriteT Sky(0,0,20,640,512,256,256,0,0);
	
	//Initialize Interface Sprites
	PS2SpriteT cursor(0,0,21000,24,24,32,32,224,0);
	//MainMenu
	PS2SpriteT mmstart(0,120,96,32,96,32,0,0);
	PS2SpriteT mmoptions(0,160,128,32,128,32,96,0);
	PS2SpriteT mmexit(0,200,64,32,64,32,192,96);
	//Options Menu
	PS2SpriteT omlaps(0,120,96,32,96,32,96,64);
	PS2SpriteT omcharacter(0,160,192,32,192,32,0,96);
	PS2SpriteT omback(0,200,96,32,96,32,160,128);
	PS2SpriteT omld1(128,120,128,32,32,32,32,0,32);
	PS2SpriteT omld2(152,120,130,32,32,32,32,0,32);
	PS2SpriteT omcm(140,160,32,32,32,32,0,224);
	//Race Countdown
	PS2SpriteT cdready(0,0,20000,384,128,96,32,0,192);
	PS2SpriteT cd3(0,0,20000,128,128,32,32,96,32);
	PS2SpriteT cd2(0,0,20000,128,128,32,32,64,32);
	PS2SpriteT cd1(0,0,20000,128,128,32,32,32,32);
	PS2SpriteT cdstart(0,0,20000,384,128,96,32,0,0);
	//Race
	PS2SpriteT rlap(140,-220,20000,64,32,64,32,192,64);
	PS2SpriteT rlapd1(204,-220,20000,32,32,32,32,0,32);
	PS2SpriteT rlapd2(228,-220,20000,32,32,32,32,0,32);
	PS2SpriteT rlapdiv(252,-220,20000,32,32,32,32,64,64);
	PS2SpriteT rmlapd1(276,-220,20000,32,32,32,32,0,32);
	PS2SpriteT rmlapd2(300,-220,20000,32,32,32,32,0,32);
	PS2SpriteT rposd(-280,-220,20000,64,64,32,32,0,64);
	PS2SpriteT rpossuffix(-250,-220,20000,32,16,32,16,128,208);
	//Pause Menu
	PS2SpriteT pmpause(0,0,20010,160,32,160,32,0,128);
	PS2SpriteT pmresume(0,120,20010,128,32,128,32,128,160);
	PS2SpriteT pmrestart(0,160,20010,128,32,128,32,0,160);
	PS2SpriteT pmexit(0,200,20010,64,32,64,32,192,96);
	//End of race rankings Menu
	PS2SpriteT rrndisplay(-100,0,20010,64,64,32,32,0,32);
	PS2SpriteT rrnsuffix(-80,0,20010,64,32,32,16,96,192);
	PS2SpriteT rrcm(100,0,20010,64,64,32,32,0,224);
	PS2SpriteT rrexit(0,200,20010,64,32,64,32,192,96);
	
	
	SPS2Manager.Initialise(1024);	// 1024 * 4K Pages = 4MB Total
	VIFStaticDMA.Initialise(512);	// 512 * 4K Pages = 2MB Static DMA
	VIFDynamicDMA.Initialise(256);	// 256 * 4K Pages * 2 Buffers = 2MB Dynamic DMA	
	
	//Backgrounds load test
	if(!Background.LoadBitmap("DTX.bmp", true, false))
	{
		g_bLoop = false;
	}
	if(!Background.LoadBitmap("NTX.bmp", true, false))
	{
		g_bLoop = false;
	}
	if(!Background.LoadBitmap("MKmbg.bmp", true, false))
	{
		g_bLoop = false;
	}
	if(!Background.LoadBitmap("Logo.bmp", true, false))
	{
		g_bLoop = false;
	}
	
	//Initialize Auxiliary texture
	if(!Auxiliary.LoadBitmap("MKaux.bmp", true, false))
	{
		g_bLoop = false;
	}
	
	//Sky Texture
	if(!Skytex.LoadBitmap("Sky.bmp", true, false))
	{
		g_bLoop = false;
	}
	
	//Initialize Karts AI Waypoints
	for(i=0;i<71;i++)
	{
		AIcoor[i][0] = (AIcoor[i][0]/768)*8400;
		AIcoor[i][1] = ((768-AIcoor[i][1])/768)*8400;
	}
	
	//Initialize Terrain Renderer
	TerRenderer GroundMaster(3,3,4,700,ScreenDistance,EyeHeight,EyeDistance);
	if(!GroundMaster.LoadTextures("Track")) {g_bLoop=false;}
	
	//Initialize Kart Renderer
	KartRenderer KartMaster(Karts,8,Kartsize,ScreenDistance,EyeHeight,EyeDistance);
	if(!KartMaster.LoadTextures("Karts1.bmp","Karts2.bmp")) {g_bLoop=false;}
	
	//Initialize Text Rendering
	CFont Font;
	CTexture FontTex;
	if(!Font.Load("font.dat", true))
	{
		g_bLoop = false;
		printf("Can't load font.dat\n");
	}

	if(!FontTex.LoadBitmap("font.bmp", false, true))
	{
		printf("Can't load font.bmp\n");
		g_bLoop = false;
	}


	
	
	// Initialise control pad 0
	if(!pad_init(PAD_0, PAD_INIT_LOCK | PAD_INIT_ANALOGUE | PAD_INIT_PRESSURE))
	{
		printf("Failed to initialise control pad\n");
		pad_cleanup(PAD_0);
		exit(0);
	}										

	// Register our signal function for every possible signal (i.e. ctrl + c)
	for(int sig=0; sig<128; sig++)
		signal(sig, sig_handle);

	// Set up the DMA packet to clear the screen. We want to clear to blue.
	SPS2Manager.InitScreenClear(0, 0x05, 0x10);
	
	Background.Upload(TEXBUF480);
	Background.Select();
	//Main Game Loop
	while(g_bLoop)
	{
		VIFDynamicDMA.Fire();
		pad_update(PAD_0);
		DSP0.HandleAudio();
		if(gamestate==0) //Intro Sequence
		{
			gscounter++;
			SPS2Manager.BeginScene();
			BGS.Render();
			SPS2Manager.EndScene();
			if(gscounter==100)
			{
				Background.LoadBitmap("NTX.bmp", true, false);
				Background.Upload(TEXBUF480);
				Background.Select();
			}
			if(gscounter==200)
			{
				Background.LoadBitmap("DTX.bmp", true, false);
				Background.Upload(TEXBUF480);
				Background.Select();
			}
			if(gscounter>300)
			{
				//BGS.SprEdit(0,0,20,640,512,256,256,0,0);
				Background.LoadBitmap("MKmbg.bmp", true, false);
				Background.Upload(TEXBUF480);
				Background.Select();
				gscounter=0;
				gamestate=1;
				MmBGM.Play();
			}
		}
		else if(gamestate==1) //Main Menu
		{
			if(ifselector==0)
			{
				cursor.MoveTo(-60,120);
			}
			else if(ifselector==1)
			{
				cursor.MoveTo(-80,160);
			}
			else if(ifselector==2)
			{
				cursor.MoveTo(-50,200);
			}
			SPS2Manager.BeginScene();
			Background.Upload(TEXBUF480);
			Background.Select();
			BGS.Render();
			Auxiliary.Upload(TEXBUF480);
			Auxiliary.Select();
			mmstart.Render();
			mmoptions.Render();
			mmexit.Render();
			cursor.Render();
			SPS2Manager.EndScene();
			if(pad[0].pressed & PAD_UP)
			{
				if(ifselector>0)
				{
					ifselector--;
				}
			}
			if(pad[0].pressed & PAD_DOWN)
			{
				if(ifselector<2)
				{
					ifselector++;
				}
			}
			if(pad[0].pressed & PAD_CROSS)
			{
				if(ifselector==0)
				{
					gamestate=3;
					ifselector=0;
					CdBGM.Play();
				}
				else if(ifselector==1)
				{
					gamestate=2;
					ifselector=0;
				}
				else if(ifselector==2)
				{
					g_bLoop=false;
				}
			}
		}
		else if(gamestate==2) //Options Menu
		{
			if(ifselector==0)
			{
				cursor.MoveTo(-55,120);
			}
			else if(ifselector==1)
			{
				cursor.MoveTo(-105,160);
			}
			else if(ifselector==2)
			{
				cursor.MoveTo(-60,200);
			}
			if((maxlaps%10)<8)
			{
				omld2.SprEdit(152,120,128,32,32,32,32,32*(maxlaps%10),32);
			}
			else
			{
				omld2.SprEdit(152,120,128,32,32,32,32,32*((maxlaps%10)-8),64);
			}
			if((maxlaps/10)<8)
			{
				omld1.SprEdit(128,120,130,32,32,32,32,32*(maxlaps/10),32);
			}
			else
			{
				omld1.SprEdit(128,120,130,32,32,32,32,32*((maxlaps/10)-8),64);
			}
			omcm.SprEdit(140,160,128,32,32,32,32,(32*chosenchar),224);
			SPS2Manager.BeginScene();
			Background.Upload(TEXBUF480);
			Background.Select();
			BGS.Render();
			Auxiliary.Upload(TEXBUF480);
			Auxiliary.Select();
			omlaps.Render();
			omcharacter.Render();
			omback.Render();
			omld1.Render();
			omld2.Render();
			omcm.Render();
			cursor.Render();
			SPS2Manager.EndScene();
			if(pad[0].pressed & PAD_UP)
			{
				if(ifselector>0)
				{
					ifselector--;
				}
			}
			if(pad[0].pressed & PAD_DOWN)
			{
				if(ifselector<2)
				{
					ifselector++;
				}
			}
			if(pad[0].pressed & PAD_LEFT)
			{
				if(ifselector==0)
				{
					if(maxlaps>1)
					{
						maxlaps--;
					}
				}
				else if(ifselector==1)
				{
					chosenchar=((chosenchar+7)%8);
				}
			}
			if(pad[0].pressed & PAD_RIGHT)
			{
				if(ifselector==0)
				{
					if(maxlaps<99)
					{
						maxlaps++;
					}
				}
				else if(ifselector==1)
				{
					chosenchar=((chosenchar+1)%8);
				}
			}
			if(pad[0].pressed & PAD_CROSS)
			{
				if(ifselector==2)
				{
					gamestate=1;
					ifselector=0;
				}
			}
		}
		else if(gamestate==3) //Race, initialization phase
		{
			//Initialize Player
			Player.x = 1390;
			Player.y = 0;
			Player.z = 3250;
			Player.alpha = 0;
			Player.beta = 0;
			Player.gamma = 0;
			
			//Initialize Karts array
			Karts[0].x = 1390;
			Karts[0].y =0;
			Karts[0].z = 3250;
			Karts[0].alpha = 0;
			Karts[0].charid = chosenchar;
			Karts[0].visflag=true;
			Karts[1].x = 1390;
			Karts[1].y = 0;
			Karts[1].z = 3450;
			Karts[1].alpha = 0;
			Karts[1].charid = 1;
			Karts[1].visflag=true;
			Karts[2].x = 1390;
			Karts[2].y = 0;
			Karts[2].z = 3650;
			Karts[2].alpha = 0;
			Karts[2].charid = 2;
			Karts[2].visflag=true;
			Karts[3].x = 1390;
			Karts[3].y = 0;
			Karts[3].z = 3850;
			Karts[3].alpha = 0;
			Karts[3].charid = 3;
			Karts[3].visflag=true;
			Karts[4].x = 1550;
			Karts[4].y = 0;
			Karts[4].z = 3350;
			Karts[4].alpha = 0;
			Karts[4].charid = 4;
			Karts[4].visflag=true;
			Karts[5].x = 1550;
			Karts[5].y = 0;
			Karts[5].z = 3550;
			Karts[5].alpha = 0;
			Karts[5].charid = 5;
			Karts[5].visflag=true;
			Karts[6].x = 1550;
			Karts[6].y = 0;
			Karts[6].z = 3750;
			Karts[6].alpha = 0;
			Karts[6].charid = 6;
			Karts[6].visflag=true;
			Karts[7].x = 1550;
			Karts[7].y = 0;
			Karts[7].z = 3950;
			Karts[7].alpha = 0;
			Karts[7].charid = 7;
			Karts[7].visflag=true;
			
			Karts[chosenchar].charid=0;
			
			//Initialize Kart Stats;
			for(i=0;i<8;i++)
			{
				KS[i].laps=0;
				KS[i].cpn=0;
				KS[i].waypoint=0;
				KS[i].spd=0;
				KS[i].lapflag1=false;
				KS[i].lapflag2=false;
				KS[i].finished=false;
			}
			for(i=1;i<8;i++)
			{
				KS[i].spd=float((rand()%6)+35);
			}
			positions[0]=1;
			positions[1]=2;
			positions[2]=3;
			positions[3]=4;
			positions[4]=5;
			positions[5]=6;
			positions[6]=7;
			positions[7]=0;
			
			frindex=0;
			gscounter=0;
			gamestate=4;
		}
		else if(gamestate==4) //Race, count down phase
		{
			gscounter++;
			//Render Graphics
			GroundMaster.GetPlayerPos(Player);
			KartMaster.GetPlayerPos(Player);
			SPS2Manager.BeginScene();
			Skytex.Upload(TEXBUF480);
			Skytex.Select();
			Sky.Render();
			GroundMaster.Render();
			KartMaster.Render();
			Auxiliary.Upload(TEXBUF480);
			Auxiliary.Select();
			if(gscounter<120)
			{
				cdready.Render();
			}
			else if(gscounter<150)
			{
				cd3.Render();
			}
			else if(gscounter<180)
			{
				cd3.Render();
			}
			else if(gscounter<210)
			{
				cd2.Render();
			}
			else if(gscounter<240)
			{
				cd1.Render();
			}
			else
			{
				cdstart.Render();
			}
			SPS2Manager.EndScene();
			if(gscounter>255) 
			{
				gscounter=0;
				gamestate=5;
				RaceBGM.Play();
			}
		}
		else if(gamestate==5) //Race, player control phase
		{
			DSP0.HandleAudio();
			//Player Input
			if(pad[0].pressed & PAD_START)
			{
				gamestate=7;
			}
			if(pad[0].buttons & PAD_LEFT)
			{
				Karts[0].alpha-=0.1;
			}
			if(pad[0].buttons & PAD_RIGHT)
			{
				Karts[0].alpha+=0.1;
			}
			if(pad[0].buttons & PAD_SQUARE)
			{
				if(KS[0].spd>1)
				{KS[0].spd-=1;}
				else
				{KS[0].spd=0;}
			}
			if(pad[0].buttons & PAD_CROSS)
			{
				if(KS[0].spd<48.5)
				{KS[0].spd+=2;}
				else
				{KS[0].spd=50.5;}
			}
			if(KS[0].spd>0.25)
			{
				KS[0].spd-=0.25;
			}
			else
			{
				KS[0].spd=0;
			}
			//Player and AI Karts collision detection
			for(i=1;i<8;i++)
			{
				if((((Karts[0].x-Karts[i].x)*(Karts[0].x-Karts[i].x))+((Karts[0].z-Karts[i].z)*(Karts[0].z-Karts[i].z)))<(Kartsize*Kartsize))
				{
					KS[0].spd=20;
				}
			}
			//AI direction and speed sorting
			for(i=1;i<8;i++)
			{
				wpdst=((Karts[i].x-AIcoor[KS[i].waypoint][0])*(Karts[i].x-AIcoor[KS[i].waypoint][0]))+((Karts[i].z-AIcoor[KS[i].waypoint][1])*(Karts[i].z-AIcoor[KS[i].waypoint][1]));
				wpdst=sqrt(wpdst);
				if(AIcoor[KS[i].waypoint][1]>Karts[i].z)
				{
					Karts[i].alpha=ASin((AIcoor[KS[i].waypoint][0]-Karts[i].x)/wpdst);
				}
				else
				{
					Karts[i].alpha=(PI-ASin((AIcoor[KS[i].waypoint][0]-Karts[i].x)/wpdst));
				}
				if(wpdst<53)
				{
					KS[i].waypoint=(KS[i].waypoint+1)%71;
					KS[i].spd=float((rand()%6)+48);
				}
				
			}
			DSP0.HandleAudio();
			//Calculate Karts Movement
			for(i=0;i<8;i++)
			{
				mallowance=KS[i].spd;
				while(mallowance>0)
				{
					arrindex=768*(int((8399-(Karts[i].z+cos(Karts[i].alpha)))/8400*768))+int((Karts[i].x+sin(Karts[i].alpha))/8400*768);
					switch(Tdata[arrindex])
					{
						//Terrain types
						case 4: 	mallowance-=4;
									break;
						
						case 3: 	mallowance-=2;
									break;
						
						case 2:		mallowance-=1;
									break;
						
						//Invisible walls
						case 0:  	mallowance=0;
									KS[i].spd=0;
									break;
								 
						//Check Points
						default: 	mallowance-=1;
									KS[i].cpn=(Tdata[arrindex]-5);
									if(KS[i].cpn==0) //Start Line
									{
										//Lap counter management
										slzone=(arrindex/768)-393;
										if(slzone==0)
										{
											KS[i].lapflag1=true;
											KS[i].lapflag2=false;
										}
										else if(slzone==1)
										{
											if(KS[i].lapflag1==true)
											{
												if(KS[i].lapflag2==true)
												{
													KS[i].laps++;
												}
												else
												{
													if(KS[i].laps>0)
													{
														KS[i].laps--;
													}
												}
											}
											KS[i].lapflag1=false;
										}
										else if(slzone==2)
										{
											KS[i].lapflag1=true;
											KS[i].lapflag2=true;
										}
									}
									//Determine positions
									for(j=0;j<8;j++)
									{
										for(k=j;k<8;k++)
										{
											if(KS[positions[j]].laps<KS[positions[k]].laps)
											{
												poshelper=positions[j];
												positions[j]=positions[k];
												positions[k]=poshelper;
											}
										}
									}
									for(j=0;j<8;j++)
									{
										for(k=j;k<8;k++)
										{
											if(KS[positions[j]].laps==KS[positions[k]].laps && KS[positions[j]].cpn<KS[positions[k]].cpn)
											{
												poshelper=positions[j];
												positions[j]=positions[k];
												positions[k]=poshelper;
											}
										}
									}
									break;
					}
					if(mallowance>0)
					{
						Karts[i].z+=(cos(Karts[i].alpha));
						Karts[i].x+=(sin(Karts[i].alpha));
					}
				}
			}
			//Sync player position data for rendering
			Player.x = Karts[0].x;
			Player.y = Karts[0].y;
			Player.z = Karts[0].z;
			Player.alpha = Karts[0].alpha;
			//Determine player position in race
			playerposition=0;
			while(positions[playerposition]!=0)
			{
				playerposition++;
			}
			playerposition++;
			
			//End of Race condition
			for(i=1;i<8;i++)
			{
				if(KS[i].laps==(maxlaps+1))
				{
					if(KS[i].finished==false)
					{
						KS[i].finished=true;
						if(frindex<7)
						{
							finalrankings[frindex]=i;
							frindex++;
						}
					}
				}
			}
			if(KS[0].laps==(maxlaps+1))
			{
				if(KS[i].finished==false)
				{
					KS[i].finished=true;
					if(frindex<7)
					{
						finalrankings[frindex]=0;
						frindex++;
					}
				}
				while(frindex<8)
				{
					finalrankings[frindex]=positions[frindex];
					frindex++;
				}
				ifselector=0;
				gamestate=6;
				StatsBGM.Play();
			}
			
			DSP0.HandleAudio();
			//Lap display
			if((KS[0].laps%10)<8)
			{
				rlapd2.SprEdit(228,-220,20001,32,32,32,32,32*(KS[0].laps%10),32);
			}
			else
			{
				rlapd2.SprEdit(228,-220,20001,32,32,32,32,32*((KS[0].laps%10)-8),64);
			}
			if((KS[0].laps/10)<8)
			{
				rlapd1.SprEdit(204,-220,20002,32,32,32,32,32*(KS[0].laps/10),32);
			}
			else
			{
				rlapd1.SprEdit(204,-220,20002,32,32,32,32,32*((KS[0].laps/10)-8),64);
			}
			//Max Laps display
			if((maxlaps%10)<8)
			{
				rmlapd2.SprEdit(300,-220,20003,32,32,32,32,32*(maxlaps%10),32);
			}
			else
			{
				rmlapd2.SprEdit(300,-220,20003,32,32,32,32,32*((maxlaps%10)-8),64);
			}
			if((maxlaps/10)<8)
			{
				rmlapd1.SprEdit(276,-220,20004,32,32,32,32,32*(maxlaps/10),32);
			}
			else
			{
				rmlapd1.SprEdit(276,-220,20004,32,32,32,32,32*((maxlaps/10)-8),64);
			}
			//Position Display
			if((playerposition%10)<8)
			{
				rposd.SprEdit(-280,-220,20003,64,64,32,32,32*(playerposition%10),32);
			}
			else
			{
				rposd.SprEdit(-280,-220,20003,64,64,32,32,32*((playerposition%10)-8),64);
			}
			
			if(playerposition==1)
			{
				rpossuffix.SprEdit(-225,-215,20004,64,32,32,16,96,192);
			}
			else if(playerposition==2)
			{
				rpossuffix.SprEdit(-225,-215,20004,64,32,32,16,96,208);
			}
			else if(playerposition==3)
			{
				rpossuffix.SprEdit(-225,-215,20004,64,32,32,16,128,192);
			}
			else
			{
				rpossuffix.SprEdit(-225,-215,20004,64,32,32,16,128,208);
			}
			
			//Render Graphics
			GroundMaster.GetPlayerPos(Player);
			KartMaster.GetPlayerPos(Player);
			SPS2Manager.BeginScene();
			Skytex.Upload(TEXBUF480);
			Skytex.Select();
			Sky.Render();
			DSP0.HandleAudio();
			GroundMaster.Render();
			DSP0.HandleAudio();
			KartMaster.Render();
			DSP0.HandleAudio();
			//Render HUD
			Auxiliary.Upload(TEXBUF480);
			Auxiliary.Select();
			rlap.Render();
			rlapd1.Render();
			rlapd2.Render();
			rlapdiv.Render();
			rmlapd1.Render();
			rmlapd2.Render();
			rposd.Render();
			rpossuffix.Render();
			DSP0.HandleAudio();
			SPS2Manager.EndScene();
		}
		else if(gamestate==6) //End of Race Statistics
		{
			cursor.MoveTo(-50,200);
			SPS2Manager.BeginScene();
			Skytex.Upload(TEXBUF480);
			Skytex.Select();
			Sky.Render();
			GroundMaster.Render();
			Auxiliary.Upload(TEXBUF480);
			Auxiliary.Select();
			for(i=0;i<7;i++)
			{
				rrndisplay.SprEdit(-150,((40*i)-135),2001,32,32,32,32,(32*(i+1)),32);
				if(i==0)
				{
					rrnsuffix.SprEdit(-120,((40*i)-135),20002,32,16,32,16,96,192);
				}
				else if(i==1)
				{
					rrnsuffix.SprEdit(-120,((40*i)-135),20002,32,16,32,16,96,208);
				}
				else if(i==2)
				{
					rrnsuffix.SprEdit(-120,((40*i)-135),20002,32,16,32,16,128,192);
				}
				else
				{
					rrnsuffix.SprEdit(-120,((40*i)-135),20002,32,16,32,16,128,208);
				}
				rrcm.SprEdit(150,((40*i)-140),20003,32,32,32,32,(32*(Karts[finalrankings[i]].charid)),224);
				rrndisplay.Render();
				rrnsuffix.Render();
				rrcm.Render();
			}
			rrndisplay.SprEdit(-150,(280-140),2001,32,32,32,32,0,64);
			rrnsuffix.SprEdit(-120,(280-135),20002,32,16,32,16,128,208);
			rrcm.SprEdit(150,(280-140),20003,32,32,32,32,(32*(Karts[finalrankings[7]].charid)),224);
			rrndisplay.Render();
			rrnsuffix.Render();
			rrcm.Render();
			rrexit.Render();
			cursor.Render();
			SPS2Manager.EndScene();
			if(pad[0].pressed & PAD_CROSS)
			{
				gamestate=1;
				ifselector=0;
				MmBGM.Play();
			}
		}
		else if(gamestate==7) //Pause menu
		{
			if(ifselector==0)
			{
				cursor.MoveTo(-75,120);
			}
			else if(ifselector==1)
			{
				cursor.MoveTo(-80,160);
			}
			else if(ifselector==2)
			{
				cursor.MoveTo(-50,200);
			}
			SPS2Manager.BeginScene();
			Skytex.Upload(TEXBUF480);
			Skytex.Select();
			Sky.Render();
			GroundMaster.Render();
			KartMaster.Render();
			//Render HUD
			Auxiliary.Upload(TEXBUF480);
			Auxiliary.Select();
			rlap.Render();
			rlapd1.Render();
			rlapd2.Render();
			rlapdiv.Render();
			rmlapd1.Render();
			rmlapd2.Render();
			rposd.Render();
			rpossuffix.Render();
			pmpause.Render();
			pmresume.Render();
			pmrestart.Render();
			pmexit.Render();
			cursor.Render();
			SPS2Manager.EndScene();
			if(pad[0].pressed & PAD_UP)
			{
				if(ifselector>0)
				{
					ifselector--;
				}
			}
			if(pad[0].pressed & PAD_DOWN)
			{
				if(ifselector<2)
				{
					ifselector++;
				}
			}
			if(pad[0].pressed & PAD_CROSS)
			{
				if(ifselector==0)
				{
					gamestate=5;
					ifselector=0;
				}
				else if(ifselector==1)
				{
					CdBGM.Play();
					gamestate=3;
					ifselector=0;
				}
				else if(ifselector==2)
				{
					MmBGM.Play();
					gamestate=1;
					ifselector=0;
				}
			}
			if(pad[0].pressed & PAD_START)
			{
				gamestate=5;
				ifselector=0;
			}
		}
	}
	//Game exit cleanup
	DSP0.Close();
	pad_cleanup(PAD_0);
	return 0;
}

