// Texture Manager
#include <windows.h>
#include <stdio.h>
#include <GL\gl.h>
#include <GL\glu.h>
#include "nVidia GL\glext.h"
#include "WolfDef.h"

#define Vid_CacheTexPair(name) {Vid_CacheTex(name); Vid_CacheTex((name)+1);}
#define Vid_CacheTexPairEx(name) Vid_CacheTexPair((name)<<1)

// ------------------------- * Picture Loding & Caching * -------------------------
cache_tex cachetexs[MAX_CACHED_TEXS+2], notexture;
int numcachetexs=0;

#define MAX_SIZE maxTexSize

void Vid_InitCelFloor(int r1,int r2,int g1,int g2,int b1,int b2)
{
	int i;
	unsigned char floortex[128*128*3], celtex[128*128*3];

	for (i=0; i<(128*128); i++) 
		{ 
		
			celtex[(i*3)]=(char)r1;
			celtex[(i*3)+1]=(char)g1;
			celtex[(i*3)+2]=(char)b1;
			floortex[(i*3)]=(char)r2;
			floortex[(i*3)+1]=(char)g2;
			floortex[(i*3)+2]=(char)b2;
		
		}

		glGenTextures(1, &cachetexs[TEXT_CEL].PICtex);
		glBindTexture(GL_TEXTURE_2D, cachetexs[TEXT_CEL].PICtex);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, 2.f);
		gluBuild2DMipmaps(GL_TEXTURE_2D, 3, 128, 128, GL_RGB, GL_UNSIGNED_BYTE, &celtex);

		glGenTextures(1, &cachetexs[TEXT_FLOOR].PICtex);
		glBindTexture(GL_TEXTURE_2D, cachetexs[TEXT_FLOOR].PICtex);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, 2.f);
		gluBuild2DMipmaps(GL_TEXTURE_2D, 3, 128, 128, GL_RGB, GL_UNSIGNED_BYTE, &floortex);

}
int Vid_InitTexMan(void)
{
	unsigned char notex[16*16*3], *dest;
	int x, y;

	Con_Printf("Initializing TexMan...");

// create a simple checkerboard texture for the default
	for(y=0; y<16; y++)
		for(x=0; x<16; x++)
		{
			dest=&notex[(y*16+x)*3];
			if((y<8)^(x<8))
				{dest[0]=0x00; dest[1]=0x00; dest[2]=0x00;}
			else
				{dest[0]=0x9F; dest[1]=0x5B; dest[2]=0x53;}
		}
	notexture.width=notexture.height=16;
	notexture.alpha=false;
	notexture.name=-1;
// OpenGL
	glGenTextures(1, &notexture.PICtex);
	glBindTexture(GL_TEXTURE_2D, notexture.PICtex);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, 2.f);
	gluBuild2DMipmaps(GL_TEXTURE_2D, notexture.alpha ? 4 : 3, notexture.width, notexture.height, notexture.alpha ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, notex);
//Loading the Detailed Textuires {added by Idot}
	CreateTexture(&WhiteStoneDetails,	"\Textures\\Detailed.pk3\\WhiteStone.jpg");
	CreateTexture(&BlueStoneDetails,	"\Textures\\Detailed.pk3\\BlueStone.jpg");
	CreateTexture(&CementDetails,		"\Textures\\Detailed.pk3\\Cement.jpg");
	CreateTexture(&BrickDetails,		"\Textures\\Detailed.pk3\\Brick.jpg");
	CreateTexture(&RockDetails,			"\Textures\\Detailed.pk3\\Rock.jpg");
	CreateTexture(&MarbleDetails,		"\Textures\\Detailed.pk3\\Marble.jpg");
	CreateTexture(&OrangeStoneDetails,	"\Textures\\Detailed.pk3\\OrangeStone.jpg");
	CreateTexture(&FloorDetails,		"\Textures\\Detailed.pk3\\Floor.jpg");
	CreateTexture(&CeilDetails,			"\Textures\\Detailed.pk3\\Ceil.jpg");
//Loading the Portraits {added by Idot}
	CreateTexture(&Portraits[0],		"\Textures\\Portraits.pk3\\EinFurrer.jpg");
	CreateTexture(&Portraits[1],		"\Textures\\Portraits.pk3\\Spricht.jpg");
	CreateTexture(&Portraits[2],		"\Textures\\Portraits.pk3\\Hitler.jpg");
	CreateTexture(&Portraits[3],		"\Textures\\Portraits.pk3\\Hitler2.jpg");
	CreateTexture(&Portraits[4],		"\Textures\\Portraits.pk3\\Hitler3.jpg");
//Loading the Posters {added by Idot}
	CreateTexture(&Posters[0],			"\Textures\\Posters.pk3\\EsLebe.jpg");
	CreateTexture(&Posters[1],			"\Textures\\Posters.pk3\\Hitler-Jugend.jpg");
	CreateTexture(&Posters[2],			"\Textures\\Posters.pk3\\Ein-Kampf.jpg");
	CreateTexture(&Posters[3],			"\Textures\\Posters.pk3\\Ja.jpg");
	CreateTexture(&Posters[4],			"\Textures\\Posters.pk3\\Ja2.jpg");
	CreateTexture(&Posters[5],			"\Textures\\Posters.pk3\\Mein_Kampf.jpg");
	CreateTexture(&Posters[6],			"\Textures\\Posters.pk3\\pst.jpg");
	CreateTexture(&Posters[7],			"\Textures\\Posters.pk3\\Sieg.jpg");
	CreateTexture(&Posters[8],			"\Textures\\Posters.pk3\\staub.jpg");
	CreateTexture(&Posters[9],			"\Textures\\Posters.pk3\\wille.jpg");
	Con_Printf("done\n");
	return 1;
}

void Vid_ShutDownTexMan(void)
{
	Vid_UnCacheAllTexs();
// remove default texture
	glDeleteTextures(1, &notexture.PICtex);
// remove conback textures
	Vid_UnLoadConBackTexture();
}

cache_tex *Vid_FindCachedTex(int name)
{
	cache_tex *tex;
	int i;

	for(tex=cachetexs, i=0; i<numcachetexs; tex++, i++)
		if(name==tex->name) return tex;
	return NULL;
}

cache_tex *Vid_CacheTex(int name)
{
	unsigned char *data;
	cache_tex *tex;
	int i;

	for(tex=cachetexs, i=0; i<numcachetexs; tex++, i++)
		if(name==tex->name) return tex;

	if(numcachetexs==MAX_CACHED_TEXS)
	{
		Con_Printf("Warning! numcachetexs == MAX_CACHED_TEXS\n");
		return NULL;
	}
	if(!File_LoadTexture(name, &data, tex)) return NULL;

// setup struct for just created picture
	numcachetexs++;
	tex->name=name;

// OpenGL stuff
	glGenTextures(1, &tex->PICtex);
	glBindTexture(GL_TEXTURE_2D, tex->PICtex);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, 2.f);
	gluBuild2DMipmaps(GL_TEXTURE_2D, tex->alpha ? 4 : 3, tex->width, tex->height, tex->alpha ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, data);
	free(data);
	return tex;
}

void Vid_UnCacheTex(int name)
{
	cache_tex *tex;

	tex=Vid_FindCachedTex(name);
	if(!tex) return;

	glDeleteTextures(1, &tex->PICtex);
	memmove(tex, tex+1, (int)&cachetexs[--numcachetexs]-(int)tex);
}

void Vid_UnCacheAllTexs(void)
{
	cache_tex *tex;
	int i;

	for(tex=cachetexs, i=0; i<numcachetexs; tex++, i++)
		glDeleteTextures(1, &tex->PICtex);
	numcachetexs=0;
}

// precaches all needed textures for current level!
void Vid_PrecacheTextures(void)
{
	int n=0;
	int x, y;

	Con_Printf("Precaching...");

	Vid_UnCacheAllTexs();
	for(x=0; x<64; x++)
		for(y=0; y<64; y++)
		{
			if(CurMapData.tile_info[x][y]&TILE_IS_WALL)
				Vid_CacheTexPair(CurMapData.wall_tex[x][y]);
		}
// Any Level needs doors (allmost any)
	Vid_CacheTexPair(TEX_DDOOR);
	Vid_CacheTexPair(TEX_PLATE);
	Vid_CacheTexPair(TEX_DELEV);
	Vid_CacheTexPair(TEX_DLOCK);
	Con_Printf(" done [%d textures]\n", numcachetexs);
}

void Vid_SelectFloor(void)
{
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, cachetexs[TEXT_FLOOR].PICtex);
}

void Vid_SelectCel(void)
{
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, cachetexs[TEXT_CEL].PICtex);
}

void Vid_SelectTexture(int name)
{
	cache_tex *tex;

	tex=Vid_FindCachedTex(name);
	if(!tex) tex=&notexture;
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, tex->PICtex);
}

void Vid_SelectTextureEx(int name)
{
	cache_tex *tex;

	tex=Vid_CacheTex(name);
	if(!tex) tex=&notexture;
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, tex->PICtex);
}