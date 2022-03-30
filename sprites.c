/*************************************\
* Sprites Handling Code (server)      *
\*************************************/
#include <windows.h>
#include <stdio.h>
#include "WolfDef.h"

// total sprites on level in a moment!
#define MAX_SPRITES 1024

// nobody should see this array!
sprite_t Spr_Sprites[MAX_SPRITES];
int n_of_sprt;

int Spr_Init(void)
{
	Spr_ResetSprites();
	return 1;
}

// called only when client must reconnect
// will not set remove flag!
void Spr_ResetSprites(void)
{
	n_of_sprt=0;
	memset(Spr_Sprites, 0, sizeof(Spr_Sprites));
}

void Spr_RemoveSprite(sprite_t* sprite)
{
	if(sprite==NULL) return;
	sprite->flags|=SPRT_REMOVE;
}

sprite_t* Spr_GetNewSprite(void)
{
	int n;
	sprite_t* sprt;

	for(n=0, sprt=Spr_Sprites; n<n_of_sprt; n++, sprt++)
	{
		if(sprt->flags&SPRT_REMOVE)
		{// free spot: clear it first
			memset(sprt, 0, sizeof(sprite_t));
			return sprt;
		}
	}
	if(n_of_sprt>=MAX_SPRITES)
	{
		Con_Printf("Warning n_of_sprt==MAX_SPRITES\n");
		return NULL;
	}
	return &Spr_Sprites[n_of_sprt++];
}

// ------------------------- * Changing Sprite Data * -------------------------

void Spr_SetPos(sprite_t* sprite, int x, int y, int angle)
{
	if(sprite==NULL) return;
	sprite->x=x;
	sprite->y=y;
	sprite->ang=angle;
	sprite->tilex=x>>TILESHIFT;
	sprite->tiley=y>>TILESHIFT;
	sprite->flags|=SPRT_CHG_POS;

	if(!(x&HALFTILE))  //(x%TILEGLOBAL >= HALFTILE)
		sprite->tilex--;
	if(!(y&HALFTILE))
		sprite->tiley--;
}

void Spr_SetTex(sprite_t* sprite, int index, int tex)
{
	if(sprite==NULL) return;
	sprite->tex[index]=tex;
	sprite->flags|=SPRT_CHG_TEX;
}

// ------------------------- * Visibility list * -------------------------
#define MAXVISABLE 128
visobj_t vislist[MAXVISABLE], *visptr;
// macro to get distance from a void pointer to visobj_t
#define vis_dist(vis) ( ((visobj_t *)vis)->dist )

int Spr_cmpVis(const void *vis1, const void *vis2)
{
	if(    vis_dist(vis1)==vis_dist(vis2) ) return 0; // process equal distance
	return vis_dist(vis1) <vis_dist(vis2) ? 1 : -1;  // if dist > sprite must be first
}

/*
** Will build & sort visibility list of sprites
**
** it is a ready-to-draw list. No platform specific code here!
** soriting from far to near, list built based on spotvis array,
** made by raycaster!
** called only by client, so no server data is avail!
*/
int Spr_CreateVisList(void)
{
	int n;
	sprite_t* sprt;
	int tx, ty;
	int num_visable=0;

	visptr=vislist;

	for(n=0, sprt=Spr_Sprites; n<n_of_sprt; n++, sprt++)
	{
		if(sprt->flags&SPRT_REMOVE) continue;
		tx=sprt->tilex; ty=sprt->tiley;
// can be in any of !4! tiles!, not 9! see definition of tilex & tiley
		if(spotvis[tx][ty] || spotvis[tx+1][ty] || spotvis[tx][ty+1] || spotvis[tx+1][ty+1])
		{// player spoted this!
			visptr->dist=AdvCalcDist(sprt->x, sprt->y); //FIXME!
			visptr->x=sprt->x;
			visptr->y=sprt->y;
			visptr->tex=sprt->tex[0]; //FIXME!
			num_visable++;
			if(num_visable<=MAXVISABLE)
				visptr++;
			else
			 break;
		}
	}
// sorting list
	if(num_visable) // do not sort if zero entries!
		qsort(vislist, num_visable, sizeof(vislist[0]), Spr_cmpVis);

	return num_visable;
}