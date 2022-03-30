unsigned int nOfPoly=0;
clock_t fps_timer=0, fps_timer_old=0;
int frames=0, fps=0;
char virtualreality=0, action=0, pause=0, intermission=0, quit=0, video=1;
char skill=2;
int damagecount, bonuscount;
int tics;
// debug modes:


char noclip=0, god=0, map=0, notarget=0, condump=1;

unsigned char CurrentMap=1;

typedef	enum
{
	ex_stillplaying,
	ex_completed,
	ex_died,
	ex_warped,
	ex_resetgame,
	ex_loadedgame,
	ex_victorious,
	ex_abort,
	ex_demodone,
	ex_secretlevel
} exit_t;
exit_t playstate;

// ------------------------- * End of the Header * -------------------------
void Draw3DScreen(void)
{
	int x, y, angle, vangle;

// initializing
	nOfPoly=0;
	memset(spotvis, 0, sizeof(spotvis));
	memset(floorvis, 0, sizeof(floorvis));
	
	x=Player.x;
	y=Player.y;
	angle=Player.angle;
	vangle=Player.vangle;//added by Idot

	floorvis[x>>TILESHIFT][y>>TILESHIFT]=1;// little hack!
	AM_AutoMap.vis[x>>TILESHIFT][y>>TILESHIFT]=1;
// ------------------------- * 3D drawing * -------------------------
// Draw celing & floor [if use a 2D hack]
	//DrawCelFloor();
// setup fov, fog & 3d resolution
	GL_SwitchTo3DState();
// setup camera
	GL_SetupProjection(x, y, angle, vangle);

// turn texturing on FIXME!!!!!!!
//	glEnable(GL_TEXTURE_2D);

// draw celing & floor [if textured/fogged]
	DrawCelFloor();//moved my Idot because of skeletons
// draw walls
	R_RayCast(x, y, angle, ANG_90);
	glEnable(GL_DEPTH_TEST);
// draw celing & floor [if textured/fogged]
//	DrawCelFloor();
// draw sprites
	DrawSprites();
// ------------------------- * end of 3D drawing * -------------------------
// *** end of 3D drawing ***

	GL_Restore2DState();
	DrawGun(); // FIXME: change this little
}

void DrawDebug(void)
{
//		int X_, Y_;X_=34;Y_=6;//added by Iodt for debugging

	FNT_SetFont(FNT_CONSOLE);
	FNT_SetStyle(0, 0, 0);
	FNT_SetColor(0, 180, 180, 255);
	FNT_SetScale(1, 1);

	FNT_PrintfPos(0, 1, "PlayerPos: X=%4d, Y=%4d, A=%4d.", Player.x, Player.y, FINE2DEG(Player.angle));
	FNT_PrintfPos(0, 2, "(%3dPolygons)", nOfPoly);
	FNT_PrintfPos(33, 2, "FPS:%3d", fps);
	FNT_PrintfPos(0, 3, "Kills:%d/%d Treasure:%d/%d Secrets:%d/%d", gamestate.killcount,			gamestate.killtotal,
																																	gamestate.treasurecount,	gamestate.treasuretotal,
																																	gamestate.secretcount,		gamestate.secrettotal);
	FNT_PrintfPos(0, 4, "%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d",
										areabyplayer[0],
										areabyplayer[1],
										areabyplayer[2],
										areabyplayer[3],
										areabyplayer[4],
										areabyplayer[5],
										areabyplayer[6],
										areabyplayer[7],
										areabyplayer[8],
										areabyplayer[9],
										areabyplayer[10],
										areabyplayer[11],
										areabyplayer[12],
										areabyplayer[13],
										areabyplayer[14],
										areabyplayer[15],
										areabyplayer[16],
										areabyplayer[17],
										areabyplayer[18],
										areabyplayer[19],
										areabyplayer[20],
										areabyplayer[21],
										areabyplayer[22],
										areabyplayer[23],
										areabyplayer[24],
										areabyplayer[25],
										areabyplayer[25],
										areabyplayer[27],
										areabyplayer[28],
										areabyplayer[29],
										areabyplayer[30],
										areabyplayer[31],
										areabyplayer[32],
										areabyplayer[33],
										areabyplayer[34],
										areabyplayer[35],
										areabyplayer[36]);
	FNT_PrintfPos(Player.areanumber, 5, "%d", Player.areanumber);
	FNT_PrintfPos(0, 6, "Time: %02d%c%02d", gamestate.TimeCount/60000, (gamestate.TimeCount/500)%2 ? ':' : ' ', (gamestate.TimeCount/1000)%60);
	FNT_PrintfPos(0, 7, "Player MapPos: X=%4d, Y=%4d, d=%4d", abs(Player.x/FLOATTILE), abs(Player.y/FLOATTILE), Distance_Square_);	
	FNT_PrintfPos(0, 8, "objects=%4d, skiped rays=%4d", n_sprt_, n_skiped_);	
	//if (IsMultiTexEnabled()) FNT_PrintfPos(0, 8, "ARB_Multitex");	
	
}

cvar_t developer={"developer", "0", false};

void Draw2DScreen(void)
{
// ------------------------- * Devider * -------------------------
	if(key_dest==key_menu)
	{ // draw menu!
		M_Draw();
	}
	else
	{
		if(playstate==ex_died)
		{
			DrawDied();//FIXME
		}
		if(map) AM_DrawAutomap();
		DrawDebug();
//		if(developer.value) DrawDebug();
		R_DrawHUD();
	}
	Con_DrawConsole(); // draw console always
	UpdatePaletteShifts();
/*
	Vid_DrawPicture(304-(bonuscount>>2), 224-(bonuscount>>2),
									336+(bonuscount>>2), 256+(bonuscount>>2),
									TEX_VIDEO, 0, 0, 1, 1, 1); // crosshair
*/	
// ------------------------- * Devider * -------------------------
}

void Usekey(int x, int y, int angle)
{

	if(angle==ANG_45 || angle==ANG_135 || angle==ANG_315 || angle==ANG_225) return;

	CheckUsable(POS2TILE(x), POS2TILE(y), Get4dir(angle));
}

char elevok[4]={1, 0, 1, 0}; // can Player use elevator if facing like this

void CheckUsable(int x, int y, int dir)
{
	int dx, dy;

	dx=dx4dir[dir];
	dy=dy4dir[dir];
	x+=dx; y+=dy;

	if(CurMapData.tile_info[x][y]&TILE_IS_DOOR)
	{
		Door_TryUse(DoorMap[x][y], Player.keys);
		return;
	}
	if(CurMapData.tile_info[x][y]&TILE_IS_SECRET)
	{
		PW_Push(x, y, dir);
		return;
	}
	if(CurMapData.tile_info[x][y]&TILE_IS_ELEVATOR)
	{
		if(!elevok[Get4dir(Player.angle)]) return; // don't press elev rails (was a bug in early ver.)
		CurMapData.wall_tex[x][y]=(ELEVATOR_PRESSED-1)*2; // change switch texture
		if(CurMapData.tile_info[x-dx][y-dy]&TILE_IS_SLEVEL)
		{
			Msg_Printf("You have found a secret level!");
			playstate=ex_secretlevel;
		}
		else
		{
			playstate=ex_completed;
		}
		SD_PlaySound(LEVELDONESND);
		Msg_Printf("Level Complete!");
		M_Inter_f();
		return;
	}
	if(CurMapData.tile_info[x][y]&TILE_IS_WALL)
	{
		switch (CurMapData.wall_tex[x][y]){
			case 10://Iron-Eagle
				FloorIronEagle(&x,&y);
				break;
			default:;
		};
		;/////////////////////
	}
	//SD_PlaySound(DONOTHINGSND); FIXME: only after channels implemented
}

void MovePlayer()
{
	CL_SendCmd();
	return;
}

void ClipMove(int xmove, int ymove)
{
	int basex, basey;

	basex=Player.x;
	basey=Player.y;

	Player.x+=xmove;
	Player.y+=ymove;

	if ((basex!=Player.x)||(basey!=Player.y))CheckDistanceSquares();//recheck again!! //added by Idot

	if((noclip || TryMove()) && Player.x>0 && Player.x<UNITW && Player.y>0 && Player.y<UNITH) return; // Ok we moved as we wanted or we can move through walls!

	if(xmove)	// don't bother eating shit if we don't move x!
	{
		Player.x=basex+xmove;
		Player.y=basey;
		if(TryMove()) return; // May be we'll move only X direction?
	}

	if(ymove)	// don't bother eating shit if we don't move y!
	{
		Player.x=basex;
		Player.y=basey+ymove;
		if(TryMove()) return; // May be we'll move only Y direction?
	}

	// No we must stay on one place... :(
	Player.x=basex;
	Player.y=basey;
	Player.movspd=0;
	Player.strspd=0;
}

char TryMove(void)
{
	int xl, yl, xh, yh, x, y, n, deltax, deltay;

	xl=(Player.x-PLAYERSIZE)>>TILESHIFT;
	yl=(Player.y-PLAYERSIZE)>>TILESHIFT;
	xh=(Player.x+PLAYERSIZE)>>TILESHIFT;
	yh=(Player.y+PLAYERSIZE)>>TILESHIFT;

	// Cheching for solid walls:
	for(y=yl; y<=yh; y++)
	{
		for(x=xl; x<=xh; x++)
		{
			if(CurMapData.tile_info[x][y] & TILE_IS_SOLIDTILE) return 0;
			if(CurMapData.tile_info[x][y] & TILE_IS_DOOR)
			{
				if(Door_Opened(x, y)!=DOOR_FULLOPEN) return 0;
			}
		}
	}

	for(n=0; n<NumGuards; n++)
	{
		if(Guards[n].state>=st_die1) continue;
		deltax = Player.x-Guards[n].x;
		if (deltax < -MINACTORDIST || deltax > MINACTORDIST) continue;
		deltay = Player.y-Guards[n].y;
		if (deltay < -MINACTORDIST || deltay > MINACTORDIST) continue;

		return 0;
	}

	return 1;
}		

void Goobers_f(void)
{
	Msg_Printf("You got all ammo & keys!");
	GiveAmmo(99);
	GiveWeapon(WEAPON_AUTO);
	GiveWeapon(WEAPON_CHAIN);
	GiveKey(KEY_GOLD);
	GiveKey(KEY_SILVER);
	HealSelf(100);
}

void Hurt_f(void)
{
	Msg_Printf("You are a sadomasochist!");
	Player.health=1;
	Player.ammo=0;
}

void Map_f(void)
{
	int level;

	if(Cmd_Argc()==1)
	{
		Con_Printf("use: map <levelnumber>\n");
		return;
	}
	level=atoi(Cmd_Argv(1));
	if(level && level<=TotalLevels)
	{
		Con_Printf("Chainging Level...\n");
		SetUpPlayer(1);
		LoadRealLevel((unsigned char)level);
	}
	else
	{
		Con_Printf("Wrong Level\n");
	}
}

void Bool_f(void)
{
	if(CheckBool(&god,			Cmd_Argv(0),	"god",			 "God Mode ON!", "God Mode OFF!")) return;
	if(CheckBool(&noclip,		Cmd_Argv(0),	"noclip",		 "Noclip ON!",	 "Noclip OFF!"  )) return;
	if(CheckBool(&notarget, Cmd_Argv(0),	"notarget",  "Notarget ON!", "Notarget OFF!")) return;
	if(CheckBool(&map,			Cmd_Argv(0),	"togglemap", "Map ON!",			 "Map OFF!"     )) return;
}

void Quit_f(void){quit=1;}

void SetCmds(void)
{
	Cmd_AddCommand("goobers", Goobers_f);
	Cmd_AddCommand("hurt", Hurt_f);
	Cmd_AddCommand("map", Map_f);
	Cmd_AddCommand("god", Bool_f);
	Cmd_AddCommand("noclip", Bool_f);
	Cmd_AddCommand("notarget", Bool_f);
	Cmd_AddCommand("togglemap", Bool_f);
	Cmd_AddCommand("quit", Quit_f);
	Cvar_RegisterVariable(&developer);
}

int CheckBool(char *bool_var, char *ConsoleInput, char *con_str, char *msg_on, char *msg_off)
{
	if(strcmp(ConsoleInput, con_str)==0)
	{
		*bool_var^=1;
		Msg_Printf("%s", *bool_var ? msg_on : msg_off);
		return 1;
	}
	return 0;
}

void frame_speed(void)
{
	if(!fps_timer_old) fps_timer_old=timeGetTime();

	frames++;
  fps_timer=timeGetTime(); //clock();
  if(fps_timer-fps_timer_old>=FPS_UPDATE)  // Updates FPS counter every 500 clocks ticks.
	{
    fps=1000*frames/(fps_timer-fps_timer_old);
    frames=0;
    fps_timer_old=fps_timer;
  }
}
