#include <windows.h>
#include <stdio.h>
#include "WolfDef.h"

// ---------------- * AI Functions (think for T_Advance) * ---------------------
/*
================================
Called, when actor finished previouse
moving & located in the <center> of the tile!
Actor will try walking in direction
returns:
1 if dir is OK
0 if failed
================================
*/
int AI_ChangeDir(Guard_struct *self, dir8type new_dir)
{
	int oldx, oldy, newx, newy; // all it tiles
	int n;

	oldx=POS2TILE(self->x);
	oldy=POS2TILE(self->y);
	newx=oldx+dx8dir[new_dir];
	newy=oldy+dy8dir[new_dir];

	if(new_dir&0x01) // same as %2 (diagonal dir)
	{
		if(CurMapData.tile_info[newx][oldy] & TILE_IS_SOLIDTILE ||
			 CurMapData.tile_info[oldx][newy] & TILE_IS_SOLIDTILE ||
			 CurMapData.tile_info[newx][newy] & TILE_IS_SOLIDTILE)
			return 0;
		for(n=0; n<NumGuards; n++)
		{
			if(self[n].state>=st_die1) continue;
			if(self[n].tilex==newx && self[n].tiley==newy) return 0; // another guard in path
			if(self[n].tilex==oldx && self[n].tiley==newy) return 0; // another guard in path
			if(self[n].tilex==newx && self[n].tiley==oldy) return 0; // another guard in path
		}
	}
	else // linear dir (E, N, W, S)
	{
		if(CurMapData.tile_info[newx][newy] & TILE_IS_SOLIDTILE) return 0;
		if(CurMapData.tile_info[newx][newy] & TILE_IS_DOOR)
		{
			if(self->type==en_fake || self->type==en_dog) // they can't open doors
			{
				if(DoorMap[newx][newy]->action!=dr_open) // path is blocked by a not opened door
					return 0;
			}
			else
			{
				self->waitfordoor=DoorMap[newx][newy];
				goto moveok;
			}
		}
		for(n=0; n<NumGuards; n++)
		{
			if(Guards[n].state>=st_die1) continue;
			if(Guards[n].tilex==newx && Guards[n].tiley==newy) return 0; // another guard in path
		}
	}

moveok:
	self->tilex=newx;
	self->tiley=newy;

	CurMapData.tile_info[oldx][oldy]&= ~TILE_IS_ACTOR; // update map status
	CurMapData.tile_info[newx][newy]|=	TILE_IS_ACTOR;

	if(CurMapData.areas[newx][newy]>0)
		self->areanumber=CurMapData.areas[newx][newy];
	self->distance=TILEGLOBAL;
	self->dir=new_dir;
	return 1;
}

/*
===============
= Will decide, if 
= moster should
= turn on waypoint
===============
*/
void AI_Path(Guard_struct *self)
{
	if(CurMapData.tile_info[self->x>>TILESHIFT][self->y>>TILESHIFT]&TILE_IS_WAYPOINT)
	{
		long tileinfo=CurMapData.tile_info[self->x>>TILESHIFT][self->y>>TILESHIFT];
		if(tileinfo&TILE_IS_E_TURN)				self->dir=dir8_east;
		else if(tileinfo&TILE_IS_NE_TURN) self->dir=dir8_northeast;
		else if(tileinfo&TILE_IS_N_TURN)	self->dir=dir8_north;
		else if(tileinfo&TILE_IS_NW_TURN) self->dir=dir8_northwest;
		else if(tileinfo&TILE_IS_W_TURN)	self->dir=dir8_west;
		else if(tileinfo&TILE_IS_SW_TURN) self->dir=dir8_southwest;
		else if(tileinfo&TILE_IS_S_TURN)	self->dir=dir8_south;
		else if(tileinfo&TILE_IS_SE_TURN) self->dir=dir8_southeast;
	}
	if(!AI_ChangeDir(self, self->dir)) self->dir=dir8_nodir;
}

/*
==================================
= AI_Dodge
=
= Attempts to choose and initiate a movement for ob that sends it towards
= the player while dodging
=
==================================
*/
void AI_Dodge(Guard_struct *self)
{
	int deltax, deltay, i;
	dir8type dirtry[5], turnaround, tdir;

	if(self->flags&FL_FIRSTATTACK)
	{
// turning around is only ok the very first time after noticing the player
		turnaround=dir8_nodir;
		self->flags&= ~FL_FIRSTATTACK;
	}
	else
		turnaround=opposite8[self->dir];

	deltax=POS2TILE(Player.x)-POS2TILE(self->x);
	deltay=POS2TILE(Player.y)-POS2TILE(self->y);

//
// arange 5 direction choices in order of preference
// the four cardinal directions plus the diagonal straight towards
// the player
//

	if(deltax>0)
		{dirtry[1]=dir8_east; dirtry[3]=dir8_west;}
	else
		{dirtry[1]=dir8_west; dirtry[3]=dir8_east;}

	if(deltay>0)
		{dirtry[2]=dir8_north; dirtry[4]=dir8_south;}
	else
		{dirtry[2]=dir8_south; dirtry[4]=dir8_north;}

// randomize a bit for dodging
	if(abs(deltax)>abs(deltay))
	{
		tdir=dirtry[1]; dirtry[1]=dirtry[2]; dirtry[2]=tdir; // => swap dirtry[1] & dirtry[2]
		tdir=dirtry[3]; dirtry[3]=dirtry[4]; dirtry[4]=tdir; // => swap dirtry[3] & dirtry[4]
	}

	if(US_RndT()<128)
	{
		tdir=dirtry[1]; dirtry[1]=dirtry[2]; dirtry[2]=tdir;
		tdir=dirtry[3]; dirtry[3]=dirtry[4]; dirtry[4]=tdir;
	}

	dirtry[0]=diagonal[dirtry[1]][dirtry[2]];

// try the directions util one works
	for(i=0; i<5; i++)
	{
		if(dirtry[i]==dir8_nodir || dirtry[i]==turnaround) continue;
		if( AI_ChangeDir(self, dirtry[i]) ) return;
	}

// turn around only as a last resort
	if(turnaround!=dir8_nodir)
		if( AI_ChangeDir(self, turnaround) ) return;

	self->dir=dir8_nodir;
}

/*
============================
= AI_Chase
=
= As AI_Dodge, but doesn't try to dodge
============================
*/
void AI_Chase(Guard_struct *self)
{
	int deltax, deltay;
	dir8type d[2];
	dir8type tdir, olddir, turnaround;

	olddir=self->dir;
	turnaround=opposite8[olddir];
	d[0]=d[1]=dir8_nodir;

	deltax=POS2TILE(Player.x)-POS2TILE(self->x);
	deltay=POS2TILE(Player.y)-POS2TILE(self->y);

	if(deltax>0)			d[0]=dir8_east;
	else if(deltax<0)	d[0]=dir8_west;
	
	if(deltay>0)			d[1]=dir8_north;
	else if(deltay<0)	d[1]=dir8_south;

	if(abs(deltay)>abs(deltax))
		{tdir=d[0]; d[0]=d[1]; d[1]=tdir;} // swap d[0] & d[1]

	if(d[0]==turnaround) d[0]=dir8_nodir;
	if(d[1]==turnaround) d[1]=dir8_nodir;

	if(d[0]!=dir8_nodir)
		if( AI_ChangeDir(self, d[0]) ) return;

	if(d[1]!=dir8_nodir)
		if( AI_ChangeDir(self, d[1]) ) return;

	// there is no direct path to the player, so pick another direction
	if(olddir!=dir8_nodir)
		if( AI_ChangeDir(self, olddir) ) return;

	if(US_RndT()>128) // randomly determine direction of search
		for(tdir=dir8_east; tdir<=dir8_south; tdir+=2) // * Revision
		{
			if(tdir!=turnaround)
				if( AI_ChangeDir(self, tdir) ) return;
		}
	else
		for(tdir=dir8_south; tdir>=dir8_east; tdir-=2) // * Revision
		{
			if(tdir!=turnaround)
				if( AI_ChangeDir(self, tdir) ) return;
		}

	if(turnaround!=dir8_nodir)
		if( AI_ChangeDir(self, turnaround) ) return;

	self->dir=dir8_nodir; // can't move
}

/*
============================
= AI_Retreat
= Retreat means "to treat again" (Webster Dictionary) :)
= Run Away from player
============================
*/
void AI_Retreat(Guard_struct *self)
{
	int deltax, deltay;
	dir8type d[2], tdir;

	deltax=(Player.x>>TILESHIFT) - (self->x>>TILESHIFT);
	deltay=(Player.x>>TILESHIFT) - (self->y>>TILESHIFT);

	d[0]=deltax<0 ? dir8_east  : dir8_west;
	d[1]=deltay<0 ? dir8_north : dir8_south;

	if(abs(deltay)>abs(deltax))
		{tdir=d[0]; d[0]=d[1]; d[1]=tdir;} // swap d[0] & d[1]

	if( AI_ChangeDir(self, d[0]) ) return;
	if( AI_ChangeDir(self, d[1]) ) return;

// there is no direct path to the player, so pick another direction

	if(US_RndT()>128) // randomly determine direction of search
	{
		for(tdir=dir8_east; tdir<=dir8_south; tdir+=2) // * Revision
			if( AI_ChangeDir(self, tdir) ) return;
	}
	else
	{
		for(tdir=dir8_south; tdir>=dir8_east; tdir-=2) // * Revision
			if( AI_ChangeDir(self, tdir) ) return;
	}

	self->dir=dir8_nodir;		// can't move
}

/*
=================
= Moves object for distance in global units,
= in ob->dir direction
= Actors are not allowed to move inside the player
= Does NOT check to see if the move is tile map valid
=
= ob->x	= adjusted for new position
= ob->y
=================
*/
void T_Move(Guard_struct *self, long dist)
{

	if(self->dir==dir8_nodir || !dist) return;

	self->x+=dist*dx8dir[self->dir];
	self->y+=dist*dy8dir[self->dir];

// check to make sure it's not on top of player
	if(abs(self->x-Player.x)<=MINACTORDIST)
		if(abs(self->y-Player.y)<=MINACTORDIST)
		{
			if(self->type==en_blinky||
				 self->type==en_clyde	||
				 self->type==en_pinky	||
				 self->type==en_inky	||
				 self->type==en_spectre) TakeDamage(2, self); // ghosts hurt player!
//
// back up
//
			self->x-=dist*dx8dir[self->dir];
			self->y-=dist*dy8dir[self->dir];
			return;
		}

	self->distance-=dist;
	if(self->distance<0) self->distance=0;
}

/*
================================
Will move object forvard
using speed value
================================
*/
void T_Advance(Guard_struct *self, think_t think)
{
	long move;

	if(!think)
	{
		Con_Printf("Warning: Advance without <think> proc\n");
		return;
	}

	move=self->speed*tics;
	while(move>0)
	{
 
// waiting for a door to open
 		if(self->waitfordoor)
		{
			Door_OpenDoor(self->waitfordoor);
			if(self->waitfordoor->action!=dr_open) return; // not opened yet...
			self->waitfordoor=NULL;	// go ahead, the door is now open
		}

		if(move<self->distance)
		{
			T_Move(self, move);
			break;
		}

// fix position to account for round off during moving
		self->x=TILE2POS(self->tilex);
		self->y=TILE2POS(self->tiley);

		move-=self->distance;

// think: Where to go now?
 		think(self);
		self->angle=dir8angle[self->dir];
		if(self->dir==dir8_nodir) return; // all movement is blocked
	}
}

// ------------------------- * Devider * -------------------------
/*
================================
Monster stand on one place &
is looking for any enemy!
================================
*/
void T_Stand(Guard_struct *self)
{
	SightPlayer(self); // smth like FindTarget! FIXME
}

/*
================================
Monster walks, following path &
is looking for any enemy!
================================
*/
void T_Path(Guard_struct *self)
{

	if(SightPlayer(self)) return; // FIXME: FindTarget!

	if(!self->speed) return; // if patroling with a speed of 0

	if(self->dir==dir8_nodir)
	{
		AI_Path(self);
		if(self->dir==dir8_nodir) return; // all movement is blocked
	}

	T_Advance(self, AI_Path);
}

/*
================================
Those color bastards will move
towards player & hurt him, with
a touch!
================================
*/
void T_Ghosts(Guard_struct *self)
{
	if(self->dir==dir8_nodir)
	{
		AI_Chase(self);
		if(self->dir==dir8_nodir) return;	// object is blocked in
		
		self->angle=dir8angle[self->dir];
	}

	T_Advance(self, AI_Chase);
}

/* Guards will try to find player
** They will run after it till death
*/
void T_Chase(Guard_struct *self)
{
	int	dx,dy,dist,chance;
	char dodge;

	// if (gamestate.victoryflag) return;

	dodge=0;
	if(CheckLine(self))	// got a shot at player?
	{
		dx = abs((self->x>>TILESHIFT) - (Player.x>>TILESHIFT));
		dy = abs((self->y>>TILESHIFT) - (Player.y>>TILESHIFT));
		dist=max_of_2(dx, dy);
		if(!dist || (dist==1 && self->distance<16) )
			chance=300;
		else
			chance=(tics<<4)/dist;//100/dist;

		if(US_RndT()<chance)
		{	// go into attack frame
			A_StateChange(self, st_shoot1);
			return;
		}
		dodge=1;
	}

	if(self->dir==dir8_nodir)
	{
		if(dodge)
			AI_Dodge(self);
		else
			AI_Chase(self);
		if (self->dir==dir8_nodir) return; // object is blocked in
		self->angle=dir8angle[self->dir];
	}

	T_Advance(self, dodge ? AI_Dodge : AI_Chase);
}

/*
===============
=
= T_Bite
= Dog will try to hurt player if can reach!
===============
*/
void T_Bite(Guard_struct *self)
{
	long dx, dy;

	PlaySoundLocActor(DOGATTACKSND, self);

	dx=abs(Player.x-self->x)-TILEGLOBAL;
	if(dx<=MINACTORDIST)
	{
		dy=abs(Player.y-self->y)-TILEGLOBAL;
		if(dy<=MINACTORDIST)
			if(US_RndT()<180)
			{
				TakeDamage(US_RndT()>>4, self);
				return;
			}
	}
}

/*
=================
=
= T_DogChase
= FIXME: look in original code!!!
=================
*/
void T_DogChase(Guard_struct *self)
{
	long dx, dy;

	if(self->dir==dir8_nodir)
	{
		AI_Dodge(self);
		self->angle=dir8angle[self->dir];
		if(self->dir==dir8_nodir) return; // object is blocked in
	}

	//
	// check for bite range
	//
	dx=abs(Player.x-self->x)-TILEGLOBAL/2;
	if(dx<=MINACTORDIST)
	{
		dy=abs(Player.y-self->y)-TILEGLOBAL/2;
		if(dy<=MINACTORDIST)
		{
			A_StateChange(self, st_shoot1);
			return; // bite player!
		}
	}

	T_Advance(self, AI_Dodge);
}

/*
=================
= T_BossChase:
= T_Schabb, T_Gift, T_Fat
= they retreat if too close to player!
=================
*/
void T_BossChase(Guard_struct *self)
{
	int	dx, dy, dist;
	unsigned char dodge;

	dodge=0;
	dx=abs(self->tilex - (Player.x>>TILESHIFT) );
	dy=abs(self->tiley - (Player.y>>TILESHIFT) );
	dist=max_of_2(dx, dy);

	if(CheckLine(self))						// got a shot at player?
	{
		if(US_RndT() < tics<<3)
		{	// go into attack frame
			A_StateChange(self, st_shoot1);
			return;
		}
		dodge=1;
	}

	if(self->dir == dir8_nodir)
	{
		if(dodge)
			AI_Dodge(self);
		else
			AI_Chase(self);
		if (self->dir == dir8_nodir)
			return;							// object is blocked in
	}

	T_Advance(self, dist<4 ? AI_Retreat : (dodge ? AI_Dodge : AI_Chase));
}

/*
=================
=
= T_Fake
=
=================
*/
void T_Fake(Guard_struct *self)
{

	if(CheckLine(self)) // got a shot at player?
	{
		if(US_RndT() < tics<<1)
		{	// go into attack frame
			A_StateChange(self, st_shoot1);
			return;
		}
	}

	if(self->dir == dir8_nodir)
	{
		AI_Dodge(self);
		if(self->dir == dir8_nodir) return; // object is blocked in
	}

	T_Advance(self, AI_Dodge);
}

// ------------------------- * Enemy's Attack * -------------------------
/*
=============================================================================

								FIGHT

=============================================================================
*/
/*
===============
= T_Shoot
=
= Try to damage the player, 
= based on skill level and player's speed
===============
*/
void T_Shoot(Guard_struct *self)
{
	int	dx, dy, dist;
	int	hitchance, damage;

	if(!areabyplayer[self->areanumber])	return;
	if(!CheckLine(self)) return; // player is behind a wall

	dx=abs((self->x>>TILESHIFT)-(Player.x>>TILESHIFT));
	dy=abs((self->y>>TILESHIFT)-(Player.y>>TILESHIFT));
	dist=max_of_2(dx, dy);

	if(self->type==en_ss || self->type==en_boss)
		dist=dist*2/3;					// ss are better shots

	if(Player.movspd)  // if running: FIXME!
		hitchance=160;
	else
		hitchance=256;

// if guard is visible by player
// player can see to dodge
// So left only check if guard is in player's fov: FIXME: not fixed fov!
 	if(AngDiff(TransformPoint(self->x, self->y), Player.angle)<ANG_HFOV)
		hitchance-=dist*16;
	else
		hitchance-=dist*8;

// see if the shot was a hit
	if(US_RndT()<hitchance)
	{
		if(dist<2)
			damage=US_RndT()>>2;
		else if(dist<4)
			damage=US_RndT()>>3;
		else
			damage=US_RndT()>>4;

		TakeDamage(damage, self);
	}

	switch(self->type)
	{
	 case en_ss:
	   PlaySoundLocActor(SSFIRESND, self);
	   break;
	 case en_gift:
	 case en_fat:
	 case en_mecha:
	 case en_hitler:
	 case en_boss:
	   PlaySoundLocActor(BOSSFIRESND, self);
	   break;
	 default:
	   PlaySoundLocActor(NAZIFIRESND, self);
	}
}

/*
===============
=
= T_Launch
=
===============
*/
void T_Launch(Guard_struct *self)
{
	Guard_struct *proj;
	int iangle;

	iangle=TransformPoint(self->x, self->y)+ANG_180;
	if(iangle>ANG_360) iangle-=ANG_360;

	if(self->type==en_death)
	{// death knight launches 2 rockets with 4 degree shift each.
		T_Shoot(self);
		if(self->state == st_shoot2)
			iangle=NormalizeAngle( iangle-DEG2FINE(4) );
		else
			iangle=NormalizeAngle( iangle+DEG2FINE(4) );
	}

	proj=GetNewActor();
	if(proj==NULL) return;

	proj->x=self->x;
	proj->y=self->y;
	proj->tilex=self->tilex;
	proj->tiley=self->tiley;
	proj->state=st_stand;
	proj->ticks=1;
	proj->dir=dir8_nodir;
	proj->angle=iangle;
	proj->speed=0x2000;
	proj->flags=FL_NONMARK; // FL_NEVERMARK;
	proj->sprite=Spr_GetNewSprite();

	switch(self->type)
	{
	case en_death:
		proj->type=en_hrocket;
		//PlaySoundLocActor(KNIGHTMISSILESND, self);
		break;
	case en_angel:
		proj->type=en_spark;
		//PlaySoundLocActor(ANGELFIRESND, self);
		break;
	case en_fake:
		proj->type=en_fire;
		proj->state=st_path1;
		proj->flags=FL_NEVERMARK;
		proj->speed=0x1200;
		PlaySoundLocActor(FLAMETHROWERSND, self);
		break;
	case en_schabbs:
		proj->type=en_needle;
		proj->state=st_path1;
		PlaySoundLocActor(SCHABBSTHROWSND, self);
		break;
	default:
		proj->type=en_rocket;
		PlaySoundLocActor(MISSILEFIRESND, self);
	}
}
