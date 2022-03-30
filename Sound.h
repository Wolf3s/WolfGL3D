#include "Audiowl6.h"

typedef struct
{
	int length; // in bytes
	//char name[MAX_WLPATH];
	int name;
	byte *data;
} cache_snd;

typedef enum {sfx_none, sfx_pc, sfx_adlib} sfx_t;
typedef enum {digi_none, digi_sb, digi_ss} digi_t;
typedef enum {mus_none, mus_adlib} mus_t;
typedef struct
{
	bool avail[3][3];
	sfx_t sfx;
	digi_t digi;
	mus_t mus;
} snd_mode;
extern snd_mode SD_mode;

void SD_SetSoundLoc(int gx, int gy);
int SD_LoadIdSound(int n, int chn);
int SD_ChooseChannel(soundnames SoundNum);
cache_snd *SD_CacheSND(int name);
void SD_UnCacheSnd(int name);
void SD_UnCacheAllSnds(void);
void SD_InitDigiMap(void);

void SD_PlayImf_f(void);

extern void SD_StopMusic(void);
extern void SD_PlayMusic(int song, int force_restart);
extern void SD_PlaySound(soundnames SoundNum);
extern void SD_PlaySound3D(soundnames SoundNum, int x, int y);
extern void SD_StopAllSound(void);
extern void SD_UpdateSounds3D(void);
extern int SD_Init(void);
extern void SD_Config(int option, int setting);
extern void SD_ShutDown(void);


#define	PlaySoundLocTile(s, tx, ty)	SD_PlaySound3D(s, ((tx)<<TILESHIFT) + HALFTILE, ((ty)<<TILESHIFT) + HALFTILE)
#define	PlaySoundLocActor(s, ob)		SD_PlaySound3D(s, (ob)->x, (ob)->y)