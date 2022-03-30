/*
===============
=
= HealSelf
=
===============
*/
void	HealSelf (int points)
{
	Player.health += points;
	if (Player.health>100)
		Player.health = 100;

	gamestate.getgatgun = 0;	// JR
}

/*
===============
=
= GivePoints
=
===============
*/
void	GivePoints (long points)
{
	Player.score += points;
	while (Player.score >= Player.nextextra)
	{
		Msg_Printf("You have scored for extra life!");
		Player.nextextra += EXTRAPOINTS;
		GiveExtraMan ();
	}
}

/*
===============
=
= GiveAmmo
=
===============
*/
void	GiveAmmo (int ammo)
{
	if (!Player.ammo)				// knife was out
	{
		if (!Player.weapframe)
		{
			Player.curweapon = Player.chsweapon;
		}
	}
	Player.ammo += ammo;
	if (Player.ammo > 99)
		Player.ammo = 99;
}

/*
===============
=
= GiveExtraMan
=
===============
*/
void	GiveExtraMan (void)
{
	if (Player.lives<9)	Player.lives++; // not more than 9 lives!!!
	SD_PlaySound(BONUS1UPSND);
}

/*
==================
=
= GiveWeapon
=
==================
*/
void GiveWeapon (int weapon)
{
	int wps;
	
	wps=1<<weapon;
	
	GiveAmmo (6);
	
	if(Player.weapons & wps)
	{ // have this weapon...
		return;  // recieved all, free :)
	}

	Player.curweapon=Player.chsweapon=weapon;
	Player.weapons|=wps;
}

/*
==================
=
= GiveKey
=
==================
*/
void GiveKey (int key)
{
	Player.keys |= (1<<key);
}


/*
===============
=
= TakeDamage
=
===============
*/
void TakeDamage (int points, Guard_struct *attacker)
{
	LastAttacker = attacker;

	//if (gamestate.victoryflag) return;
	if (gamestate.difficulty==gd_baby) points>>=2;

	if (!god)	Player.health -= points;

	if (Player.health<=0)
	{
		Player.health = 0;
		playstate = ex_died;
		killerobj = attacker;
	}

	StartDamageFlash (points);
	gamestate.getgatgun=0;

	//
	// MAKE BJ'S EYES BUG IF MAJOR DAMAGE!
	// P.S. It's Spear's feature!
	if (points > 30 && Player.health!=0 && !god)
	{
		gamestate.ouchface=1;
		gamestate.facecount = 0;
	}
	
}

/*
=====================
=
= ClearPaletteShifts
=
=====================
*/
void ClearPaletteShifts (void)
{
	bonuscount = damagecount = 0;
}

/*
=====================
=
= StartBonusFlash
=
=====================
*/
void StartBonusFlash (void)
{
	bonuscount = 64;		// white shift palette
}

/*
=====================
=
= StartDamageFlash
=
=====================
*/
void StartDamageFlash (int damage)
{
	damagecount += damage;
	if(damagecount>256) damagecount=256;
}

/*
=====================
=
= UpdatePaletteShifts
=
=====================
*/
void UpdatePaletteShifts (void)
{
	if(bonuscount)
	{
		// OpenGL only, use Palette for soft
		Vid_DrawBox(0, 0, 640, 480, 0xFF, 0xFF, 0xFF, (byte)(bonuscount<256 ? bonuscount : 255));

		bonuscount-=16;
		if(bonuscount<0) bonuscount=0;
	}

	if(damagecount)
	{
		// OpenGL only, use Palette for soft
		Vid_DrawBox(0, 0, 640, 480, 0xFF, 0x00, 0x00, (byte)(damagecount<<3>200 ? 200 : damagecount<<3));

		damagecount-=4;
		if(damagecount<0) damagecount=0;
	}
}

//===========================================================================
/*
===============
=
= Cmd_Fire
=
===============
*/
void Cmd_Fire(void)
{
	//buttonheld[bt_attack] = true;

	Player.weapframe = 0;
	Player.attacker	 = 1;
	Player.attackframe = 0;
	Player.attackcount = attackinfo[Player.curweapon][Player.attackframe].tics;
	Player.weapframe	 = attackinfo[Player.curweapon][Player.attackframe].frame;
}

/*
===============
=
= T_Player
=
===============
*/
void T_Player(void)
{
/*
	if(gamestate.victoryflag)	// watching the BJ actor
	{
		VictorySpin ();
		return;
	}
*/
//	UpdateFace();


	if(!Player.attacker)
	{
/*
		CheckWeaponChange ();
		if(buttonstate[bt_use])	Cmd_Use();
*/
		if(in_attack.state&1) Cmd_Fire();
				//buttonstate[bt_attack] && !buttonheld[bt_attack]) 
		MovePlayer();
/*		if (gamestate.victoryflag)		// watching the BJ actor
			return;*/
	}
	else
	{
		MovePlayer();
		T_Attack();
	}
}

/*
===============
=
= T_Attack
=
===============
*/
void T_Attack(void)
{

// change frame and fire
	Player.attackcount -= tics;
	while(Player.attackcount <= 0)
	{
		switch(attackinfo[Player.curweapon][Player.attackframe].attack)
		{
		case -1:
			Player.attacker=0;
			if(!Player.ammo)
			{
				Player.curweapon = WEAPON_KNIFE;
			}
			else
			{
				if(Player.curweapon != Player.chsweapon)
				{
					Player.curweapon = Player.chsweapon;
				}
			}
			Player.attackframe=Player.weapframe=0;
			return;
		case 4:
			if(!Player.ammo) break;
			if( in_attack.state&1 ) Player.attackframe -= 2;
		case 1:
			if(!Player.ammo)
			{	// can only happen with chain gun
				Player.attackframe++;
				break;
			}
			GunAttack();
			Player.ammo--;
			break;
		case 2:
			KnifeAttack();
			break;
		case 3:
			if (Player.ammo && in_attack.state&1 )
				Player.attackframe -= 2;
			break;
		}
		Player.attackcount += attackinfo[Player.curweapon][Player.attackframe].tics;
		Player.attackframe++;
		Player.weapframe = attackinfo[Player.curweapon][Player.attackframe].frame;
	}
}

/*
===============
=
= T_KnifeAttack
=
= Update player hands, and try to do damage when the proper frame is reached
=
===============
*/
void KnifeAttack(void)
{
	Guard_struct *closest;
	int relangle, angdelta, dist, d1, n;

	SD_PlaySound(ATKKNIFESND);
// actually fire
	dist = 0x7fffffff;
	closest = NULL;

	for(n=0; n<NumGuards; n++)
	{
		if( (Guards[n].flags & FL_SHOOTABLE)/* && (Guards[n].flags & FL_VISABLE)*/ )
		{
			relangle=TransformPoint(Guards[n].x, Guards[n].y);
			angdelta=AngDiff(relangle, Player.angle);
			if(angdelta>SHOOTDELTA)	continue;
			d1=(int)AdvCalcDist(Guards[n].x, Guards[n].y);
			if(d1 < dist)
			{
				dist = d1;
				closest = &Guards[n];
			}
		}
	}

	if(!closest || dist > (TILEGLOBAL+HALFTILE)) return; // missed if futhurer than 1.5 tiles

	// hit something
	DamageActor(closest, US_RndT() >> 4);
}
// ------------------------- * Devider * -------------------------

void	GunAttack(void)
{
	Guard_struct *closest;

	int damage;
	int dx, dy, dist;
	int viewdist, n;
	int relangle, angdelta, d1;

	switch (Player.curweapon)
	{
	case WEAPON_PISTOL:
		SD_PlaySound(ATKPISTOLSND);
		break;
	case WEAPON_AUTO:
		SD_PlaySound(ATKMACHINEGUNSND);
		break;
	case WEAPON_CHAIN:
		SD_PlaySound(ATKGATLINGSND);
		break;
	}
	Player.madenoise=1;

//
// find potential targets
//
	viewdist = 0x7fffffffl;
	closest = NULL;

	for(n=0; n<NumGuards; n++)
	{
		if( (Guards[n].flags & FL_SHOOTABLE)/* && (Guards[n].flags & FL_VISABLE)*/ )
		{
			relangle=TransformPoint(Guards[n].x, Guards[n].y);
			angdelta=AngDiff(relangle, Player.angle);
			if(angdelta>SHOOTDELTA)	continue;
			if(!CheckLine(&Guards[n]))	continue;
			d1=intDistance(Guards[n].x, Guards[n].y);
			if(viewdist>d1)
			{
				viewdist= d1;
				closest = &Guards[n];
			}
		}
	}

	if(!closest) return;
//
// hit something
//
	dx = abs(closest->tilex - (Player.x>>TILESHIFT));
	dy = abs(closest->tiley - (Player.y>>TILESHIFT));
	dist = dx>dy ? dx:dy;

	if (dist<2)
		damage = US_RndT() / 4;
	else if (dist<4)
		damage = US_RndT() / 6;
	else
	{
		if ( (US_RndT() / 12) < dist)		// missed
			return;
		damage = US_RndT() / 6;
	}

	DamageActor(closest,damage);
}
//===========================================================================
