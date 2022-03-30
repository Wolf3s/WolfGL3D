void DrawDied(void)
{
	if(action) SD_PlaySound(PLAYERDEATHSND);

	action=0;

	if(in_use.state&1)
	{
		action=1;

		Player.weapons=START_WEAPONS;
		Player.ammo=START_AMMO;
		Player.health=START_HEALTH;
		Player.curweapon=START_WEAPON;
		Player.chsweapon=START_WEAPON;
		Player.lives-=1;
		if(Player.lives<0)
		{
			Msg_Printf("Game Over!");
			key_dest=key_menu;
			action=0;
		}
		//Player.score=10000;
		//Player.nextextra=EXTRAPOINTS;
		Player.keys=START_KEYS;

		Player.movspd=0;
		Player.strspd=0;
		Player.rotspd=0;
		Player.rotchg=0;

		Player.weapframe=0;
		Player.fire=0;

		playstate=ex_stillplaying;

		LoadRealLevel((unsigned char)((gamestate.episode-1)*10+gamestate.map));
	}
}