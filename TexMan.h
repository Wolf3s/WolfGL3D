// this structure holds texture info
typedef struct
{
	short width, height;
	bool alpha;
	unsigned int PICtex; // for OpenGL!
	int name;
} cache_tex;

UINT	WhiteStoneDetails ,BlueStoneDetails, CementDetails,//Detailed Textures {added by Idot}
		BrickDetails, RockDetails, MarbleDetails, OrangeStoneDetails,
		FloorDetails,CeilDetails,
		Portraits[5],Posters[10];
		
int Vid_InitTexMan(void);
void Vid_ShutDownTexMan(void);

cache_tex *Vid_FindCachedTex(int name);
cache_tex *Vid_CacheTex(int name);
void Vid_UnCacheTex(int name);
void Vid_UnCacheAllTexs(void);

void Vid_PrecacheTextures(void);

void Vid_SelectTexture(int name);
void Vid_SelectTextureEx(int name);
void Vid_InitCelFloor(int r1,int r2,int g1,int g2,int b1,int b2);
void Vid_SelectFloor(void);
void Vid_SelectCel(void);


#define TEX_SPRITE_BIT	4096
#define TEX_DOOR_BIT		2048
// texture IDs used by cache routines
#define TEX_DDOOR	(0|TEX_DOOR_BIT)	// simple Door
#define TEX_PLATE	(2|TEX_DOOR_BIT)	// door Plate
#define TEX_DELEV	(4|TEX_DOOR_BIT)	// elevator Door
#define TEX_DLOCK	(6|TEX_DOOR_BIT)	// locked Door