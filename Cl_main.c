#include <windows.h>
#include <stdio.h>
#include "WolfDef.h"

// these two are not intended to be set directly
cvar_t cl_name={"_cl_name", "player", true};
cvar_t cl_color={"_cl_color", "0", true};

cvar_t lookstrafe={"lookstrafe", "0", true};
cvar_t allwaysrun={"allwaysrun", "0", true};
cvar_t sensitivity={"sensitivity", "15", true};

cvar_t m_yaw={"m_yaw", "1", true};
cvar_t m_forward={"m_forward", "1", true};
cvar_t m_side={"m_side", "900", true};

/*
=================
CL_SendCmd
=================
*/
void CL_SendCmd(void)
{
	extern int in_impulse;
	usercmd_t cmd;
	int dx=0, dy=0, sa;

	// get basic movement from keyboard
	CL_BaseMove(&cmd);
	
	// allow mice or other external controllers to add to the move
	IN_Move(&cmd);

// FIXME! Steb, remove shit! Make real Client - Server
	if(in_use.state&1)
	{	
		Usekey(Player.x, Player.y, Player.angle);
	}

	if(in_impulse)
	{
		switch(in_impulse)
		{
		case 1:
			if(!(Player.weapons&WEAPONS_KNIFE)) goto impulse_no;
			Player.curweapon=Player.chsweapon=WEAPON_KNIFE;
			Player.weapframe=0;
			break;
		case 2:
			if(!(Player.weapons&WEAPONS_PISTOL)) goto impulse_no;
			Player.curweapon=Player.chsweapon=WEAPON_PISTOL;
			Player.weapframe=0;
			break;
		case 3:
			if(!(Player.weapons&WEAPONS_AUTO)) goto impulse_no;
			Player.curweapon=Player.chsweapon=WEAPON_AUTO;
			Player.weapframe=0;
			break;
		case 4:
			if(!(Player.weapons&WEAPONS_CHAIN)) goto impulse_no;
			Player.curweapon=Player.chsweapon=WEAPON_CHAIN;
			Player.weapframe=0;
			break;
		default:
			Msg_Printf("Unknown Impulse: %d", in_impulse);
			break;
		}

goto impulse_ok;
impulse_no:
		Msg_Printf("No weapon");
impulse_ok:
		in_impulse=0;
	}

	Player.angle=NormalizeAngle(Player.angle);
	Player.vangle=NormalizeVAngle(Player.vangle);

	if(cmd.forwardmove)
	{
		dx =(int)(cmd.forwardmove*(tics/70.0)*CosTable[Player.angle]);
		dy =(int)(cmd.forwardmove*(tics/70.0)*SinTable[Player.angle]);
	}
	Player.movspd=cmd.forwardmove>cl_forwardspeed.value ? 1 : 0; // FIXME: for T_Shoot!

	if(cmd.sidemove)
	{
		sa =NormalizeAngle(Player.angle-ANG_90);
		dx+=(int)(cmd.sidemove*(tics/70.0)*CosTable[sa]);
		dy+=(int)(cmd.sidemove*(tics/70.0)*SinTable[sa]);
	}

	if(dx || dy)
	{
		ClipMove(dx, dy);
		Pow_PickUp(Player.x>>TILESHIFT, Player.y>>TILESHIFT);

		// ------------------------- * Checking for area change * -------------------------
		if(CurMapData.areas[Player.x>>TILESHIFT][Player.y>>TILESHIFT]>=0 && CurMapData.areas[Player.x>>TILESHIFT][Player.y>>TILESHIFT]!=Player.areanumber)
		{
			Player.areanumber=CurMapData.areas[Player.x>>TILESHIFT][Player.y>>TILESHIFT];
			Ar_ConnectAreas(Player.areanumber);
		}
		// ------------------------- * End checking * -------------------------
	}
}

void CL_Init(void)
{	

	CL_InitInput();
	
//
// register our commands
//
	Cvar_RegisterVariable(&cl_name);
	Cvar_RegisterVariable(&cl_color);
	Cvar_RegisterVariable(&cl_forwardspeed);
	Cvar_RegisterVariable(&cl_backspeed);
	Cvar_RegisterVariable(&cl_sidespeed);
	Cvar_RegisterVariable(&cl_movespeedkey);
	Cvar_RegisterVariable(&cl_yawspeed);
	Cvar_RegisterVariable(&cl_anglespeedkey);
	Cvar_RegisterVariable(&lookstrafe);
	Cvar_RegisterVariable(&sensitivity);
	Cvar_RegisterVariable(&allwaysrun);

	Cvar_RegisterVariable(&m_yaw);
	Cvar_RegisterVariable(&m_forward);
	Cvar_RegisterVariable(&m_side);

//	Cvar_RegisterVariable (&cl_autofire);
}

