#include <windows.h>
#include <stdio.h>
#include "WolfDef.h"

void R_DrawFace(void)
{
	int face;

	gamestate.facecount+=tics;
	if(gamestate.facecount>US_RndT())
	{
		gamestate.ouchface=0;
		gamestate.faceframe=(US_RndT()>>6);
		if(gamestate.faceframe==3) gamestate.faceframe=0;
		gamestate.facecount=0;
	}

	if(Player.health)
	{
		if(gamestate.getgatgun)
		{
			face=GOTGATLINGPIC;
			gamestate.getgatgun--;
		}
		else if(gamestate.ouchface)
			face=FACE1APIC;// BJOUCHPIC
		else
		{
/*
			if(god)
				face=FACE1APIC+gamestate.faceframe;// GODMODEFACE1PIC
			else
*/
				face=FACE1APIC+3*((100-Player.health)>>4)+gamestate.faceframe;
		}
	}
	else
	{
		if(LastAttacker->type == en_needle)
			face=MUTANTBJPIC;
		else
		  face=FACE8APIC;
	}

	Vid_DrawPic(272, 394, face);
}

void R_LatchNumber(int x, int y, int width, long number)
{
	int length, c;
	char str[20];

	ltoa(number, str, 10);
	length=strlen(str);

	while(length<width)
	{
		//Vid_DrawPic(x, y, N_BLANKPIC);
		x+=16;
		width--;
	}

	c=length<=width ? 0 : length-width;

	while(c<length)
	{
		Vid_DrawPic(x, y, str[c]-'0'+N_0PIC);
		x+=16;
		c++;
	}
}

void R_DrawHUD(void)
{
	Vid_DrawPic(0, 384, STATUSBARPIC);
	R_DrawFace();
	if(Player.keys & KEYS_GOLD)
		Vid_DrawPic(480, 394, GOLDKEYPIC);
	else
		Vid_DrawPic(480, 394, NOKEYPIC);
	if(Player.keys & KEYS_SILVER)
		Vid_DrawPic(480, 432, SILVERKEYPIC);
	else
		Vid_DrawPic(480, 432, NOKEYPIC);

	Vid_DrawPic(512, 403, KNIFEPIC+Player.curweapon);
	R_LatchNumber( 32, 422, 2, gamestate.map);
	R_LatchNumber( 96, 422, 6, Player.score);
	R_LatchNumber(224, 422, 1, Player.lives);
	R_LatchNumber(336, 422, 3, Player.health);
	R_LatchNumber(432, 422, 2, Player.ammo);
}


// caled every frame
void R_UpdateScreen(void)
{


	frame_speed();

	Vid_BeginFrame();

	if(key_dest!=key_menu)
		Draw3DScreen(); 
	Draw2DScreen();

	Vid_EndFrame();
}