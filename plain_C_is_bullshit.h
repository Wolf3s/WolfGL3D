//#include "WolfDef.h"
//transfered from WolfDef.h by Idot
//typedef	enum {false, true} bool; //it was in the C vesrion



/*
// ------------------------- * Devider * -------------------------
	Constants & Defines for WolfGL Project
*/
//#include <GL\gl.h>
//#include "nVidia GL\glext.h"
//#include <GL\glu.h>
// ID's data types, let they be here!
//typedef	enum {false, true} bool; //it was in the C vesrion
typedef	unsigned char  byte;
typedef	unsigned short word;
typedef	unsigned long  longword;
typedef	byte* Ptr;
// end id' types!
#define XRES			640 // 800 // 640
#define YRES			480 // 600 // 480

// this 2 values describe 2d drawing size
// and are not dependant from resolution/window size
#define SIZE_2D_X 640
#define SIZE_2D_Y 480

extern int tics;
extern int g_XRES;
extern int g_YRES;

//---------------
//
// gamestate structure
//
//---------------
typedef struct
{
	unsigned char difficulty;		// game difficulty
	unsigned char episode, map;	// current map & episode
	unsigned char faceframe;		// bj's face in the HUD
	unsigned char getgatgun;
	unsigned char ouchface;
	int facecount;
	long TimeCount;
	int	secretcount, treasurecount, killcount;	// this level data found by player
	int secrettotal, treasuretotal, killtotal;	// this level 100%
}  gametype;
extern gametype gamestate;

#include "build.h"

#include "cvar.h"
#include "Player.h"
#include "console.h"
#include "OpenGL.h"
#include "video.h"
#include "cmd.h"
#include "sound.h"
#include "window.h"
#include "keys.h"
#include "system.h"
#include "Q_fns.h"
#include "math.h"
#include "TexMan.h"
#include "fileio.h"
#include "level.h"
#include "areas.h"
#include "doors.h"
#include "sprites.h"
#include "Actors.h"
#include "ai_com.h"
#include "client.h"
//#include "server.h"
#include "input.h"
#include "FontMan.h"
#include "Menu.h"
#include "Render.h"
#include "raycast.h"
#include "map.h"
#include "automap.h"
#include "version.h"

#include "vgapics.h"
#include "powerups.h"
#include "Pwalls.h"
#include "wl_text.h"

void MovePlayer(void);
void Attack(void);
void Draw3DScreen(void);
void Draw2DScreen(void);
void NewFrame(void);
void Usekey(int x,int y,int angle);
void ClipMove(int xmove, int ymove);
void frame_speed(void);
char TryMove(void);
void SetUpPlayer(char NewGame);
void LoadRealLevel(unsigned char level);
void CheckUsable(int x, int y, int dir);
char CanCloseDoor(int x, int y, char vert);

void TakeDamage (int points, Guard_struct *attacker);
void GiveWeapon (int weapon);
void GiveAmmo (int ammo);
void GiveExtraMan (void);
void HealSelf (int points);
void GivePoints (long points);
void GiveKey (int key);
void ScanInfoPlane(int level);
void UpdatePaletteShifts (void);
void StartBonusFlash (void);
void ClearPaletteShifts (void);
void StartDamageFlash (int damage);

void SpawnStand (enemy_t which, int tilex, int tiley, int dir);
void SpawnPatrol (enemy_t which, int tilex, int tiley, int dir);
void SpawnDeadGuard(enemy_t which, int x, int y);
void SpawnBoss(enemy_t which, int x, int y);
void SpawnGhosts(enemy_t which, int x, int y);

void ProcessGuards(void);

void T_Projectile(Guard_struct *Guard);
void T_BJRun(Guard_struct *Guard);
void T_BJJump(Guard_struct *Guard);
void T_BJYell(Guard_struct *Guard);
void T_BJDone(Guard_struct *Guard);

void SpawnBJVictory(void);

char SightPlayer (Guard_struct *Guard);
void FirstSighting (Guard_struct *Guard);
char CheckSight (Guard_struct *Guard);
char CheckLine (Guard_struct *Guard);
void KillActor (Guard_struct *Guard);
void DamageActor (Guard_struct *Guard, unsigned damage);
void PlaceItemType(int itemtype, int tilex, int tiley);

void DrawDied(void);

void T_Player(void);
void T_Attack(void);
void KnifeAttack(void);
void GunAttack(void);
void Tick(int DeltaTime);

int CheckBool(char *bool_var, char *ConsoleInput, char *con_str, char *msg_on, char *msg_off);


#define BASEDIR "basewgl\\"

#define UPPERZCOORD	 0.6f
#define	LOWERZCOORD -0.6f

#define WEAPONS_KNIFE		1
#define WEAPONS_PISTOL	2
#define WEAPONS_AUTO		4
#define WEAPONS_CHAIN		8

#define WEAPON_KNIFE	0
#define WEAPON_PISTOL	1
#define WEAPON_AUTO		2
#define WEAPON_CHAIN	3

#define KEY_GOLD			0
#define KEY_SILVER		1

#define KEYS_GOLD			1
#define	KEYS_SILVER		2

#define START_AMMO		8
#define	START_HEALTH	100
#define START_LIVES		3
#define START_SCORE		0
#define START_WEAPON	WEAPON_PISTOL
#define	START_WEAPONS	(WEAPONS_KNIFE | WEAPONS_PISTOL) // | WEAPONS_AUTO | WEAPONS_CHAIN)
#define START_KEYS		0

#define MINACTORDIST 0x10000 // minimum dist from player center to any actor center

#define GRID_SIZE  0x10000	 // Square side (1l<<16)
#define TILEGLOBAL 0x10000
#define HALFTILE   0x8000
#define FLOATTILE  65536.0f
#define TILESHIFT  16				// 1<<TILESHIFT=0x10000 (TILEGLOBAL)
#define WORLDH		 64
#define WORLDW		 64
#define UNITH			4194304    // WORLDH*GRID_SIZE
#define UNITW			4194304    // WORLDW*GRID_SIZE
#define MAXDIST		5931642    // (int)sqrt(UNITH*UNITH+UNITW*UNITW) 

#define HORIZON		240     // YRES/2 
#define VIEWSIZE	412						// where to start Control Panel

#define MINDIST				(0x5800)
#define PLAYERSIZE		MINDIST			// player radius
#define CLOSEWALL			MINDIST //22			// Space between wall & player

#define PLAYERSPEED		3000
#define RUNSPEED   		6000

#define FPS_UPDATE	500	// clock ticks (1000=1second)

#define	TEX_CON		0
#define TEX_FLOOR 1
#define TEX_CEL   2
#define TEX_WALL_BEG	3


#define SECRET_BLOCK 0x62

#define ELEVATOR_SWITCH		21
#define ELEVATOR_PRESSED	22

#define ALTELEVATORTILE		107

#define MAPSHIFT	2
#define MAPXSTART 192
#define MAPYSTART 100

#define SIZEOF_SPRITES 24

#define MAXACTORS			150				// max number of nazis, etc / map
#define MAXSTATS			400				// max number of lamps, bonus, etc
#define MAXDOORS			64				// max number of sliding doors
#define MAXWALLTILES	64				// max number of wall tiles

#define TURNTICS	10
#define SPDPATROL	512
#define SPDDOG		1500

#define FL_SHOOTABLE		1
#define FL_BONUS				2
#define FL_NEVERMARK		4
#define FL_VISABLE			8
#define FL_ATTACKMODE		16
#define FL_FIRSTATTACK	32
#define FL_AMBUSH				64
#define FL_NONMARK			128

#define EXTRAPOINTS		40000	// points for a new player

#define MAX_CACHED_TEXS 1024
#define TEXT_CEL MAX_CACHED_TEXS
#define TEXT_FLOOR MAX_CACHED_TEXS+1
void SetCmds(void);