typedef struct
{
	int x;
	int y;
	int angle;
	int vangle;
	int movspd;
	int strspd;
	int rotspd;
	int rotchg;
	int health;
	int armor;
	int ammo;
	int frags;
	int score;
	int nextextra;
	char lives;
	char weapons;
	char keys;
	char curweapon;
	char chsweapon;
	char weapframe;
	char attackframe;
	int attackcount;
	char model;
	char mdlframe;
	char fire;
	char areanumber;
	char madenoise;
	char attacker;
}  playerstruct;

extern playerstruct Player;
