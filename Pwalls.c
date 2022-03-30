/*************************************\
* PushWalls Code                      *
* Note: as in wolf only one Pwall can *
*       be active at a time           *
\*************************************/
#include <windows.h>
#include <stdio.h>
#include "WolfDef.h"

Pwall_t PWall;

// On New Level Start
void PW_Reset(void)
{
	PWall.active=false;
}

/* called by server, when someone tries to push a secret wall */
/* dir is in which dir pushwall is going to move */
void PW_Push(int x, int y, dir4type dir)
{
	int dx, dy;

	if(PWall.active) return; // another PWall is moving [only one at a time!]

	dx=dx4dir[dir]; dy=dy4dir[dir];

	if(CurMapData.tile_info[x+dx][y+dy]&(TILE_IS_SOLIDTILE | TILE_IS_DOOR))
	{ // noway (smth is blocking)
		Msg_Printf("No way!");
		SD_PlaySound(NOWAYSND);
		return;
	}
	
// remove secret flag & make everything needed when pushwall used!
	CurMapData.tile_info[x][y]&=(~TILE_IS_SECRET);
	Msg_Printf("You have found a secret!");
	gamestate.secretcount++;
	SD_PlaySound(PUSHWALLSND);

// good way to avoid stuckness; [un]coment one more down!
// it makes a tile behind pushwall unpassable
	CurMapData.tile_info[x+dx][y+dy]|=TILE_IS_WALL;

// write down PWall info
	PWall.active=true;
	PWall.PWtilesmoved=PWall.PWpointsmoved=0;
	PWall.dir=dir;
	PWall.x=x; PWall.y=y;
	PWall.dx=dx; PWall.dy=dy;
	PWall.tex=CurMapData.wall_tex[x][y];
}

/* Called by Server each frame to make changes to PWall */
void PW_Process(void)
{
	if(!PWall.active) return; // no active PWall to work with

	PWall.PWpointsmoved+=tics;
	
	if(PWall.PWpointsmoved<128) return;

	PWall.PWpointsmoved-=128;
	PWall.PWtilesmoved++;
// Free tile
	CurMapData.tile_info[PWall.x][PWall.y]&=(~TILE_IS_WALL);
// Occupy new tile
	PWall.x+=PWall.dx;
	PWall.y+=PWall.dy;
	CurMapData.tile_info[PWall.x][PWall.y]|=TILE_IS_WALL; // wall now
	CurMapData.wall_tex[PWall.x][PWall.y]=PWall.tex;
// Shall we move futher?
	if(CurMapData.tile_info[PWall.x+PWall.dx][PWall.y+PWall.dy]&(TILE_IS_SOLIDTILE | TILE_IS_DOOR) ||
	   PWall.PWtilesmoved==3)
	{
// Free Push Wall
		PWall.active=false;
	}
	else
	{
// good way to avoid stuckness; [un]coment one more up!
// it makes a tile behind pushwall unpassable
		CurMapData.tile_info[PWall.x+PWall.dx][PWall.y+PWall.dy]|=TILE_IS_WALL;
	}
}
