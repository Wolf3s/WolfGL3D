/*
W       W      L    F   GGG  L        333  DDDD
W       W      L   F F G   G L       3   3 D   D
W       W  OO  L   F   G     L           3 D   D
W       W O  O L  FFF  G  GG L        333  D   D
W   W   W O  O L   F   G   G L           3 D   D
 W W W W  O  O L   F   G   G L   L   3   3 D   D
  W   W    OO  LL  F    GGG  LLLLL    333  DDDD
based on NewWolf Code by DarkOne the Hacker. <wolfgl.narod.ru> DarkOne@mail.navigators.lv
modified by Idot
project homepages www.WolfGL3D.h1.ru & www.WolfGL3D.narod.ru
*/
//#pragma comment(lib, "libpng.lib")
#pragma comment(lib, "jpeg.lib")
#pragma comment(lib, "libpng.lib")


#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <io.h>
#include <fcntl.h>
#include <math.h>
#include <time.h>
#include <string.h>
#include <GL\gl.h>
#include <GL\glu.h>
// ------------------------- * DarkOne Includes * -------------------------
#include "WolfDef.h"
#include "tmp.h"
#include "WolfGL.h"
#include "Wl_2Ddrw.h"
#include "FileIOo.h"
#include "Wl_Agent.h"
#include "Wl_Game.h"
#include "Wl_AI.h"
#include "resource.h"
// ------------------------- * <*** Main ***> * -------------------------




int WINAPI WinMain(HINSTANCE hCurInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
//	HWND hWndDlg;
  MSG msg;
//	RECT rect;

// ------------------------- * Splash Screen * -------------------------
	/* hWndDlg=CreateDialog(hCurInstance, MAKEINTRESOURCE(IDD_DIALOG1), NULL, NULL);
	if(hWndDlg)
	{
		if(GetWindowRect(hWndDlg, &rect))
		{
			if(rect.left>(rect.top*2))
			{
				SetWindowPos(hWndDlg, 0,
					(rect.left/2)-((rect.right-rect.left)/2),
					rect.top, 0, 0,
					SWP_NOZORDER | SWP_NOSIZE);
			}
		}
		ShowWindow(hWndDlg, SW_SHOWDEFAULT);
		UpdateWindow(hWndDlg);
		SetForegroundWindow(hWndDlg);
	} */

// ------------------------- * Init * -------------------------
	Win_Init(hCurInstance);
	Cmd_Init();  // only after it all other things, which call add_cmd
	Con_Init(); // must be very first to enable console error printing

	Sys_Init();
	Math_Init(); // after Sys_Init
	Ver_Init();

	Key_Init();
	M_Init(); // menu init

	File_Init(); // before video & sound

	IN_Init();

	if ( (!strcmp(lpCmdLine,"1f")) || (!strcmp(lpCmdLine,"3f")) || (!strcmp(lpCmdLine,"4f")) || (!strcmp(lpCmdLine,"5f")) || (!strcmp(lpCmdLine,"6f")) || (!strcmp(lpCmdLine,"8f")) || (!strcmp(lpCmdLine,"1w")) || (!strcmp(lpCmdLine,"3w")) || (!strcmp(lpCmdLine,"4w")) || (!strcmp(lpCmdLine,"5w")) || (!strcmp(lpCmdLine,"6w")) || (!strcmp(lpCmdLine,"8w")) || (!strcmp(lpCmdLine,"1f32")) || (!strcmp(lpCmdLine,"3f32")) || (!strcmp(lpCmdLine,"4f32")) || (!strcmp(lpCmdLine,"5f32")) || (!strcmp(lpCmdLine,"6f32")) || (!strcmp(lpCmdLine,"8f32")) || (!strcmp(lpCmdLine,"1w32")) || (!strcmp(lpCmdLine,"3w32")) || (!strcmp(lpCmdLine,"4w32")) || (!strcmp(lpCmdLine,"5w32")) || (!strcmp(lpCmdLine,"6w32")) || (!strcmp(lpCmdLine,"8w32"))) {
	if (!strcmp(lpCmdLine,"1f")) {Win_CreateWindow(true, false, 1024, 768, 16);}
	if (!strcmp(lpCmdLine,"3f")) {Win_CreateWindow(true, false, 320, 240, 16);}
	if (!strcmp(lpCmdLine,"4f")) {Win_CreateWindow(true, false, 400, 300, 16);}
	if (!strcmp(lpCmdLine,"5f")) {Win_CreateWindow(true, false, 512, 384, 16);}
	if (!strcmp(lpCmdLine,"6f")) {Win_CreateWindow(true, false, 640, 480, 16);}
	if (!strcmp(lpCmdLine,"8f")) {Win_CreateWindow(true, false, 800, 600, 16);}

	if (!strcmp(lpCmdLine,"1w")) {Win_CreateWindow(false, false, 1024, 768, 16);}
	if (!strcmp(lpCmdLine,"3w")) {Win_CreateWindow(false, false, 320, 240, 16);}
	if (!strcmp(lpCmdLine,"4w")) {Win_CreateWindow(false, false, 400, 300, 16);}
	if (!strcmp(lpCmdLine,"5w")) {Win_CreateWindow(false, false, 512, 384, 16);}
	if (!strcmp(lpCmdLine,"6w")) {Win_CreateWindow(false, false, 640, 480, 16);}
	if (!strcmp(lpCmdLine,"8w")) {Win_CreateWindow(false, false, 800, 600, 16);}

	if (!strcmp(lpCmdLine,"1f32")) {Win_CreateWindow(true, false, 1024, 768, 32);}
	if (!strcmp(lpCmdLine,"3f32")) {Win_CreateWindow(true, false, 320, 240, 32);}
	if (!strcmp(lpCmdLine,"4f32")) {Win_CreateWindow(true, false, 400, 300, 32);}
	if (!strcmp(lpCmdLine,"5f32")) {Win_CreateWindow(true, false, 512, 384, 32);}
	if (!strcmp(lpCmdLine,"6f32")) {Win_CreateWindow(true, false, 640, 480, 32);}
	if (!strcmp(lpCmdLine,"8f32")) {Win_CreateWindow(true, false, 800, 600, 32);}

	if (!strcmp(lpCmdLine,"1w32")) {Win_CreateWindow(false, false, 1024, 768, 32);}
	if (!strcmp(lpCmdLine,"3w32")) {Win_CreateWindow(false, false, 320, 240, 32);}
	if (!strcmp(lpCmdLine,"4w32")) {Win_CreateWindow(false, false, 400, 300, 32);}
	if (!strcmp(lpCmdLine,"5w32")) {Win_CreateWindow(false, false, 512, 384, 32);}
	if (!strcmp(lpCmdLine,"6w32")) {Win_CreateWindow(false, false, 640, 480, 32);}
	if (!strcmp(lpCmdLine,"8w32")) {Win_CreateWindow(false, false, 800, 600, 32);}
	} else { Win_CreateWindow(false, false, 800, 600, 16); }

	GL_Init();
	//CreateTexture(WhiteStoneDetails,"\Textures\\StoneWall.jpg");
	Vid_Init();
	Vid_InitTexMan();
	SD_Init();

	Lvl_Init();
	AM_Init();	//automap
	Pow_Init();	// powerups
	Spr_Init(); // sprites
	
	SetCmds(); // kill it as fast as possible, it is evil!
	CL_Init();

// load defaults. Preferrably before startup of parts, using them[defaults] (resolution-> video)
	Cbuf_AddText("exec newwolf.rc\n");

	Con_Printf("\n========WolfGL Initialized=========\n\n");
		//Con_Printf("\n\nCommandLine: ");
		//Con_Printf(lpCmdLine);
		//Con_Printf("\n");
		//Con_Printf(strcmp(lpCmdLine,"2f")?"aa":"bb");
// ------------------------- * Devider * -------------------------
	FNT_Init();

// ------------------------- * Devider * -------------------------
	Win_Show(true);
	//DestroyWindow(hWndDlg); // Splash
	
	while(GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
		if(quit) DestroyWindow(Win_hWnd);
	}
	return 0;
}

// -------------------------- * User Part * -----------------------------------------------
#define	MAXTICS	10

void Tick(int DeltaTime) // DeltaTime is in milliseconds
{
	static int oldT=0;

//	if(!windowactive) return;
	tics=(DeltaTime+oldT)*70/1000;//14;
	if(!tics)
	{
		oldT=(DeltaTime+oldT);
		return;
	}
	oldT=(DeltaTime+oldT)-tics*1000/70;

	if(tics>MAXTICS) tics=MAXTICS;

	SD_UpdateSounds3D();
	Con_Process();
	Cbuf_Execute();
	if(key_dest==key_game)//action)
	{
		Player.madenoise=0;

		T_Player();

		ProcessGuards();	// if single
		PW_Process();
		Door_ProcessDoors_e(tics, DeltaTime);
		gamestate.TimeCount+=DeltaTime;
	}
}

void NewFrame(void)
{
	static unsigned int oldtime1;
	unsigned int tmp, timediff1;
	static char first=1;

	tmp=timeGetTime();
	if(first){oldtime1=tmp; first=0;}
	timediff1=tmp-oldtime1;
	oldtime1=tmp;
	Tick(timediff1);
	R_UpdateScreen();
}

void CleanUp(void)
{
	static int a=0;
	FILE *fp;

	if(a) return; //execute this only once!
	a=1;

	Con_Printf("Terminating WolfGL...\n");

	fp=fopen(BASEDIR"config.cfg", "wt");
	if(fp)
	{
		fprintf(fp, "// WolfGL Version "VERSION" config file\n");
		Cvar_WriteVariables(fp);
		Key_WriteBindings(fp);
		fclose(fp);
	}
	else
		Con_Printf("Error saving config\n");

	IN_Shutdown();
	SD_ShutDown();
	File_ShutDown();
	FNT_ShutDown();
	Vid_UnCacheAllPics();
	Vid_ShutDownTexMan();
	GL_ShutDown();
	Win_ShutDown();
	PostQuitMessage(0);
}