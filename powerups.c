/*************************************\
* PowerUps handling code              *
\*************************************/
#include <windows.h>
#include <stdio.h>
#include "WolfDef.h"
#include "sprt_def.h"

typedef struct powerup_s
{
	int x, y;
	pow_t type;
	sprite_t *sprt;
	struct powerup_s *prev, *next;
} powerup_t;

powerup_t *powerups=NULL;

powerup_t *Pow_Remove(powerup_t *powerup);
powerup_t *Pow_AddNew(void);

int Pow_Texture[pow_last]=
{
	SPR_STAT_34,		//pow_gibs
	SPR_STAT_38,		//pow_gibs2
	SPR_STAT_6,		//pow_alpo
	SPR_STAT_25,		//pow_firstaid
	SPR_STAT_20,		//pow_key1
	SPR_STAT_21,		//pow_key2
// not used
	SPR_STAT_20,	//pow_key3
	SPR_STAT_20,		//pow_key4

	SPR_STAT_29,		//pow_cross
	SPR_STAT_30,		//pow_chalice
	SPR_STAT_31,		//pow_bible  in fact it is a chest with treasure
	SPR_STAT_32,		//pow_crown
	SPR_STAT_26,		//pow_clip
	SPR_STAT_26,		//pow_clip2
	SPR_STAT_27,		//pow_machinegun
	SPR_STAT_28,		//pow_chaingun
	SPR_STAT_24,		//pow_food
	SPR_STAT_33,		//pow_fullheal
// spear
	SPR_STAT_49,		//pow_25clip
	SPR_STAT_51,		//pow_spear
};
// ------------------------- * Devider * -------------------------

int Pow_Init(void)
{
	Pow_Reset();
	return 1;
}

// finctions to work with lists
void Pow_Reset(void)
{
	powerup_t *powerup=powerups;

	while(powerup)
		powerup=Pow_Remove(powerup);
	powerups=NULL;
}

powerup_t *Pow_Remove(powerup_t *powerup)
{
	powerup_t *next;

	if(powerup==NULL) return NULL;
	if(powerup->prev) powerup->prev->next=powerup->next;
	if(powerup->next) powerup->next->prev=powerup->prev;
	next=powerup->next;
	if(powerups==powerup) powerups=next; //fuck!
	free(powerup);
	return next;
}

powerup_t *Pow_AddNew(void)
{
	powerup_t *newp;
	
	newp=malloc(sizeof(powerup_t));
	newp->prev=NULL;
	newp->next=powerups;
	if(powerups)
		powerups->prev=newp;
	powerups=newp;
	return newp;
}

// ------------------------- * Powerup Code * -------------------------
// ADDME:
// If adding respawn code add it here! Will require another sub,
// which will be called on server side every frame or to say 0.5 sec
// Just remove sprite, when object is picked, leave powerup & set flag & time...

// return 1 is picked up powerup
int Pow_Give(pow_t type)
{
	switch(type)
	{
	case pow_firstaid:
		if(Player.health>=100) return 0;
		Msg_Printf("Picked up a medikt.");
		SD_PlaySound(HEALTH2SND);
		HealSelf(25);
		break;
	case pow_key1:
		Msg_Printf("Picked up a gold key.");
		SD_PlaySound(GETKEYSND);
		GiveKey(KEY_GOLD);
		break;
	case pow_key2:
		Msg_Printf("Picked up a silver key.");
		SD_PlaySound(GETKEYSND);
		GiveKey(KEY_SILVER);
		break;
	case pow_cross:
		Msg_Printf("Picked up a cross.");
		SD_PlaySound(BONUS1SND);
		GivePoints(100);
		gamestate.treasurecount++;
		break;
	case pow_chalice:
		Msg_Printf("Picked up a chalice.");
		SD_PlaySound(BONUS2SND);
		GivePoints(500);
		gamestate.treasurecount++;
		break;
	case pow_bible:
		Msg_Printf("Picked up a chest.");
		SD_PlaySound(BONUS3SND);
		GivePoints(1000);
		gamestate.treasurecount++;
		break;
	case pow_crown:
		Msg_Printf("Picked up a crown.");
		SD_PlaySound(BONUS4SND);
		GivePoints(5000);
		gamestate.treasurecount++;
		break;
	case pow_clip:
		if(Player.ammo>=99) return 0;
		Msg_Printf("Picked up a clip.");
		SD_PlaySound(GETAMMOSND);
		GiveAmmo(8);
		break;
	case pow_clip2:
		if(Player.ammo>=99) return 0;
		Msg_Printf("Picked up a used clip.");
		SD_PlaySound(GETAMMOSND);
		GiveAmmo(4);
		break;
	case pow_machinegun:
		SD_PlaySound(GETMACHINESND);
		Msg_Printf("You got the machine gun!");
		GiveWeapon(WEAPON_AUTO);
		break;
	case pow_chaingun:
		Msg_Printf("You got the chain gun!");
		SD_PlaySound(GETGATLINGSND);
		GiveWeapon(WEAPON_CHAIN);

		gamestate.facecount=0;
		gamestate.getgatgun=50; // 50 frames BJ will smile!!
		break;
	case pow_fullheal:
		Msg_Printf("Extra life!");
		//SD_PlaySound(BONUS1UPSND); // why play it here, if it will be playyed in GiveExtraMan();?
		HealSelf(100);
		GiveAmmo(25);
		GiveExtraMan();
		gamestate.treasurecount++;
		break;
	case pow_food:
		if(Player.health>=100) return 0;
		Msg_Printf("You picked up cold food.");
		SD_PlaySound(HEALTH1SND);
		HealSelf(10);
		break;
	case pow_alpo:
		if(Player.health>=100) return 0;
		Msg_Printf("You picked up dog's food.");
		SD_PlaySound(HEALTH1SND);
		HealSelf(4);
		break;
	case pow_gibs:
		if(Player.health>10) return 0;
		Msg_Printf("BLOOD!");
		SD_PlaySound (SLURPIESND);
		HealSelf(1);
		break;
	case pow_25clip:
		if(Player.ammo>=99) return 0;
		Msg_Printf("Picked up an ammo box.");
		SD_PlaySound(GETAMMOBOXSND);
		GiveAmmo(25);
		break;
	default:
		Con_Printf("Warning: Unknown item type: %d\n", type);
		return 0;
	}
	return 1;
}

// x, y, is in TILES!
void Pow_Spawn(int x, int y, int type)
{
	powerup_t *newp;

	CurMapData.tile_info[x][y]|=TILE_IS_POWERUP;
	newp=Pow_AddNew();
	newp->sprt=Spr_GetNewSprite();
	Spr_SetPos(newp->sprt, TILE2POS(newp->x=x), TILE2POS(newp->y=y), 0);
	newp->type=type;
	newp->sprt->flags|=SPRT_ONE_TEX;
	Spr_SetTex(newp->sprt, 0, Pow_Texture[type]);
	CurMapData.tile_info[x][y]|=TILE_IS_POWERUP;
// good place to update total treasure count!
}

// x, y, is in TILES
// we will get here only if tile powerup flag is set!
// FIXME add an object ref, who wants to pick it up!
void Pow_PickUp(int x, int y)
{
	powerup_t *pow;
	bool p_left=false, p_pick=false;

	for(pow=powerups; pow; pow=pow->next)
	{
check_again:
		if(pow->x==x && pow->y==y)
		{// got a powerup here
			if(Pow_Give(pow->type)) //FIXME script
			{// picked up this stuff, remove it!
				p_pick=true;
				Spr_RemoveSprite(pow->sprt);
				pow=Pow_Remove(pow);
				if(pow)
					goto check_again;
				else
					break;
			}
			else
			{// player do not need it, so may be next time!
				p_left=true;
			}
		}
	}
	if(p_pick) StartBonusFlash();
	if(p_left)
		CurMapData.tile_info[x][y]|= TILE_IS_POWERUP;
	else
		CurMapData.tile_info[x][y]&=~TILE_IS_POWERUP;
}