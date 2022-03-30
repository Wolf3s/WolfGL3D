#include <windows.h>
#include <stdio.h>
#include "wolfdef.h"
#include "sprt_def.h"
#include <GL\gl.h>
#include <GL\glu.h>

leveldef CurMapData;
int TotalLevels=0;

int Lvl_Init(void)
{
	memset(&CurMapData, 0, sizeof(CurMapData));
	
	TotalLevels=File_MAP_OpenMapFiles(WolfVer.file_maph, WolfVer.file_mapm);
	if(!TotalLevels) return 0;

	Con_Printf("Lvl_Init: Done.\n");
	return 1;
}

void Lvl_SpawnStatic(int type, int x, int y)
{
	sprite_t *spr;

	if(statinfo[type].powerup==-1)
	{
		if(statinfo[type].block)
		{ // blocking static
			CurMapData.tile_info[x][y]|=TILE_IS_BLOCK;
			spr=Spr_GetNewSprite();
			if(spr==NULL) return;
			Spr_SetPos(spr, TILE2POS(x), TILE2POS(y), 0);
			Spr_SetTex(spr, 0, SPR_STAT_0+type);
		}
		else
		{// dressing static

			CurMapData.tile_info[x][y]|=TILE_IS_DRESS;
			spr=Spr_GetNewSprite();
			if(spr==NULL) return;
			Spr_SetPos(spr, TILE2POS(x), TILE2POS(y), 0);
			Spr_SetTex(spr, 0, SPR_STAT_0+type);
		}
	}
	else
	{
		Pow_Spawn(x, y, statinfo[type].powerup);
		if(statinfo[type].powerup==pow_cross || 
			 statinfo[type].powerup==pow_chalice ||
			 statinfo[type].powerup==pow_bible ||
			 statinfo[type].powerup==pow_crown ||
			 statinfo[type].powerup==pow_fullheal)
				gamestate.treasuretotal++; 
	}
}

void Lvl_SpawnObj(int type, int x, int y)
{
	if(type>=23 && type<23+num_statics)
	{// static object
		Lvl_SpawnStatic(type-23, x, y);
		return;
	}

	switch(type)
	{
	case 0x13: // start N
		CurMapData.pSpawn.x=x;
		CurMapData.pSpawn.y=y;
		CurMapData.pSpawn.angle=ANG_90;
		break;
	case 0x14: // start E
		CurMapData.pSpawn.x=x;
		CurMapData.pSpawn.y=y;
		CurMapData.pSpawn.angle=ANG_0;
		break;
	case 0x15: // start S
		CurMapData.pSpawn.x=x;
		CurMapData.pSpawn.y=y;
		CurMapData.pSpawn.angle=ANG_270;
		break;
	case 0x16: // start W
		CurMapData.pSpawn.x=x;
		CurMapData.pSpawn.y=y;
		CurMapData.pSpawn.angle=ANG_180;
		break;
	case 0x5a: // turn E
		CurMapData.tile_info[x][y]|=TILE_IS_E_TURN;//FIXME!
		break;
	case 0x5b: // turn NE
		CurMapData.tile_info[x][y]|=TILE_IS_NE_TURN;//FIXME!
		break;
	case 0x5c: // turn N
		CurMapData.tile_info[x][y]|=TILE_IS_N_TURN;//FIXME!
		break;
	case 0x5d: // turn NW
		CurMapData.tile_info[x][y]|=TILE_IS_NW_TURN;//FIXME!
		break;
	case 0x5e: // turn W
		CurMapData.tile_info[x][y]|=TILE_IS_W_TURN;//FIXME!
		break;
	case 0x5f: // turn SW
		CurMapData.tile_info[x][y]|=TILE_IS_SW_TURN;//FIXME!
		break;
	case 0x60: // turn S
		CurMapData.tile_info[x][y]|=TILE_IS_S_TURN;//FIXME!
		break;
	case 0x61: // turn SE
		CurMapData.tile_info[x][y]|=TILE_IS_SE_TURN;//FIXME!
		break;
	case 0x62: // pushwall modifier
		CurMapData.tile_info[x][y]|=TILE_IS_SECRET;
		gamestate.secrettotal++;
		break;
	case 0x63: // End Game triger
		CurMapData.tile_info[x][y]|=TILE_IS_EXIT;
		break;
	// spawn guards
	}
}

// loads a map with a one based index n (e1m1=1, e2m5=15)
// it just fills in the 'leveldef' struct
int Lvl_LoadLevel(int n)
{
	int zero_=0;

typedef struct
{
	int Mode;
	float Color[4];
	float Density;		// for Exp & Exp2
	float Start, End;	// for Linear (it suck, IMHO)
} fog_type;
	int x, y0, y, l1, l2, l3;
	fog_type FogStat;
	int xx, yy, yymap,lightcnt=0;
	GLfloat lightPosition[4];
    GLfloat lightAmbient2[]  = {0.1f, 1.0f, 1.0f, 0.3f} ;
    GLfloat lightDiffuse[]  = {0.1f, 1.0f, 1.0f, 0.3f} ;
    GLfloat lightSpecular[] = {0.1f, 1.0f, 1.0f, 0.3f} ;
	GLfloat lightDirection[] = {0.0f, -1.0f, 0.0f, 1.0f} ;

	//GLfloat lightAmbient[] = {0.5f, 0.5f, 0.5f, 1.0f} ;
    //glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lightAmbient) ;
	Pow_Reset();
	Spr_ResetSprites();

	memset(&CurMapData, 0, sizeof(CurMapData));
	if(!File_MAP_ReadData(n,	CurMapData.layer1, 
														CurMapData.layer2, 
														CurMapData.layer3, 
														CurMapData.name)) return 0;

// ------------------------- * Parsing Layers * -------------------------
	for(y0=0; y0<64; y0++)
		for(x=0; x<64; x++)
	{
		y=63-y0;
		l1=CurMapData.layer1[y0*64+x];
		l2=CurMapData.layer2[y0*64+x];
		l3=CurMapData.layer3[y0*64+x];

// if server, process obj layer!
		if(l2) Lvl_SpawnObj(l2, x, y);

// Map data layer
		if(l1==0)
		{
			CurMapData.areas[x][y]=-3; // unknown area
		}			
		else if(l1<0x6a) // solid map object
		{
			if((l1>=0x5A && l1<=0x5F) || l1==0x64 || l1==0x65) // door
			{
				CurMapData.tile_info[x][y]|=TILE_IS_DOOR;
				Door_SpawnDoor(x, y, l1);
				CurMapData.areas[x][y]=-2; // door area
			}
			else
			{
				CurMapData.tile_info[x][y]|=TILE_IS_WALL;
				CurMapData.wall_tex[x][y]=(l1-1)*2; // spawn a wall
					// FIXME: [*2] we are saving loads of mul, but...
				CurMapData.areas[x][y]=-1; // wall area
				if(l1==0x15) // elevator
				{//////////0x15==21, 21 is elevator Map ID (added by Idot)
					CurMapData.tile_info[x][y]|=TILE_IS_ELEVATOR;
				}
			}
		}
		else if(l1==0x6a) // Ambush floor tile
		{
			CurMapData.tile_info[x][y]|=TILE_IS_AMBUSH;
			CurMapData.areas[x][y]=-3; // unknown area
		}
		else if(l1>=FIRSTAREA && l1<(FIRSTAREA+NUMAREAS)) // area
		{
			if(l1==FIRSTAREA)
				CurMapData.tile_info[x][y]|=TILE_IS_SLEVEL; // secret level
			CurMapData.areas[x][y]=l1-FIRSTAREA;// spawn area
		}
		else
		{
			CurMapData.areas[x][y]=-3; // unknown area
		}
// End of the map data layer
	}
	CurMapData.music=levelsongs[n-1];
	CurMapData.celing=WolfPal[celing_color[n-1]];
	CurMapData.floor=WolfPal[0x19]; // standart floor color
		FogStat.Color[0]=(float)WolfPal[celing_color[n-1]].r/256;
		FogStat.Color[1]=(float)WolfPal[celing_color[n-1]].g/256;
		FogStat.Color[2]=(float)WolfPal[celing_color[n-1]].b/256;
		FogStat.Color[3]=(float)0.3;
		glFogfv(GL_FOG_COLOR, FogStat.Color);
		Vid_InitCelFloor(CurMapData.celing.r,CurMapData.floor.r,CurMapData.celing.g,CurMapData.floor.g,CurMapData.celing.b,CurMapData.floor.b);
	/*for(xx=0; xx<64; xx++)
	for(yy=0, yymap=63; yy<64; yy++, yymap--)
	{
	if(CurMapData.tile_info[xx][yymap]&TILE_IS_DRESS)
	{
		    lightcnt++;
			lightPosition[0] = (float)xx;
			lightPosition[1] = (UPPERZCOORD-LOWERZCOORD)/2;
			lightPosition[2] = (float)yy;
			lightPosition[3] = 1.0f;
			glLightfv(GL_LIGHT0+lightcnt, GL_DIFFUSE,lightDiffuse);
			glLightfv(GL_LIGHT0+lightcnt, GL_AMBIENT,lightAmbient2);
			glLightfv(GL_LIGHT0+lightcnt, GL_POSITION, lightPosition);
			glEnable(GL_LIGHT0+lightcnt);
			
			
	}
	}*/		
	//Loading Iron-Eagles
	//IronEagles=0;
	//for(y0=0; y0<64; y0++)
	//	for(x=0; x<64; x++)
	//		if (CurMapData.wall_tex[x][y]==100) IronEagles++;
	ResetMapModels();
	for(y0=0; y0<64; y0++)
		for(x=0; x<64; x++)
			switch (CurMapData.wall_tex[x][y0]){
				case 6: AddPoster(&x,&y0,&zero_);
					break;
				case 10: AddIronEagle(&x,&y0,&zero_);
					break;
				case 20: AddPortrait(&x,&y0,&zero_);
					;
			}	
		/*
	for(y0=0; y0<64; y0++)
		for(x=0; x<64; x++)
			switch (CurMapData.wall_tex[x][y0]){
				case 31: AddCross(&x,&y0);
			}	
			*/
	return 1;
}