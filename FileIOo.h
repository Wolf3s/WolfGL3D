// ------------------------- * File I/O * -------------------------
/*
================================
	My own code + some code ripped from wolf's src + some from
	WolfEx by Bruce Lewis
================================
*/
// ------------------------- * Devider * -------------------------
void LoadRealLevel(unsigned char level)
{

	CurrentMap=level;
// ------------------------- * Reseting values * -------------------------
	Door_ResetDoors();
	Ar_ResetAreas();
	AM_ResetAutomap();
	PW_Reset();

// FIXME:
	memset(&gamestate,	0, sizeof(gamestate)); // Reset gamestate
	ResetGuards();
	loadedgame=0;	// reset loaded game flag every time loading new level
	
	gamestate.difficulty=skill;
	gamestate.episode=level/10+1;
	gamestate.map=level%10;
	if(gamestate.map==0)
	{
		gamestate.map=10;
		gamestate.episode--;
	}

	if(!Lvl_LoadLevel(level))
	{
		Con_Printf("Error loading level: %d\n", level);
		return;
	}

	ScanInfoPlane(level);
	Door_SetAreas();

	Player.x=TILE2POS(CurMapData.pSpawn.x);
	Player.y=TILE2POS(CurMapData.pSpawn.y);
	Player.angle=CurMapData.pSpawn.angle;
	Player.areanumber=CurMapData.areas[Player.x>>TILESHIFT][Player.y>>TILESHIFT]>=0 ? CurMapData.areas[Player.x>>TILESHIFT][Player.y>>TILESHIFT] : 36;
	
	Ar_ConnectAreas(Player.areanumber);
	Vid_PrecacheTextures();
	CheckDistanceSquares();
// ! Done Loading Level ! Display Info & Start music!
	Con_Printf("\n\35\36\36\36\36\36\36\36\36\36\36\36\36\36\36\36\36\36\36\36\36\36\36\36\36\36\36\36\36\36\36\36\36\36\36\36\37\n");
	Msg_Printf("%s", CurMapData.name);
	Con_Printf("\n\n");
	SD_PlayMusic(CurMapData.music, 1);
}

void SetUpPlayer(char NewGame)
{
	if(NewGame)
	{
		Player.health=START_HEALTH;
		Player.ammo=START_AMMO;
		Player.weapons=START_WEAPONS;
		Player.curweapon=START_WEAPON;
		Player.chsweapon=START_WEAPON;
		Player.lives=START_LIVES;
		Player.score=START_SCORE;
		Player.nextextra=EXTRAPOINTS;
	}
	Player.keys=START_KEYS;
	Player.movspd=0;
	Player.strspd=0;
	Player.rotspd=0;
	Player.rotchg=0;

	Player.weapframe=0;
	Player.fire=0;

	playstate=ex_stillplaying;
}
