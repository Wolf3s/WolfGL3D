#define NUM_TEXTURES 32
#include "nVidia GL\glext.h"

bool multitex;
extern bool IsMultiTexEnabled(void);


typedef struct
{
	short width, height; // scaled (ready to draw!)
	unsigned int PICtex; // for OpenGL!
	//char name[MAX_WLPATH];
	int name;
} cache_pic;

typedef unsigned int GLuint;

extern bool multitex;
extern int  GL_Init(void);
extern void GL_ShutDown(void);
extern void GL_Swap(void);

extern void GL_SwitchTo3DState(void);
extern void GL_Restore2DState(void);
extern void GL_SetupProjection(int x, int y, int angle, int vangle);
extern void GL_CheckFOV(void);

extern void Vid_BeginFrame(void);
extern void Vid_EndFrame(void);

extern cache_pic *Vid_CachePic(int name);
extern cache_pic *Vid_CachePicEx(int name);
extern void Vid_UnCachePic(int name);
extern void Vid_UnCacheAllPics(void);
