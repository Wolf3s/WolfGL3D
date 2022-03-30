typedef struct
{
	// player saw this tile?
	int vis[64][64];
} AutoMap_t;

extern AutoMap_t AM_AutoMap;

int AM_Init(void);
void AM_ResetAutomap(void);
void AM_DrawAutomap(void);
void AM_Hide_f(void);
void AM_Reveal_f(void);