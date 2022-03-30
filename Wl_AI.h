
#define BJRUNSPEED	2048
#define BJJUMPSPEED	680
/*
===============
=
= SpawnBJVictory
=
===============
*/
void SpawnBJVictory(void)
{
	Guard_struct *bj;

	bj=SpawnActor(en_bj, Player.x>>TILESHIFT, Player.y>>TILESHIFT, dir4_north);
	if(!bj) return;

	bj->x=Player.x;
	bj->y=Player.y;
	bj->state=st_path1;
	bj->speed=BJRUNSPEED;
	bj->flags=FL_NONMARK; // FL_NEVERMARK;
	bj->temp2=6;
}

/*
===============
=
= T_BJRun
=
===============
*/
void T_BJRun(Guard_struct *Guard)
{
//	MoveObj(Guard, Guard->speed);
	
	if(!Guard->distance)
	{
		Guard->distance=TILEGLOBAL;
		if ( !(-- Guard->temp2) )
		{
			A_StateChange(Guard, st_shoot1);
			Guard->speed=BJJUMPSPEED;
			return;
		}
	}
}

/*
===============
=
= T_BJJump
=
===============
*/
void T_BJJump(Guard_struct *Guard)
{
//	MoveObj(Guard, Guard->speed);
}

/*
===============
=
= T_BJYell
=
===============
*/
void T_BJYell(Guard_struct *Guard)
{
	PlaySoundLocActor(YEAHSND, Guard);	// JAB
}

/*
===============
=
= T_BJDone
=
===============
*/
void T_BJDone(Guard_struct *Guard)
{
	playstate = ex_victorious;				// exit castle tile
}

// ------------------------- * Additional Functions * -------------------------


// ------------------------- * Devider * -------------------------

/*
===============
=
= SightPlayer
=
= Called by actors that ARE NOT chasing the player.  If the player
= is detected (by sight, noise, or proximity), the actor is put into
= it's combat frame and true is returned.
=
= Incorporates a random reaction delay
=
===============
*/
char SightPlayer(Guard_struct *Guard)
{
	//if (Guard->flags & FL_ATTACKMODE)
		//Quit ("An actor in ATTACKMODE called SightPlayer!");

	if (Guard->temp2)
	{
	//
	// count down reaction time
	//
		Guard->temp2-=tics;
		if (Guard->temp2 > 0)	return 0;
		Guard->temp2 = 0;	// time to react
	}
	else
	{
		if (Guard->flags & FL_AMBUSH)
		{
			if (!CheckSight (Guard))return 0;
			Guard->flags &= ~FL_AMBUSH;
		}
		else
		{
			if (!areabyplayer[Guard->areanumber])	return 0;
			if (!Player.madenoise && !CheckSight (Guard)) return 0;
		}
		
		// if we are here we see/hear player!!!

		switch (Guard->type)
		{
		case en_guard:
			Guard->temp2 = 1+US_RndT()/4;
			break;
		case en_officer:
			Guard->temp2 = 2;
			break;
		case en_mutant:
			Guard->temp2 = 1+US_RndT()/6;
			break;
		case en_ss:
			Guard->temp2 = 1+US_RndT()/6;
			break;
		case en_dog:
			Guard->temp2 = 1+US_RndT()/8;
			break;
		case en_boss:
		case en_schabbs:
		case en_fake:
		case en_mecha:
		case en_hitler:
		case en_gretel:
		case en_gift:
		case en_fat:
		case en_spectre:
		case en_angel:
		case en_trans:
		case en_uber:
		case en_will:
		case en_death:
			Guard->temp2 = 1;
			break;
		}
		return 0;  // we are amazed & waiting to understand what to do!
	}

	FirstSighting (Guard);

	return 1;
}

/*
===============
=
= FirstSighting
=
= Puts an actor into attack mode and possibly reverses the direction
= if the player is behind it
=
===============
*/
void FirstSighting (Guard_struct *Guard)
{
//
// react to the player
//
	if(notarget) return; // enemy doesn`t react on player
	switch (Guard->type)
	{
	case en_guard:
		PlaySoundLocActor(HALTSND, Guard);
		A_StateChange(Guard, st_chase1);
		Guard->speed *= 3;			// go faster when chasing player
		break;
	case en_officer:
		PlaySoundLocActor(SPIONSND, Guard);
		A_StateChange(Guard, st_chase1);
		Guard->speed *= 5;			// go faster when chasing player
		break;
	case en_mutant:
		A_StateChange(Guard, st_chase1);
		Guard->speed *= 3;			// go faster when chasing player
		break;
	case en_ss:
		PlaySoundLocActor(SCHUTZADSND,Guard);
		A_StateChange(Guard, st_chase1);
		Guard->speed *= 4;			// go faster when chasing player
		break;
	case en_dog:
		PlaySoundLocActor(DOGBARKSND,Guard);
		A_StateChange(Guard, st_chase1);
		Guard->speed *= 2;			// go faster when chasing player
		break;

	case en_boss:
		SD_PlaySound(GUTENTAGSND);
		A_StateChange(Guard, st_chase1);
		Guard->speed = SPDPATROL*3;	// go faster when chasing player
		break;
	case en_gretel:
		SD_PlaySound(KEINSND);
		A_StateChange(Guard, st_chase1);
		Guard->speed *= 3;			// go faster when chasing player
		break;
	case en_gift:
		SD_PlaySound(EINESND);
		A_StateChange(Guard, st_chase1);
		Guard->speed *= 3;			// go faster when chasing player
		break;
	case en_fat:
		SD_PlaySound(ERLAUBENSND);
		A_StateChange(Guard, st_chase1);
		Guard->speed *= 3;			// go faster when chasing player
		break;
	case en_schabbs:
		SD_PlaySound(SCHABBSHASND);
		A_StateChange(Guard, st_chase1);
		Guard->speed *= 3;			// go faster when chasing player
		break;
	case en_fake:
		SD_PlaySound(TOT_HUNDSND);
		A_StateChange(Guard, st_chase1);
		Guard->speed *= 3;			// go faster when chasing player
		break;
	case en_mecha:
		SD_PlaySound(DIESND);
		A_StateChange(Guard, st_chase1);
		Guard->speed *= 3;			// go faster when chasing player
		break;
	case en_hitler:
		SD_PlaySound(DIESND);
		A_StateChange(Guard, st_chase1);
		Guard->speed *= 5;			// go faster when chasing player
		break;
	case en_blinky:
	case en_clyde:
	case en_pinky:
	case en_inky:
		A_StateChange(Guard, st_chase1);
		Guard->speed *= 2;			// go faster when chasing player
		break;

/* ************ Spear ***********
	case en_spectre:
		SD_PlaySound(GHOSTSIGHTSND);
		A_StateChange(Guard, st_chase1);
		Guard->speed = 800;			// go faster when chasing player
		break;
	case en_angel:
		SD_PlaySound(ANGELSIGHTSND);
		A_StateChange(Guard, st_chase1);
		Guard->speed = 1536;			// go faster when chasing player
		break;
	case en_trans:
		SD_PlaySound(TRANSSIGHTSND);
		A_StateChange(Guard, st_chase1);
		Guard->speed = 1536;			// go faster when chasing player
		break;
	case en_uber:
		A_StateChange(Guard, st_chase1);
		Guard->speed = 3000;			// go faster when chasing player
		break;
	case en_will:
		SD_PlaySound(WILHELMSIGHTSND);
		A_StateChange(Guard, st_chase1);
		Guard->speed = 2048;			// go faster when chasing player
		break;
	case en_death:
		SD_PlaySound(KNIGHTSIGHTSND);
		A_StateChange(Guard, st_chase1);
		Guard->speed = 2048;			// go faster when chasing player
		break;
*/
	}

	if(Guard->waitfordoor) Guard->waitfordoor=NULL;	// ignore the door opening command

	Guard->dir=dir8_nodir;
	Guard->flags |= FL_ATTACKMODE|FL_FIRSTATTACK;
}

/*
================
=
= CheckSight
=
= Checks a straight line between player and current object
=
= If the sight is ok, check alertness and angle to see if they notice
=
= returns true if the player has been spoted
=
================
*/
#define MINSIGHT 0x18000

char CheckSight(Guard_struct *Guard)
{
	int deltax, deltay;

//
// don't bother tracing a line if the area isn't connected to the player's
//
	if(!(Guard->flags & FL_AMBUSH))
		if(!areabyplayer[Guard->areanumber]) return 0;
//
// if the player is real close, sight is automatic
//
	deltax = Player.x - Guard->x;
	deltay = Player.y - Guard->y;

	if(abs(deltax)<MINSIGHT && abs(deltay)<MINSIGHT) return 1;

//
// see if they are looking in the right direction
//
	switch (Guard->dir)
	{
	case dir8_north:
		if (deltay < 0)	return 0;
		break;
	case dir8_east:
		if (deltax < 0) return 0;
		break;
	case dir8_south:
		if (deltay > 0) return 0;
		break;
	case dir8_west:
		if (deltax > 0) return 0;
		break;
	}

//
// trace a line to check for blocking tiles (corners)
//
	return CheckLine(Guard);
}

/*
=====================
= CheckLine
=
= Returns true if a straight line between the player and ob is unobstructed
=====================
*/
char CheckLine(Guard_struct *Guard)
{
	return Map_CheckLine(Guard->x, Guard->y, Player.x, Player.y);
}


/*
===================
=
= ProjectileTryMove
=
= returns true if move ok
===================
*/
#define PROJSIZE	0x2000

char ProjectileTryMove (Guard_struct *Guard)
{
	int xl, yl, xh, yh, x, y;

	xl=(Guard->x-PROJSIZE)>>TILESHIFT;
	yl=(Guard->y-PROJSIZE)>>TILESHIFT;

	xh=(Guard->x+PROJSIZE)>>TILESHIFT;
	yh=(Guard->y+PROJSIZE)>>TILESHIFT;

	// Checking for solid walls:
	for(y=yl; y<=yh; y++)
	{
		for(x=xl; x<=xh; x++)
		{
			// FIXME: decide what to do with statics & Doors!
			if(CurMapData.tile_info[x][y]&(TILE_IS_WALL|TILE_IS_BLOCK)) return 0;
			if(CurMapData.tile_info[x][y]&TILE_IS_DOOR)
			{
				if(Door_Opened(x, y)!=DOOR_FULLOPEN) return 0;
			}
		}
	}
/* Projectile will fly through objects (even guards & columns) - must fix to create rocket launcher!
	for(n=0; n<NumGuards; n++)
	{
		if(Guards[n].state>=st_die1) continue;
		deltax = Player.x-Guards[n].x;
		if (deltax < -MINACTORDIST || deltax > MINACTORDIST) continue;
		deltay = Player.y-Guards[n].y;
		if (deltay < -MINACTORDIST || deltay > MINACTORDIST) continue;

		return 0;
	}*/
	return 1;
}

/*
=================
=
= T_Projectile
=
=================
*/
#define PROJECTILESIZE	0xc000

void T_Projectile (Guard_struct *Guard)
{
	long	deltax,deltay;
	int		damage;
	long	speed;

	speed = Guard->speed*tics;

	deltax = (long)(speed*CosTable[Guard->angle]);
	deltay = (long)(speed*SinTable[Guard->angle]);

	if (deltax>TILEGLOBAL) deltax = TILEGLOBAL;
	if (deltax<-TILEGLOBAL) deltax = -TILEGLOBAL; // my
	if (deltay>TILEGLOBAL) deltay = TILEGLOBAL;
	if (deltay<-TILEGLOBAL) deltay = -TILEGLOBAL; // my

	Guard->x += deltax;
	Guard->y += deltay;

	deltax = LABS(Guard->x - Player.x);
	deltay = LABS(Guard->y - Player.y);

	if (!ProjectileTryMove(Guard))
	{
		if (Guard->type == en_rocket || Guard->type == en_hrocket)
		{ // rocket run into obstacle, draw explosion!
			PlaySoundLocActor(MISSILEHITSND, Guard);
			A_StateChange(Guard, st_die1);
		}
		else
		{
			A_StateChange(Guard, st_remove); // mark for removal
		}
		return;
	}

	if (deltax < PROJECTILESIZE && deltay < PROJECTILESIZE)
	{	// hit the player
		switch (Guard->type)
		{
		case en_needle:
			damage = (US_RndT() >>3) + 20;
			break;
		case en_rocket:
		case en_hrocket:
		case en_spark:
			damage = (US_RndT() >>3) + 30;
			break;
		case en_fire:
			damage = (US_RndT() >>3);
			break;
		default:
			damage=0;
			break;
		}

		TakeDamage(damage, Guard);
		A_StateChange(Guard, st_remove); // mark for removal
		return;
	}

	Guard->tilex = Guard->x >> TILESHIFT;
	Guard->tiley = Guard->y >> TILESHIFT;
}

/*
===================
=
= DamageActor
=
= Called when the player succesfully hits an enemy.
=
= Does damage points to enemy ob, either putting it into a stun frame or
= killing it.
=
===================
*/
void DamageActor (Guard_struct *Guard, unsigned damage)
{
	Player.madenoise = 1;

// do double damage if shooting a non attack mode actor
	if ( !(Guard->flags & FL_ATTACKMODE) )	damage <<= 1;

	Guard->health -= damage;

	if (Guard->health<=0)
		KillActor(Guard);
	else
	{
		if ( !(Guard->flags & FL_ATTACKMODE) )
			FirstSighting (Guard);		// put into combat mode

		switch (Guard->type)		// dogs only have one hit point
		{
		case en_guard:
		case en_officer:
		case en_mutant:
		case en_ss:
			if (Guard->health&1)
				A_StateChange(Guard, st_pain);
			else
				A_StateChange(Guard, st_pain1);
			break;
		}
	}
}

/*
===============
=
= KillActor
=
===============
*/
void KillActor (Guard_struct *Guard)
{
	int	tilex,tiley;

	tilex = Guard->tilex = Guard->x >> TILESHIFT;		// drop item on center
	tiley = Guard->tiley = Guard->y >> TILESHIFT;

	switch (Guard->type)
	{
	case en_guard:
		GivePoints (100);
		PlaceItemType(pow_clip2,tilex,tiley);
		break;
	case en_officer:
		GivePoints (400);
		PlaceItemType(pow_clip2,tilex,tiley);
		break;
	case en_mutant:
		GivePoints (700);
		PlaceItemType (pow_clip2,tilex,tiley);
		break;
	case en_ss:
		GivePoints (500);
		if (Player.weapons & WEAPONS_AUTO) // have a schmeiser?
			PlaceItemType (pow_clip2,tilex,tiley);
		else
			PlaceItemType (pow_machinegun,tilex,tiley);
		break;
	case en_dog:
		GivePoints (200);
		break;

	case en_boss:
		GivePoints (5000);
		PlaceItemType (pow_key1,tilex,tiley);
		break;
	case en_gretel:
		GivePoints (5000);
		PlaceItemType (pow_key1,tilex,tiley);
		break;
	case en_gift:
		GivePoints (5000);
		//gamestate.killx = player->x;
		//gamestate.killy = player->y;
		break;
	case en_fat:
		GivePoints (5000);
		//gamestate.killx = player->x;
		//gamestate.killy = player->y;
		break;
	case en_schabbs:
		GivePoints (5000);
		//gamestate.killx = player->x;
		//gamestate.killy = player->y;
		A_DeathScream(Guard);
		break;
	case en_fake:
		GivePoints (2000);
		break;
	case en_mecha:
		GivePoints (5000);
		break;
	case en_hitler:
		GivePoints (5000);
		//gamestate.killx = player->x;
		//gamestate.killy = player->y;
		A_DeathScream(Guard);
		break;
/* *****Spear*******
	case spectreobj:
		GivePoints (200);
		NewState (ob,&s_spectredie1);
		break;

	case angelobj:
		GivePoints (5000);
		NewState (ob,&s_angeldie1);
		break;

	case transobj:
		GivePoints (5000);
		NewState (ob,&s_transdie0);
		PlaceItemType (bo_key1,tilex,tiley);
		break;

	case uberobj:
		GivePoints (5000);
		NewState (ob,&s_uberdie0);
		PlaceItemType (bo_key1,tilex,tiley);
		break;

	case willobj:
		GivePoints (5000);
		NewState (ob,&s_willdie1);
		PlaceItemType (bo_key1,tilex,tiley);
		break;

	case deathobj:
		GivePoints (5000);
		NewState (ob,&s_deathdie1);
		PlaceItemType (bo_key1,tilex,tiley);
		break;
*/
	}

	A_StateChange(Guard, st_die1);
	gamestate.killcount++;
	Guard->flags &= ~FL_SHOOTABLE;
	//actorat[ob->tilex][ob->tiley] = NULL;
	Guard->flags |= FL_NONMARK;
}

char CanCloseDoor(int x, int y, char vert)
{
	int n;

	if (Player.x>>TILESHIFT==x && Player.y>>TILESHIFT==y) return 0;

	if (vert)
	{
		if (Player.y>>TILESHIFT == y)
		{
			if ( ( (Player.x+CLOSEWALL) >> TILESHIFT ) == x ) return 0;
			if ( ( (Player.x-CLOSEWALL) >> TILESHIFT ) == x )	return 0;
		}
		for(n=0; n<NumGuards; n++)
		{
			if(Guards[n].tilex==x && Guards[n].tiley==y) return 0; // guard in door
			if(Guards[n].tilex==x-1 && Guards[n].tiley==y && (Guards[n].x+CLOSEWALL)>>TILESHIFT == x ) return 0; // guard in door
			if(Guards[n].tilex==x+1 && Guards[n].tiley==y && (Guards[n].x-CLOSEWALL)>>TILESHIFT == x ) return 0; // guard in door
		}
	}
	else
	{
		if (Player.x>>TILESHIFT == x)
		{
			if ( ( (Player.y+CLOSEWALL) >> TILESHIFT ) == y )	return 0;
			if ( ( (Player.y-CLOSEWALL) >> TILESHIFT ) == y )	return 0;
		}
		for(n=0; n<NumGuards; n++)
		{
			if(Guards[n].tilex==x && Guards[n].tiley==y) return 0; // guard in door
			if(Guards[n].tilex==x && Guards[n].tiley==y-1 && (Guards[n].y+CLOSEWALL)>>TILESHIFT == y ) return 0; // guard in door
			if(Guards[n].tilex==x && Guards[n].tiley==y+1 && (Guards[n].y-CLOSEWALL)>>TILESHIFT == y ) return 0; // guard in door
		}
	}

	return 1;
}

void PlaceItemType(int itemtype, int tilex, int tiley)
{
	Pow_Spawn(tilex, tiley, itemtype);
}

