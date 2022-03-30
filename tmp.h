enum
{
	gd_baby,
	gd_easy,
	gd_medium,
	gd_hard
};

unsigned char loadedgame; // is equal to 1 if playing loaded game, or 0 if not

struct atkinf
{
	char tics, attack, frame;	// attack is 1 for gun, 2 for knife
} attackinfo[4][14] = // 4 guns, 14 frames max for every gun!
{
	{	{6,0,1}, {6,2,2},	{6,0,3}, {6,-1,4}	},
	{	{6,0,1}, {6,1,2},	{6,0,3}, {6,-1,4} },
	{	{6,0,1}, {6,1,2},	{6,3,3}, {6,-1,4}	},
	{	{6,0,1}, {6,1,2},	{6,4,3}, {6,-1,4}	}
};

