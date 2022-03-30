#include <windows.h>
#include <stdio.h>
#include "WolfDef.h"
#include "actor_ai.h"

/*
==================
= A_DeathScream
=  do a death scream as a 3D sound
=  depending on actor type
==================
*/
void A_DeathScream(Guard_struct *self)
{
	if(!IS_SHAREWARE)
	{// Perdez sound... in secret levels! FIXME!!!
		if(IS_SPEAR && ((gamestate.map==18 || gamestate.map==19) && !US_RndT()) || 
			!IS_SPEAR && (gamestate.map==9 && !US_RndT()) )
		{
			switch(self->type)
			{
				case en_mutant:
				case en_guard:
				case en_officer:
				case en_ss:
				case en_dog:
					PlaySoundLocActor(DEATHSCREAM6SND, self);
					return;
			}
		}
	}

	switch(self->type)
	{
	case en_mutant:
		PlaySoundLocActor(AHHHGSND, self);
		break;
	case en_guard:
	{
		int sounds[9]=
		{
			DEATHSCREAM1SND,
			DEATHSCREAM2SND,
			DEATHSCREAM3SND,
			DEATHSCREAM4SND,
			DEATHSCREAM5SND,
			DEATHSCREAM7SND,
			DEATHSCREAM8SND,
			DEATHSCREAM9SND
		};
		if(IS_SHAREWARE)
			PlaySoundLocActor(sounds[US_RndT()%2], self);
		else
			PlaySoundLocActor(sounds[US_RndT()%8], self);
	}	break;
	case en_officer:
		PlaySoundLocActor(NEINSOVASSND, self);
		break;
	case en_ss:
		PlaySoundLocActor(LEBENSND, self);
		break;
	case en_dog:
		PlaySoundLocActor(DOGDEATHSND, self);
		break;

	case en_boss:
		SD_PlaySound(MUTTISND);
		break;
	case en_schabbs:
		SD_PlaySound(MEINGOTTSND);
		break;
	case en_fake:
		SD_PlaySound(HITLERHASND);
		break;
	case en_mecha:
		SD_PlaySound(SCHEISTSND);
		break;
	case en_hitler:
		SD_PlaySound(EVASND);
		break;
	case en_gretel:
		SD_PlaySound(MEINSND);
		break;
	case en_gift:
		SD_PlaySound(DONNERSND);
		break;
	case en_fat:
		SD_PlaySound(ROSESND);
		break;
	case en_spectre:
		SD_PlaySound(GHOSTFADESND);
		break;
	case en_angel:
		SD_PlaySound(ANGELDEATHSND);
		break;
	case en_trans:
		SD_PlaySound(TRANSDEATHSND);
		break;
	case en_uber:
		SD_PlaySound(UBERDEATHSND);
		break;
	case en_will:
		SD_PlaySound(WILHELMDEATHSND);
		break;
	case en_death:
		SD_PlaySound(KNIGHTDEATHSND);
		break;
	}
}

// Hitler:

// plays Mecha Sound in 3D
void A_MechaSound(Guard_struct *self)
{
	if(areabyplayer[self->areanumber])
		PlaySoundLocActor(MECHSTEPSND, self);
}

// plays Slurpie Sound
void A_Slurpie(Guard_struct *self)
{
	SD_PlaySound(SLURPIESND);
}

/*
===============
= A_HitlerMorph
=  Spawns new actor, where Mecha Hitler is dead
===============
*/
void A_HitlerMorph(Guard_struct *self)
{
	int hitpoints[4]={500, 700, 800, 900};
	Guard_struct *hitler;

	hitler=GetNewActor();
	if(!hitler) return;

	hitler->x=self->x;//
	hitler->y=self->y;//
	hitler->distance=self->distance;
	hitler->tilex=self->tilex;//
	hitler->tiley=self->tiley;//
	hitler->angle=self->angle;//
	hitler->dir=self->dir;//
	hitler->health=hitpoints[gamestate.difficulty];
	hitler->areanumber=self->areanumber;
	hitler->state=st_chase1;//
	hitler->type=en_hitler; //
	hitler->speed=SPDPATROL*5;//
	hitler->ticks=0;//
	hitler->flags=self->flags | FL_SHOOTABLE; //
	hitler->sprite=Spr_GetNewSprite();

	gamestate.killtotal++;
}

/*
=================
= A_Smoke
=  rockets emmit smoke
=================
*/
void A_Smoke(Guard_struct *self)
{
	Guard_struct *smoke;

	smoke=GetNewActor();
	if(!smoke) return;
	smoke->x=self->x;//
	smoke->y=self->y;//
	smoke->tilex=self->tilex;//
	smoke->tiley=self->tiley;//
	smoke->state=st_die1;//
	smoke->type=(self->type==en_hrocket) ? en_hsmoke : en_smoke;
	smoke->ticks=6;//
	smoke->flags=FL_NEVERMARK; //
	smoke->sprite=Spr_GetNewSprite();
}

/*
===============
= A_StartDeathCam
=  Must start Death Cam
===============
*/
void A_StartDeathCam(Guard_struct *self)
{
}