// Wolfenstein Control Panel!  Ta Da!
#include <windows.h>
#include <stdio.h>
#include <GL\gl.h>

#include "WolfDef.h"
//#include "plain_C_is_bullshit.h" //instead WolfDef.h

//#include "gl_draw.h"//added by Idot

enum {m_none, m_main, m_newg, m_episode, m_skill, m_sound, m_controls, 
//enum m_states{m_none, m_main, m_newg, m_episode, m_skill, m_sound, m_controls, //by Idot
			m_load, m_save, m_video, m_scores, m_quit, m_intro, m_inter,
			m_readthis} m_state;

void M_Intro_f(void);
void M_Demo_f(void);
void M_Inter_f(void); 
void M_Menu_Main_f(void);
	void M_Menu_NewGame_f(void);
		void M_Menu_Episode_f(void);
			void M_Menu_Skill_f(void);
	void M_Menu_Sound_f(void);
	void M_Menu_Controls_f(void);
	void M_Menu_Load_f(void);
	void M_Menu_Save_f(void);
	void M_Menu_Video_f(void);
	void M_Menu_ReadThis_f(void); 
	void M_Menu_Scores_f(void);
	void M_Menu_Quit_f(void);

void M_Inter_Draw(void);
void M_Intro_Draw(void);
void M_ReadThis_Draw(void);
void M_Main_Draw(void);
	void M_NewGame_Draw(void);
		void M_Episode_Draw(void);
			void M_Skill_Draw(void);
	void M_Sound_Draw(void);
	void M_Controls_Draw(void);
	void M_Load_Draw(void);
	void M_Save_Draw(void);
	void M_Video_Draw(void);
	void M_Scores_Draw(void);
	void M_Quit_Draw(void);

void M_Inter_Key(int key);
void M_Intro_Key(int key);
void M_ReadThis_Key(int key);
void M_Main_Key(int key);
	void M_NewGame_Key(int key);
		void M_Episode_Key(int key);
			void M_Skill_Key(int key);
	void M_Sound_Key(int key);
	void M_Controls_Key(int key);
	void M_Load_Key(int key);
	void M_Save_Key(int key);
	void M_Video_Key(int key);
	void M_Scores_Key(int key);
	void M_Quit_Key(int key);

//=============================================================================
/*
================
M_ToggleMenu_f
================
*/
void M_ToggleMenu_f(void)
{
	if(key_dest==key_menu)
	{
		if(m_state!=m_main)
		{
			M_Menu_Main_f();
			return;
		}
		key_dest=key_game;
		m_state =m_none;
		return;
	}
	if(key_dest==key_console)
	{
		Con_ToggleConsole_f();
	}
	else
	{
		M_Menu_Main_f();
	}
}

void M_DrawCursor(int x, int y)
{
	static int mode=0, tchg=70; //70&8

	tchg-=tics;
	if(tchg<=0)
	{
		mode=1-mode;
		if(mode==1)
			tchg=8;
		else
			tchg=70;
	}
	Vid_DrawPic(x, y, mode ? C_CURSOR2PIC : C_CURSOR1PIC);
}

//=============================================================================
/* InterMission MENU (SOME SORT OF) */
typedef struct
{
	float time;
	char timestr[6];
} times;
times parTimes[]=
{
	{1.5,	"01:30"},	{2,		"02:00"},	{2,		"02:00"},	{3.5,	"03:30"},	{3,		"03:00"},	{3,		"03:00"},	{2.5,	"02:30"},	{2.5,	"02:30"},	{0,		"??:??"},	{0,		"??:??"},
	{1.5,	"01:30"},	{3.5,	"03:30"},	{3,		"03:00"},	{2,		"02:00"},	{4,		"04:00"},	{6,		"06:00"},	{1,		"01:00"},	{3,		"03:00"},	{0,		"??:??"},	{0,		"??:??"},
	{1.5,	"01:30"},	{1.5,	"01:30"},	{2.5,	"02:30"},	{2.5,	"02:30"},	{3.5,	"03:30"},	{2.5,	"02:30"},	{2,		"02:00"},	{6,		"06:00"},	{0,		"??:??"},	{0,		"??:??"},
	{2,		"02:00"},	{2,		"02:00"},	{1.5,	"01:30"},	{1,		"01:00"},	{4.5,	"04:30"},	{3.5,	"03:30"},	{2,		"02:00"},	{4.5,	"04:30"},	{0,		"??:??"},	{0,		"??:??"},
	{2.5,	"02:30"},	{1.5,	"01:30"},	{2.5,	"02:30"},	{2.5,	"02:30"},	{4,		"04:00"},	{3,		"03:00"},	{4.5,	"04:30"},	{3.5,	"03:30"},	{0,		"??:??"},	{0,		"??:??"},
	{6.5,	"06:30"},	{4,		"04:00"},	{4.5,	"04:30"},	{6,		"06:00"},	{5,		"05:00"},	{5.5,	"05:30"},	{5.5,	"05:30"},	{8.5,	"08:30"},	{0,		"??:??"},	{0,		"??:??"}
};
// ELEVATOR BACK MAPS - REMEMBER (-1)!!
unsigned char ElevatorBackTo[]={1,1,7,3,5,3};
#define PAR_AMOUNT	500
#define PERCENT100AMT	10000

enum {m_in_kills, m_in_secret, m_in_treasure, m_in_wait} m_in_state;
int counter;
int bonus, time_s, time_m, timeleft, kr, sr, tr;

void M_Inter_f(void)
{
	SD_PlayMusic(ENDLEVEL_MUS, 1);
	key_dest=key_menu;
	m_state=m_inter;
	m_in_state=m_in_kills;
	counter=0;
	bonus=0;
	time_s=gamestate.TimeCount/1000;	 // time in seconds
	if(time_s>99*60+59) time_s=99*60+59; //max 99:59
	timeleft=(int)(60*parTimes[(gamestate.episode-1)*10+gamestate.map].time)-time_s;
	time_m=time_s/60; time_s=time_s%60;
	kr=sr=tr=0;
	if(gamestate.killtotal)
		kr=gamestate.killcount*100/gamestate.killtotal;
	if(gamestate.secrettotal)
		sr=gamestate.secretcount*100/gamestate.secrettotal;
	if(gamestate.treasuretotal)
		tr=gamestate.treasurecount*100/gamestate.treasuretotal;
	if(kr==100) bonus+=PERCENT100AMT;
	if(sr==100) bonus+=PERCENT100AMT;
	if(tr==100) bonus+=PERCENT100AMT;
	if(timeleft>0) bonus+=timeleft*PAR_AMOUNT;
	GivePoints(bonus);
}

void M_Write(int x, int y, char *txt)
{
	int alpha[]={L_NUM0PIC,L_NUM1PIC,L_NUM2PIC,L_NUM3PIC,L_NUM4PIC,L_NUM5PIC,
		L_NUM6PIC,L_NUM7PIC,L_NUM8PIC,L_NUM9PIC,L_COLONPIC,0,0,0,0,0,0,L_APIC,L_BPIC,
		L_CPIC,L_DPIC,L_EPIC,L_FPIC,L_GPIC,L_HPIC,L_IPIC,L_JPIC,L_KPIC,
		L_LPIC,L_MPIC,L_NPIC,L_OPIC,L_PPIC,L_QPIC,L_RPIC,L_SPIC,L_TPIC,
		L_UPIC,L_VPIC,L_WPIC,L_XPIC,L_YPIC,L_ZPIC};

	int ox, nx, ny;
	char ch;

	ox=nx=x*16;
	ny=y*19;
	while(ch=*txt++)
	{
		switch(ch)
		{
		case '\n':
			nx=ox; ny+=38;
			continue;
		case '!':
			Vid_DrawPic(nx, ny, L_EXPOINTPIC);
			nx+=16;
			continue;
		case '\'':
			Vid_DrawPic(nx, ny, L_APOSTROPHEPIC);
			nx+=16;
			continue;
		case ' ':
			break;
		case ':':
			Vid_DrawPic(nx, ny, L_COLONPIC);
			nx+=16;
			continue;
		case '%':
			Vid_DrawPic(nx, ny, L_PERCENTPIC);
			break;
		default:
			if(ch>='a')
				ch-=('a'-'A');
			ch-='0';
			if(ch>=0)
				Vid_DrawPic(nx, ny, alpha[ch]);
		}
		nx+=32;
	}
}

// Breathe Mr. BJ!!!
void M_BJ_Breathe(void)
{
	static int which=0, bj_timer=35;
	int pics[2]={L_GUYPIC, L_GUY2PIC};

	bj_timer-=tics;
	if(bj_timer<0)
	{
		which^=1;
		bj_timer=35;
	}
	Vid_DrawPic(0, 38, pics[which]);
}

void M_Inter_Draw(void)
{
	Vid_DrawBox(0, 0, XRES, YRES, RGBA_INTER);
	M_BJ_Breathe();
	M_Write(14,  2, va("floor %d\ncompleted", gamestate.map));
	M_Write(14,  7, va("BONUS %5d", bonus));
	M_Write(16, 10, va("TIME %02d:%02d", time_m, time_s));
	M_Write(18, 12, va("PAR %s", parTimes[(gamestate.episode-1)*10+gamestate.map-1].timestr));
	M_Write( 9, 14, va("KILL RATIO %3d%%", kr));
	M_Write( 5, 16, va("SECRET RATIO %3d%%", sr));
	M_Write( 1, 18, va("TREASURE RATIO %3d%%", tr));
	R_DrawHUD();
}

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
extern exit_t playstate;

void M_Inter_Key(int key)
{
	if(key=K_SPACE)
	{
		if(gamestate.map==10)
			gamestate.map=ElevatorBackTo[gamestate.episode-1]+1;	// back from secret
		else if(playstate==9)
			gamestate.map=10;
		else
			gamestate.map++;
		SetUpPlayer(0);
		LoadRealLevel((unsigned char)((gamestate.episode-1)*10+gamestate.map));
		m_state=m_none;
		key_dest=key_game;
	}
}
//=============================================================================
/* INTRO MENU (SOME SORT OF) */
enum {m_i_pg13, m_i_title, m_i_credits, m_i_scores} m_i_state;
int i_timer;

void M_Intro_f(void)
{
	if(IS_SPEAR)
		SD_PlayMusic(XTOWER2_MUS, 1);
	else
		SD_PlayMusic(NAZI_NOR_MUS, 1);
	key_dest=key_menu;
	m_state =m_intro;
	m_i_state=m_i_pg13;
	i_timer=7*70; // 7 seconds for PG13
}

void M_Demo_f(void)
{
	if(IS_SPEAR)
		SD_PlayMusic(XTOWER2_MUS, 0);
	else
		SD_PlayMusic(NAZI_NOR_MUS, 0);
	key_dest=key_menu;
	m_state =m_intro;
	m_i_state=m_i_title;
	i_timer=15*70;
}

void M_Intro_Draw(void)
{
	i_timer-=tics;
	switch(m_i_state)
	{
	case m_i_pg13:
		Vid_DrawBox(0, 0, XRES, YRES, RGBA_PG13C);
		Vid_DrawPic(432, 264, PG13PIC);
		if(i_timer<0)
		{
			m_i_state=m_i_title;
			i_timer=15*70;
			Vid_UnCachePic(PG13PIC);
		}
		break;
	case m_i_title:
		if(IS_SPEAR)
		{
			Vid_DrawPic(0, 0, TITLE1PIC);
			Vid_DrawPic(0, 192, TITLE2PIC);
		}
		else
			Vid_DrawPic(0, 0, TITLEPIC);
		if(i_timer<0)
		{
			m_i_state=m_i_credits;
			i_timer=10*70;
			if(IS_SPEAR)
			{
				Vid_UnCachePic(TITLE1PIC);
				Vid_UnCachePic(TITLE2PIC);
			}
			else
				Vid_UnCachePic(TITLEPIC);
		}
		break;
	case m_i_credits:
		Vid_DrawPic(0, 0, CREDITSPIC);
		if(i_timer<0)
		{
			m_i_state=m_i_scores;
			i_timer=10*70;
			Vid_UnCachePic(CREDITSPIC);
		}
		break;
	case m_i_scores:
		M_Scores_Draw();
		if(i_timer<0)
		{
			M_Menu_Main_f();
		}
		break;
	}
}

void M_Intro_Key(int key)
{
	if(m_i_state==m_i_pg13)
	{
		M_Demo_f();
		Vid_UnCachePic(PG13PIC);
		return;
	}
	M_Menu_Main_f();
}

//=============================================================================
/* MAIN MENU */
int	m_main_cursor=0;
#define	MAIN_ITEMS 10
char *m_main_items[MAIN_ITEMS]=
{
	"New Game",
	"Sound",
	"Control",
	"Load Game",
	"Save Game",
	"Video Options",
	"Read This!",
	"View Scores",
	"Back to Demo",
	"Quit",
};

void M_Menu_Main_f(void)
{
	//glDisable(GL_LIGHTING) ;
	SD_PlayMusic(WONDERIN_MUS, 0);
	key_dest=key_menu;
	m_state =m_main;

}

void M_Main_Draw(void)
{
	int n;

	Vid_MenuBack();
	Vid_DrawStripes();
	Vid_DrawPic(224, 442, C_MOUSELBACKPIC);
	Vid_DrawPic(160, 0, C_OPTIONSPIC);
	if(IS_SPEAR)
		Vid_DrawMenuBox(136, 125, 356, 295);
	else
		Vid_DrawMenuBox(136, 125, 356, 326);
	FNT_SetFont(FNT_WOLF_BIG);
	FNT_SetStyle(0, 0, 0);
	FNT_SetScale(1, 1);
	FNT_SetColor(RGBA_WLMNU);
	for(n=0; n<MAIN_ITEMS; n++)
	{
		if(m_main_cursor==n) FNT_SetColor(RGBA_WLMSL);
		if(WolfVer.set==WSET_WL1 || n<6)
		{
			FNT_Print(200, 132+n*31, m_main_items[n]);
		}
		else
		{
			if(n==6)
				continue;
			else // if(n>6)
				FNT_Print(200, 132+(n-1)*31, m_main_items[n]);
		}
		if(m_main_cursor==n) FNT_SetColor(RGBA_WLMNU);
	}

	if(WolfVer.set!=WSET_WL1 && m_main_cursor>6)
		M_DrawCursor(144, 127+31*(m_main_cursor-1));
	else
		M_DrawCursor(144, 127+31*m_main_cursor);
}

void M_Main_Key(int key)
{
	switch(key)
	{
	case K_ESCAPE:
		key_dest=key_game;
		m_state =m_none;
		break;
	case K_DOWNARROW:
		if(++m_main_cursor>=MAIN_ITEMS) m_main_cursor=0;
		if(WolfVer.set!=WSET_WL1 && m_main_cursor==6) m_main_cursor=7;
		break;
	case K_UPARROW:
		if(--m_main_cursor<0)	m_main_cursor=MAIN_ITEMS-1;
		if(WolfVer.set!=WSET_WL1 && m_main_cursor==6) m_main_cursor=5;
		break;
	case K_ENTER:
		SD_PlaySound(SHOOTSND);
		switch(m_main_cursor)
		{
		case 0:
			M_Menu_NewGame_f();
			break;
		case 1:
			M_Menu_Sound_f();
			break;
		case 2:
			M_Menu_Controls_f();
			break;
		case 3:
			M_Menu_Load_f();
			break;
		case 4:
			M_Menu_Save_f();
			break;
		case 5:
			M_Menu_Video_f();
			break;
		case 6:
			M_Menu_ReadThis_f();
			break;
		case 7:
			M_Menu_Scores_f();
			break;
		case 8:
			M_Demo_f();
			break;
		case 9:
			M_Menu_Quit_f();
			break;
		}
		break;
	}
}

//=============================================================================
/* KEYS MENU */
char *bindnames[][2] =
{
	{"+attack",			"attack"},
	{"impulse 10",	"change weapon"},
	{"+forward",		"walk forward"},
	{"+back",				"backpedal"},
	{"+left",				"turn left"},
	{"+right",			"turn right"},
	{"+speed",			"run"},
	{"+moveleft",		"step left"},
	{"+moveright",	"step right"},
	{"+strafe",			"sidestep"},
	{"+use",				"use"}
};
#define	NUMCOMMANDS	(sizeof(bindnames)/sizeof(bindnames[0]))

int	keys_cursor=0, bind_grab=0;

void M_Menu_Controls_f(void)
{
	key_dest=key_menu;
	m_state =m_controls;
}

void M_FindKeysForCommand(char *command, int *twokeys)
{
	int	count;
	unsigned int l, n;
	char *b;

	twokeys[0]=twokeys[1]=-1;
	l=strlen(command);
	count=0;

	for(n=0; n<256; n++)
	{
		b=keybindings[n];
		if(!b) continue;
		if(!strncmp(b, command, l))
		{
			if(l==strlen(b))
			{		
				twokeys[count]=n;
				count++;
				if(count==2) break;
			}
		}
	}
}

void M_UnbindCommand(char *command)
{
	unsigned int l,  n;
	char *b;

	l=strlen(command);

	for(n=0; n<256; n++)
	{
		b=keybindings[n];
		if(!b) continue;
		if(!strncmp(b, command, l))
			if(l==strlen(b))
				Key_SetBinding(n, "");
	}
}

void M_Controls_Draw(void)
{
	int	n;
	int	keys[2];
	int	y, h;

	Vid_MenuBack();
	Vid_DrawPic(224, 442, C_MOUSELBACKPIC);
	Vid_DrawStripes();
	Vid_DrawPic(160, 0, C_CUSTOMIZEPIC);
	Vid_DrawMenuBox(60, 150, 520, 285);

	FNT_SetFont(FNT_WOLF_SMALL);
	FNT_SetStyle(0, 0, 0);
	FNT_SetScale(1, 1);
	FNT_SetColor(RGBA_READH);

	if(bind_grab)
		FNT_PrintC(118, 0, 640, "Press a key for this action");
	else
		FNT_PrintC(118, 0, 640, "Enter to change, backspace to clear");

	FNT_SetColor(RGBA_WLMSL);

	h=unpackshort2(FNT_GetSize("A"));

// search for known bindings
	for(n=0; n<NUMCOMMANDS; n++)
	{
		y=115+h*(n+2);

		FNT_Print(75, y, bindnames[n][1]);

		M_FindKeysForCommand(bindnames[n][0], keys);
		FNT_Printf(280, y, "%s %s", keys[0]==-1 ? "???" : Key_KeynumToString(keys[0]),
																keys[1]==-1 ? "" : va("and %s", Key_KeynumToString(keys[1]) ));
	}

	if(bind_grab)
		FNT_Print(263, 115+(keys_cursor+2)*h, "=");
	else
		FNT_Print(263, 115+(keys_cursor+2)*h, ">");
}

void M_Controls_Key(int key)
{
	char *cmd;
	int	keys[2];

	if(bind_grab)
	{	// defining a key
		if(key==K_ESCAPE)
		{
			SD_PlaySound(ESCPRESSEDSND);
			bind_grab=false;
		}
		else if(key!='`')
		{
			SD_PlaySound(SHOOTSND);
			cmd=va("bind \"%s\" \"%s\"\n", Key_KeynumToString(key), bindnames[keys_cursor][0]);
			Cbuf_InsertText(cmd);
		}

		bind_grab=false;
		return;
	}

	switch(key)
	{
	case K_ESCAPE:
		SD_PlaySound(ESCPRESSEDSND);
		M_Menu_Main_f();
		break;
	case K_LEFTARROW:
	case K_UPARROW:
		keys_cursor--;
		if(keys_cursor<0) keys_cursor=NUMCOMMANDS-1;
		break;
	case K_DOWNARROW:
	case K_RIGHTARROW:
		keys_cursor++;
		if(keys_cursor>=NUMCOMMANDS) keys_cursor=0;
		break;
	case K_ENTER:			// go into bind mode
		M_FindKeysForCommand(bindnames[keys_cursor][0], keys);
		if(keys[1]!=-1)
			M_UnbindCommand(bindnames[keys_cursor][0]);
		bind_grab = true;
		break;
	case K_BACKSPACE:	// delete bindings
	case K_DEL:				// delete bindings
		M_UnbindCommand (bindnames[keys_cursor][0]);
		break;
	}
}

//=============================================================================
/* Load/Save MENU */
int	m_load_cursor=0;

#define SAVE_SLOTS 10

char m_filenames[SAVE_SLOTS][33];
int	loadable[SAVE_SLOTS];

void M_ScanSaves(void)
{
	int	n;
	char name[MAX_OSPATH];
	FILE *fp;

// FIXME: add file & version check!

	for(n=0; n<SAVE_SLOTS ; n++)
	{
		strcpy(m_filenames[n], "      - empty -");
		loadable[n]=false;
		sprintf(name, BASEDIR"savegam%d.%s", n, WolfVer.ext);
		fp=fopen(name, "rb");
		if(!fp)	continue;
		if(fread(name, 1, 32, fp)!=32) {fclose(fp); continue;}
		strncpy(m_filenames[n], name, sizeof(m_filenames[n])-1);
		loadable[n] = true;
		fclose(fp);			
	}
}

void M_Menu_Load_f(void)
{
	key_dest=key_menu;
	m_state =m_load;
	M_ScanSaves();
}

void M_Menu_Save_f (void)
{
	// Check: Can we save?

	m_state	=m_save;
	key_dest=key_menu;
	M_ScanSaves();
}

void M_Load_Draw(void)
{
	int n;

	Vid_MenuBack();
	Vid_DrawPic(224, 442, C_MOUSELBACKPIC);
	Vid_DrawMenuBox(150, 120, 350, 336);
	Vid_DrawStripes();
	Vid_DrawPic(120, 0, C_LOADGAMEPIC);

	FNT_SetFont(FNT_WOLF_SMALL);
	FNT_SetStyle(0, 0, 0);
	FNT_SetScale(1, 1);
	FNT_SetColor(RGBA_WLMNU);
	for(n=0; n<SAVE_SLOTS; n++)
	{
		if(m_load_cursor==n)
		{
			Vid_DrawWindow(218, 132+n*31, 272, 26, RGBA_WBKGC, RGBA_WLMSL, RGBA_WLMSL);
			FNT_SetColor(RGBA_WLMSL);
		}
		else
			Vid_DrawWindow(218, 132+n*31, 272, 26, RGBA_WBKGC, RGBA_WLMNU, RGBA_WLMNU);

		FNT_Print(222, 134+n*31, m_filenames[n]);
		if(m_load_cursor==n) FNT_SetColor(RGBA_WLMNU);
	}

	M_DrawCursor(160, 127+31*m_load_cursor);
}

void M_Save_Draw(void)
{
	int n;

	Vid_MenuBack();
	Vid_DrawPic(224, 442, C_MOUSELBACKPIC);
	Vid_DrawMenuBox(150, 120, 350, 336);
	Vid_DrawStripes();
	Vid_DrawPic(120, 0, C_SAVEGAMEPIC);

	FNT_SetFont(FNT_WOLF_SMALL);
	FNT_SetStyle(0, 0, 0);
	FNT_SetScale(1, 1);
	FNT_SetColor(RGBA_WLMNU);
	for(n=0; n<SAVE_SLOTS; n++)
	{
		if(m_load_cursor==n)
		{
			Vid_DrawWindow(218, 132+n*31, 272, 26, RGBA_WBKGC, RGBA_WLMSL, RGBA_WLMSL);
			FNT_SetColor(RGBA_WLMSL);
		}
		else
			Vid_DrawWindow(218, 132+n*31, 272, 26, RGBA_WBKGC, RGBA_WLMNU, RGBA_WLMNU);

		FNT_Print(222, 134+n*31, m_filenames[n]);
		if(m_load_cursor==n) FNT_SetColor(RGBA_WLMNU);
	}

	M_DrawCursor(160, 127+31*m_load_cursor);
}

void M_Load_Key(int key)
{
	switch(key)
	{
	case K_DOWNARROW:
		if(++m_load_cursor>=SAVE_SLOTS) m_load_cursor=0;
		break;
	case K_UPARROW:
		if(--m_load_cursor<0) m_load_cursor=SAVE_SLOTS-1;
		break;
	case K_ESCAPE:
		SD_PlaySound(ESCPRESSEDSND);
		key_dest=key_menu;
		m_state =m_main;
		break;
	}
}

void M_Save_Key(int key)
{
	switch(key)
	{
	case K_DOWNARROW:
		if(++m_load_cursor>=SAVE_SLOTS) m_load_cursor=0;
		break;
	case K_UPARROW:
		if(--m_load_cursor<0) m_load_cursor=SAVE_SLOTS-1;
		break;
	case K_ESCAPE:
		SD_PlaySound(ESCPRESSEDSND);
		key_dest=key_menu;
		m_state =m_main;
		break;
	}
}

//=============================================================================
/* QUIT MENU */
int	msgNumber;
int	m_quit_prevstate;
//m_states m_quit_prevstate;//by Idot
bool wasInMenus;

char *quitMessage[]=
{
	"Dost thou wish to\nleave with such hasty\nabandon?",
	"Chickening out...\nalready?",
	"Press N for more carnage.\nPress Y to be a weenie.",
	"So, you think you can\nquit this easily, huh?",
	"Press N to save the world.\nPress Y to abandon it in\nits hour of need.",
	"Press N if you are brave.\nPress Y to cower in shame.",
	"Heroes, press N.\nWimps, press Y.",
	"You are at an intersection.\nA sign says, 'Press Y to quit.'\n>",
	"For guns and glory, press N.\nFor work and worry, press Y."
};
char *spearQuit[]=
{
	"Heroes don't quit, but\ngo ahead and press Y\nif you aren't one.",
	"Press Y to quit,\nor press N to enjoy\nmore violent diversion.",
	"Depressing the Y key means\nyou must return to the\nhumdrum workday world.",
	"Hey, quit or play,\nY or N:\nit's your choice.",
	"Sure you don't want to\nwaste a few more\nproductive hours?",
	"I think you had better\nplay some more. Please\npress N...please?",
	"If you are tough, press N.\nIf not, press Y daintily.",
	"I'm thinkin' that\nyou might wanna press N\nto play more. You do it.",
	"Sure. Fine. Quit.\nSee if we care.\nGet it over with.\nPress Y."
};

void M_Menu_Quit_f(void)
{
	if(m_state==m_quit)	return;
	wasInMenus=(key_dest==key_menu);
	key_dest=key_menu;
	m_quit_prevstate=m_state;
	m_state=m_quit;
	msgNumber=rand()%9;
}

void Quit_f(void); // FIXME!

void M_Quit_Key(int key)
{
	switch(key)
	{
	case K_ESCAPE:
	case 'n':
	case 'N':
		if(wasInMenus)
		{
			m_state=m_quit_prevstate;
		}
		else
		{
			key_dest=key_game;
			m_state =m_none;
		}
		break;
	case 'Y':
	case 'y':
		key_dest=key_console;
		Quit_f();
		break;
	default:
		break;
	}
}

void M_Quit_Draw(void)
{
	if(wasInMenus)
	{
		m_state=m_quit_prevstate;
		M_Draw();
		m_state=m_quit;
	}
	if(IS_SPEAR)
		Vid_Message(spearQuit[msgNumber]);
	else
		Vid_Message(quitMessage[msgNumber]);
}

//=============================================================================
/* New Game MENU */

void M_Menu_NewGame_f(void)
{
	// check if game in progress!
	// if not go directly to {episode | skill} (also choice is here)
	key_dest=key_menu;
	m_state=m_newg;
	if(IS_SPEAR)
		M_Menu_Skill_f();
	else
		M_Menu_Episode_f();
}

void M_NewGame_Key(int key)
{
}

void M_NewGame_Draw(void)
{
	Vid_Message("You are currently in\na game. Continuing will\nerase old game. Ok?");
}

//=============================================================================
/* Episode MENU */
bool warning_show=false;
int	m_episode_cursor=0;
#define	EPISODES 6
char *m_episodes[EPISODES]=
{
	"Escape from Wolfenstein",
	"Operation: Eisenfaust",
	"Die, Fuhrer, Die!",
  "A Dark Secret",
  "Trail of the Madman",
  "Confrontation"
};

void M_Menu_Episode_f(void)
{
	key_dest=key_menu;
	m_state=m_episode;
}

void M_Episode_Key(int key)
{
	if(warning_show) {warning_show=false; return;}
	switch(key)
	{
	case K_ESCAPE:
		SD_PlaySound(ESCPRESSEDSND);
		M_Menu_Main_f();
		break;
	case K_DOWNARROW:
		if(++m_episode_cursor>=EPISODES) m_episode_cursor=0;
		break;
	case K_UPARROW:
		if(--m_episode_cursor<0)	m_episode_cursor=EPISODES-1;
		break;
	case K_ENTER:
		if( (WolfVer.set==WSET_WL1 && m_episode_cursor>0) || 
				(WolfVer.set==WSET_WL3 && m_episode_cursor>2) )
		{
			SD_PlaySound(NOWAYSND);
			warning_show=true;
		}
		else
		{
			SD_PlaySound(SHOOTSND);
			Cvar_SetValue("episode", (float)m_episode_cursor);
			M_Menu_Skill_f();
		}
		break;
	}
}

void M_Episode_Draw(void)
{
	int n;

	Vid_MenuBack();
	Vid_DrawPic(224, 442, C_MOUSELBACKPIC);
	Vid_DrawMenuBox(12, 46, 616, 388);
	FNT_SetFont(FNT_WOLF_BIG);
	FNT_SetStyle(0, 0, 0);
	FNT_SetScale(1, 1);
	FNT_SetColor(RGBA_READH);
	FNT_PrintC(4, 12, 616, "Which episode to play?");
	FNT_SetColor(RGBA_WLMNU);

	for(n=0; n<EPISODES; n++)
	{
		Vid_DrawPic(84, 55+n*62, C_EPISODE1PIC+n);

		if( (WolfVer.set==WSET_WL1 && n>0) || 
				(WolfVer.set==WSET_WL3 && n>2) )
		{
			if(m_episode_cursor==n)
				FNT_SetColor(RGBA_NOEPH);
			else
				FNT_SetColor(RGBA_NOEPC);
		}
		else
		{
			if(m_episode_cursor==n)
				FNT_SetColor(RGBA_WLMSL);
			else
				FNT_SetColor(RGBA_WLMNU);
		}
		FNT_Printf(196, 55+(n*2)*31, "Episode %d\n%s", n+1, m_episodes[n]);
	}

	M_DrawCursor(16, 50+31*(m_episode_cursor*2));

	if(warning_show)
	{
		Vid_Message("Please select \"Read This!\"\n"
								"from the Options menu to\n"
								"find out how to order this\n"
								"episode from Apogee.");
	}
}

//=============================================================================
/* Skill MENU */
int	m_skill_cursor=2; // default is normal mode
#define	SKILLS 4
char *m_skills[SKILLS]=
{
	"Can I play, Daddy?",
	"Don't hurt me.",
	"Bring 'em on!",
	"I am Death incarnate!"
};

void M_Menu_Skill_f(void)
{
	key_dest=key_menu;
	m_state=m_skill;
}
extern char skill; //FIXME!
void M_Skill_Key(int key)
{
	switch(key)
	{
	case K_ESCAPE:
		SD_PlaySound(ESCPRESSEDSND);
		if(IS_SPEAR)
			M_Menu_Main_f();
		else
			M_Menu_Episode_f();
		break;
	case K_DOWNARROW:
		if(++m_skill_cursor>=SKILLS) m_skill_cursor=0;
		break;
	case K_UPARROW:
		if(--m_skill_cursor<0)	m_skill_cursor=SKILLS-1;
		break;
	case K_ENTER:
		SD_PlaySound(SHOOTSND);
		Cvar_SetValue("skill", (float)m_skill_cursor);
		skill=m_skill_cursor;
		SetUpPlayer(1);
		LoadRealLevel((unsigned char)(m_episode_cursor*10+1));
		key_dest=key_game;
		//start game
		break;
	}
}

void M_Skill_Draw(void)
{
	int n;

	Vid_MenuBack();
	Vid_DrawPic(224, 442, C_MOUSELBACKPIC);
	Vid_DrawMenuBox(90, 216, 450, 161);
	FNT_SetFont(FNT_WOLF_BIG);
	FNT_SetStyle(0, 0, 0);
	FNT_SetScale(1, 1);
	FNT_SetColor(RGBA_READH);
	if(IS_SPEAR)
		Vid_DrawPic(140, 163, C_HOWTOUGHPIC);
	else
		FNT_Print(140, 163, "How tough are you?");
	FNT_SetColor(RGBA_WLMNU);
	Vid_DrawPic(470, 257, m_skill_cursor+C_BABYMODEPIC);
	
	for(n=0; n<SKILLS; n++)
	{
		if(m_skill_cursor==n) FNT_SetColor(RGBA_WLMSL);
		FNT_Print(148, 240+n*31, m_skills[n]);
		if(m_skill_cursor==n) FNT_SetColor(RGBA_WLMNU);
	}

	M_DrawCursor(96, 235+31*m_skill_cursor);
}

//=============================================================================
/* Video Options MENU */
#define VOPT_COUNT 8
int m_vopt_cursor;

void M_Menu_Video_f(void)
{
	key_dest=key_menu;
	m_state=m_video;
}

void M_Video_Draw(void)
{
	Vid_MenuBack();
	Vid_DrawStripes();
	Vid_DrawPic(224, 442, C_MOUSELBACKPIC);
	Vid_DrawMenuBox(90, 120, 450, 161);

	FNT_SetFont(FNT_WOLF_BIG);
	FNT_SetStyle(0, 0, 0);
	FNT_SetScale(1, 1);
	FNT_SetColor(RGBA_WLMNU);
	
	FNT_Printf(148, 125+0*31, "Resolution: %s", "640x480 (fixed)");
	FNT_Printf(148, 125+1*31, "Field of View: %d", 90);
	FNT_Printf(148, 125+2*31, "Fog: %s", 1 ? "on" : "off");
	FNT_Printf(148, 125+3*31, "Fog Mode: %s", "GL_LINEAR");
	FNT_Printf(148, 125+4*31, "FogDensity: %d", 0);
	FNT_Printf(148, 125+5*31, "Fog Color R: %d", 0);
	FNT_Printf(148, 125+6*31, "Fog Color G: %d", 0);
	FNT_Printf(148, 125+7*31, "Fog Color B: %d", 0);
	M_DrawCursor(96, 120+31*m_vopt_cursor);
}

void M_Video_Key(int key)
{
	switch(key)
	{
	case K_ESCAPE:
		SD_PlaySound(ESCPRESSEDSND);
		M_Menu_Main_f();
		break;
	}
}

//=============================================================================
/* Sound Options MENU */
#define SOPT_COUNT 8
int m_sopt_cursor;

typedef struct
{
	int y_pos;
	char *txt;
	int enabeld, on;
} snd_t;

snd_t m_snd_items[SOPT_COUNT]=
{
	{48+0*31, "None", 1, 0},
	{48+1*31, "PC Speaker", 0, 0},
	{48+2*31, "AdLib/Sound Blaster", 1, 1},

	{48+5*31, "None", 1, 0},
	{48+6*31, "Sound Blaster", 1, 1},
	{48+7*31, "Disney Sound Source", 0, 0},

	{48+10*31, "None", 1, 0},
	{48+11*31, "AdLib/Sound Blaster", 1, 1},
};

void M_Sound_Update(void)
{
	int n, m;

	for(n=0; n<3; n++)
		for(m=0; m<(n==2?2:3); m++)
		{
			m_snd_items[n*3+m].enabeld=SD_mode.avail[n][m];
			switch(n)
			{
			case 0:
				m_snd_items[n*3+m].on=SD_mode.sfx==m;
				break;
			case 1:
				m_snd_items[n*3+m].on=SD_mode.digi==m;
				break;
			case 2:
				m_snd_items[n*3+m].on=SD_mode.mus==m;
				break;
			}
		}
}

void M_Menu_Sound_f(void)
{
	M_Sound_Update();

	key_dest=key_menu;
	m_state=m_sound;
}

void M_Sound_Draw(void)
{
	int n;

	Vid_MenuBack();
	Vid_DrawPic(224, 442, C_MOUSELBACKPIC);

	Vid_DrawMenuBox(80,  41, 500, 108);
	Vid_DrawMenuBox(80, 196, 500, 108);
	Vid_DrawMenuBox(80, 352, 500,  77);

	Vid_DrawPic(200,   0, C_FXTITLEPIC);
	Vid_DrawPic(200, 156, C_DIGITITLEPIC);
	Vid_DrawPic(200, 312, C_MUSICTITLEPIC);

	FNT_SetFont(FNT_WOLF_BIG);
	FNT_SetStyle(0, 0, 0);
	FNT_SetScale(1, 1);
	FNT_SetColor(RGBA_WLMNU);
	
	for(n=0; n<SOPT_COUNT; n++)
	{
		if(!m_snd_items[n].enabeld) FNT_SetColor(RGBA_WLMDS);
		else if(n==m_sopt_cursor)   FNT_SetColor(RGBA_WLMSL);
		FNT_Printf (200, m_snd_items[n].y_pos, m_snd_items[n].txt);
		Vid_DrawPic(144, m_snd_items[n].y_pos+4, m_snd_items[n].on ? C_SELECTEDPIC : C_NOTSELECTEDPIC);
		if(!m_snd_items[n].enabeld || n==m_sopt_cursor) FNT_SetColor(RGBA_WLMNU);
	}

	M_DrawCursor(96, m_snd_items[m_sopt_cursor].y_pos-5);
}

void M_Sound_Key(int key)
{
	switch(key)
	{
	case K_ENTER:
	case K_SPACE:
		SD_Config(m_sopt_cursor/3, m_sopt_cursor%3);
		M_Sound_Update();
		break;
	case K_ESCAPE:
		SD_PlaySound(ESCPRESSEDSND);
		M_Menu_Main_f();
		break;
	case K_DOWNARROW:
		do
		{
			if(++m_sopt_cursor>=SOPT_COUNT) m_sopt_cursor=0;
		}
		while(!m_snd_items[m_sopt_cursor].enabeld);
		break;
	case K_UPARROW:
		do
		{
			if(--m_sopt_cursor<0)	m_sopt_cursor=SOPT_COUNT-1;
		}
		while(!m_snd_items[m_sopt_cursor].enabeld);
		break;
	}
}
//=============================================================================
/* High Scores MENU */
#define	SCORES 7
char *m_score[SCORES]=
{
	"id software-'92",
	"Adrian Carmack",
	"John Carmack",
	"Kevin Cloud",
	"Tom Hall",
	"John Romero",
	"Jay Wilbur"
};

void M_Menu_Scores_f(void)
{
	if(IS_SPEAR)
		SD_PlayMusic(XAWARD_MUS, 1);
	else
		SD_PlayMusic(ROSTER_MUS, 1);
	key_dest=key_menu;
	m_state=m_scores;
}

void M_Scores_Key(int key)
{
	M_Menu_Main_f();
}

void M_Scores_Draw(void)
{
	char buffer[16], *str;
	int w;
	int n;

	Vid_MenuBack();
	Vid_DrawStripes();
	if(IS_SPEAR)
	{
		Vid_DrawPic(0, 0, HIGHSCORESPIC);
		FNT_SetFont(FNT_WOLF_BIG);
	}
	else
	{
		Vid_DrawPic(96, 0, HIGHSCORESPIC);
		Vid_DrawPic(64, 163, C_NAMEPIC);
		Vid_DrawPic(320, 163, C_LEVELPIC);
		Vid_DrawPic(448, 163, C_SCOREPIC);
		FNT_SetFont(FNT_WOLF_SMALL);
	}

	FNT_SetStyle(0, 0, 0);
	FNT_SetScale(1, 1);
	FNT_SetColor(RGBA_C15);
	
	for(n=0; n<SCORES; n++)
	{
		if(IS_SPEAR)
		{ // spear
			// name
			FNT_Print(32, 182+38*n, m_score[n]);

			// level
			ultoa(1/*FIXME-Completed Level*/, buffer, 10);
			w=unpackshort1(FNT_GetSize(buffer));
			FNT_Print(352-w, 182+38*n, buffer);
		
			// score
			ultoa(1000/*FIXME-Score*/, buffer, 10);
			w=unpackshort1(FNT_GetSize(buffer));
			FNT_Print(584-w, 182+38*n, buffer);
		}
		else
		{
			// name
			FNT_Print(64, 182+38*n, m_score[n]);
			
			// level
			ultoa(1/*FIXME-Completed Level*/, buffer, 10);
			for(str=buffer; *str; str++)
				*str=*str+(129-'0');	// Used fixed-width numbers (129...)
			w=unpackshort1(FNT_GetSize(buffer));
			FNT_Printf(340-w, 182+38*n, "E%d/L%s", 0/*FIXME-Completed Episode*/+1, buffer);
		
			// score
			ultoa(1000/*FIXME-Score*/, buffer, 10);
			for(str=buffer; *str; str++)
				*str=*str+(129-'0');	// Used fixed-width numbers (129...)
			w=unpackshort1(FNT_GetSize(buffer));
			FNT_Print(528-w, 182+38*n, buffer);
		}
	}
}

//=============================================================================
/* Help Screens */

void M_Menu_Help_f(void)
{
	FILE *fp;
	char *fname;
	unsigned char *databuff;
	int psize;

	if(Cmd_Argc()==1)
	{ // no params! just show 'Read This!'
		M_Menu_ReadThis_f();
		return;
	}
	fname=va(BASEDIR"text\\%s", Cmd_Argv(1));
	psize=File_COM_FileOpenRead(fname, &fp);
	if(psize==-1)
	{
		Con_Printf("File %s not found\n", fname);
		return;
	}
//	databuff=malloc(psize);
	databuff=(unsigned char*)malloc(psize);//by Idot
	fread(databuff, 1, psize, fp);
	fclose(fp);
//	Text_NewArticle(databuff, psize);
	Text_NewArticle((char *)databuff, psize);//by Idot
	free(databuff);

	SD_PlayMusic(CORNER_MUS, 1);
	key_dest=key_menu;
	m_state=m_readthis;
}

void M_Menu_ReadThis_f(void)
{
	long psize;
	unsigned char *databuff;
	int chunk;

	chunk=VGA_REMAP[T_HELPART];

	psize=File_VGA_GetChunkSize(chunk);
	if(psize<=0) return;
//	databuff=malloc(psize);
	databuff=(unsigned char*)malloc(psize);//by Idot
	File_VGA_ReadChunk(chunk, databuff);
//	Text_NewArticle(databuff, psize);
	Text_NewArticle((char *)databuff, psize);//by Idot
	free(databuff);

	SD_PlayMusic(CORNER_MUS, 1);
	key_dest=key_menu;
	m_state=m_readthis;
}

void M_ReadThis_Key(int key)
{
	if(Text_Input(key)) M_Menu_Main_f();
}

void M_ReadThis_Draw(void)
{
	Text_DisplayPage();
}

//=============================================================================
/* Menu Subsystem */
int M_Init(void)
{
	Cmd_AddCommand("togglemenu", M_ToggleMenu_f);

	Cmd_AddCommand("menu_main", M_Menu_Main_f);
	Cmd_AddCommand("menu_newgame", M_Menu_NewGame_f);
	Cmd_AddCommand("menu_load", M_Menu_Load_f);
	Cmd_AddCommand("menu_save", M_Menu_Save_f);
	Cmd_AddCommand("menu_keys", M_Menu_Controls_f);
	Cmd_AddCommand("help", M_Menu_Help_f);
	Cmd_AddCommand("intro", M_Intro_f);
	Cmd_AddCommand("menu_demo", M_Demo_f);
	Cmd_AddCommand("menu_quit", M_Menu_Quit_f);

	M_Intro_f();

	return 1;
}

void M_Draw(void)
{
	if(m_state==m_none || key_dest!=key_menu)	return;

	switch(m_state)
	{
	case m_none:
		break;
	case m_readthis:
		M_ReadThis_Draw();
		return;
	case m_inter:
		M_Inter_Draw();
		break;
	case m_intro:
		M_Intro_Draw();
		break;
	case m_main:
		M_Main_Draw();
		break;
	case m_newg:
		M_NewGame_Draw();
		break;
	case m_episode:
		M_Episode_Draw();
		break;
	case m_skill:
		M_Skill_Draw();
		break;
	case m_controls:
		M_Controls_Draw();
		break;
	case m_load:
		M_Load_Draw();
		break;
	case m_save:
		M_Save_Draw();
		break;
	case m_scores:
		M_Scores_Draw();
		break;
	case m_video:
		M_Video_Draw();
		break;
	case m_sound:
		M_Sound_Draw();
		break;
	case m_quit:
		M_Quit_Draw();
		break;
	}
}

void M_Keydown(int key)
{
	switch(m_state)
	{
	case m_none:
		return;
	case m_readthis:
		M_ReadThis_Key(key);
		return;
	case m_inter:
		M_Inter_Key(key);
		return;
	case m_intro:
		M_Intro_Key(key);
		return;
	case m_main:
		M_Main_Key(key);
		return;
	case m_newg:
		M_NewGame_Key(key);
		return;
	case m_episode:
		M_Episode_Key(key);
		return;
	case m_skill:
		M_Skill_Key(key);
		return;
	case m_controls:
		M_Controls_Key(key);
		return;
	case m_load:
		M_Load_Key(key);
		return;
	case m_save:
		M_Save_Key(key);
		return;
	case m_scores:
		M_Scores_Key(key);
		return;
	case m_video:
		M_Video_Key(key);
		return;
	case m_sound:
		M_Sound_Key(key);
		return;
	case m_quit:
		M_Quit_Key(key);
		return;
	}
}