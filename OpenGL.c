// OpenGL low level
#include <Windows.h>
#include <stdio.h>
#include <GL\gl.h>
#include <GL\glaux.h>
//#include "nVidia GL\glext.h"
//#include "tmp.h"// added by me just for gd_hard


//#include "nVidia GL\glh_nveb.h"
//#include "nVidia GL\glh_extensions.h"
//#include "nVidia GL\glh_obs.h"
//#include "nVidia GL\glh_glut.h"
#include <GL\glu.h>
#include "WolfDef.h"

int fogModes[3]={GL_LINEAR, GL_EXP, GL_EXP2};
typedef struct
{
	int Mode;
	float Color[4];
	float Density;		// for Exp & Exp2
	float Start, End;	// for Linear (it suck, IMHO)
} fog_type;
fog_type FogStat;
float cur_x_fov=-1, cur_y_fov; // in degrees

int maxTexSize=256; // maximum texture size 

static GLuint texture[NUM_TEXTURES];

cvar_t vid_fov={"fov", "90"}; // 10-170 degrees
cvar_t vid_fog={"fog", "1", true}; // 0 - off, 1 - on
cvar_t vid_fogmode={"fog_mode", "0", true}; // {0:"GL_LINEAR" | 1:"GL_EXP" | 2:"GL_EXP2"}
//cvar_t vid_fogstart={"fog_start", "0.01", true};	 // 1.0...64.0
//cvar_t vid_fogdens={"fog_density", "0.9", true}; // 0.0....1.0
//cvar_t vid_fogstart={"fog_start", "1.0", true};	 // 1.0...64.0
//cvar_t vid_fogend={"fog_end", "64.0", true};		 // 1.0...64.0
//cvar_t vid_fogend={"fog_end", "64.0", true};		 // 1.0...64.0
//cvar_t vid_fogcr={"fog_r", "1.0", true};				 // 0.0....1.0
//cvar_t vid_fogcg={"fog_g", "0.0", true};				 // 0.0....1.0
//cvar_t vid_fogcb={"fog_b", "0.0", true};				 // 0.0....1.0
//cvar_t vid_fogca={"fog_a", "0.0", true};				 // 0.0....1.0, 1 is untransparen
GLfloat	fogColor[4] = {0.1f,0.0f,0.0f,1.0f};
//red, greed, blue, alpha (0.0....1.0, 1 is untransparen)
/*
cvar_t vid_fogcr={"fog_r", "0.3", true};				 // 0.0....1.0
cvar_t vid_fogcg={"fog_g", "0.3", true};				 // 0.0....1.0
cvar_t vid_fogcb={"fog_b", "0.3", true};				 // 0.0....1.0
cvar_t vid_fogca={"fog_a", "0.3", true};				 // 0.0....1.0
*/

bool oglfCheckExtensionList(char *extension, char *list) {
   char *each_extension;
   char *end_list;
   int i = 0;

   end_list = list + strlen(list);

   while (list < end_list) {
      i = strcspn(list, " ");
      if ((strlen(extension) == i) && (strncmp(extension, list, i) == 0)) {
         return true;
      }
		
      list += (i + 1);
   }

   return false;
}

extern bool IsMultiTexEnabled(void){return /*(bool)multitex*/true;};

void GL_SetDefaults(void);

HDC   hDC;
HGLRC glRC;
static PIXELFORMATDESCRIPTOR pfd=
{
	sizeof(PIXELFORMATDESCRIPTOR),  
	1,                              
	PFD_DRAW_TO_WINDOW              
	| PFD_SUPPORT_OPENGL		    
	| PFD_DOUBLEBUFFER,		        
	PFD_TYPE_RGBA,                  
	16,                             
	0, 0, 0, 0, 0, 0,               
	0,                              
	0,                              
	0,                              
	0, 0, 0, 0,                     
	32,                             
	0,                              
	0,                              
	PFD_MAIN_PLANE,                 
	0,                              
	0, 0, 0                         
};

int GL_Init(void)
{
	int pixelFormat;
	char *glExt, *cp, c;
	char *ext_list;

	Con_Printf("GL_Init: Initializing OpenGL...\n");
	hDC=GetDC(Win_hWnd);
	if(hDC==NULL)
	{
		Con_Printf("Error getting Device Context\n");
		return 0;
	}
	pixelFormat=ChoosePixelFormat(hDC, &pfd);
	if(!pixelFormat)
	{
		Con_Printf("Error choosing Pixel Format\n");
		ReleaseDC(Win_hWnd, hDC);
		return 0;
	}
  if(!SetPixelFormat(hDC, pixelFormat, &pfd))
	{
		Con_Printf("Error setting Pixel Format\n");
		ReleaseDC(Win_hWnd, hDC);
		return 0;
	}
	glRC=wglCreateContext(hDC);
	if(!glRC)
	{
		Con_Printf("Error creating OpenGL Rendering Context\n");
		ReleaseDC(Win_hWnd, hDC);
		return 0;
	}
	if(!wglMakeCurrent(hDC, glRC))
	{
		Con_Printf("Error setting current OpenGL Rendering Context\n");
		wglDeleteContext(glRC);
		ReleaseDC(Win_hWnd, hDC);
		return 0;
	}

	Con_Printf("-> GL_VENDOR:  %s\n", glGetString(GL_VENDOR));
	Con_Printf("-> GL_RENDERER:%s\n", glGetString(GL_RENDERER));
	Con_Printf("-> GL_VERSION: %s\n", glGetString(GL_VERSION));
	Con_Printf("-> GL_EXTENSIONS:\n");
	glExt=(char *)glGetString(GL_EXTENSIONS);
	cp=glExt;
	while(c=*cp++)
	{
		if(c==' ')
		{
			*(cp-1)='\0';
			Con_Printf("-> %s\n", glExt);
			glExt=cp;
		}
	}
	Con_Printf("-> -- End of GL_EXTESIONS --\n");
	Con_Printf("-> GLU_VERSION: %s\n", gluGetString(GLU_VERSION));
	
	glGetIntegerv(GL_MAX_TEXTURE_SIZE, &maxTexSize);
	Con_Printf("-> Maximum texture size: %d\n", maxTexSize);
	if(maxTexSize>512) maxTexSize=512; 

	//chck is GL_ARB_multitexture available
	ext_list = (char*)glGetString(GL_EXTENSIONS);
	multitex=(bool)(oglfCheckExtensionList("GL_ARB_multitexture", ext_list));
	//multitex=true;//added for debugging
	EnableGLExtentions();

	GL_SetDefaults();

	Cvar_RegisterVariable(&vid_fov);
	Cvar_RegisterVariable(&vid_fog);
	Cvar_RegisterVariable(&vid_fogmode);
//	Cvar_RegisterVariable(&vid_fogdens);
//	Cvar_RegisterVariable(&vid_fogstart);
//	Cvar_RegisterVariable(&vid_fogend);
//	Cvar_RegisterVariable(&vid_fogcr);
//	Cvar_RegisterVariable(&vid_fogcg);
//	Cvar_RegisterVariable(&vid_fogcb);
//	Cvar_RegisterVariable(&vid_fogca);
	InitMD3Models();

	Con_Printf("OpenGL Initialized!\n");
	return 1;
}

void GL_ShutDown(void)
{
	Con_Printf("Shutting Down OpenGL...");
	if(!wglMakeCurrent(NULL, NULL))
	{
		Con_Printf("\nRelease RC Failed!\n");
	}
	if(!wglDeleteContext(glRC))
	{
		Con_Printf("\nRelease Of Rendering Context Failed!\n");
	}
	if(!ReleaseDC(Win_hWnd, hDC))
	{
		Con_Printf("\nRelease Of Device Context Failed!\n");
	}
	Con_Printf("done\n");
}

void GL_Swap(void)
{
  SwapBuffers(hDC);
}

//OpenGL Aux

void GL_CheckFOV(void)
{
	if(vid_fov.value!=cur_x_fov)
	{
		if(vid_fov.value<10 || vid_fov.value>170)
		{
			Con_Printf("Wrong FOV: %f", vid_fov.value);
			Cvar_SetValue(vid_fov.name, (cur_x_fov>=10 && cur_x_fov<=170) ? cur_x_fov : 90);
			return;
		}
		cur_x_fov=vid_fov.value;
		cur_y_fov=CalcFov(cur_x_fov, XRES, YRES);
		Con_Printf("Setting FOV: x=%f, y=%f\n", cur_x_fov, cur_y_fov);
	}
}

void GL_CheckFog(void)
{	
enum
{
	gd_baby,
	gd_easy,
	gd_medium,
	gd_hard
};

glClearColor(0.5f,0.5f,0.5f,0.1f);			// We'll Clear To The Color Of The Fog ( Modified )
//glClearColor(0.5f,0.5f,0.8f,1.0f);			// We'll Clear To The Color Of The Fog ( Modified )


// fog mode...
//	if(vid_fogmode.value!=FogStat.Mode)
	{
		if(vid_fogmode.value<0 || vid_fogmode.value>2)
			FogStat.Mode=(FogStat.Mode>=0 && FogStat.Mode<=2) ? FogStat.Mode : 0;
		else
			FogStat.Mode=(int)vid_fogmode.value;
		Cvar_SetValue(vid_fogmode.name, (float)FogStat.Mode);
		glFogi(GL_FOG_MODE, fogModes[FogStat.Mode]);
	}
// fog color
	glFogfv(GL_FOG_COLOR, fogColor);
// fog density 
if	(Player.health<50) glFogf(GL_FOG_DENSITY,  1-sqrt(2*(float)Player.health+1)/100);


// fog start, end 1.0...64.0
		glFogf(GL_FOG_START, 0.1);
		glFogf(GL_FOG_END, 64.0);
//enable only if player's health is low and game is hard
//if	((gamestate.difficulty==gd_hard)&&(Player.health<50)) glEnable(GL_FOG);
//	else 
		glDisable(GL_FOG);
/* original code by DarkOne
// fog mode...
	if(vid_fogmode.value!=FogStat.Mode)
	{
		if(vid_fogmode.value<0 || vid_fogmode.value>2)
			FogStat.Mode=(FogStat.Mode>=0 && FogStat.Mode<=2) ? FogStat.Mode : 0;
		else
			FogStat.Mode=(int)vid_fogmode.value;
		Cvar_SetValue(vid_fogmode.name, (float)FogStat.Mode);
		glFogi(GL_FOG_MODE, fogModes[FogStat.Mode]);
	}
// fog density 
	if(vid_fogdens.value!=FogStat.Density)
		glFogf(GL_FOG_DENSITY, FogStat.Density=vid_fogdens.value);
// fog start, end
	if(vid_fogstart.value!=FogStat.Start)
		glFogf(GL_FOG_START, FogStat.Start=vid_fogstart.value);
	if(vid_fogend.value!=FogStat.End)
		glFogf(GL_FOG_END, FogStat.End=vid_fogend.value);
// fog color
if(vid_fogcr.value!=FogStat.Color[0] || vid_fogcg.value!=FogStat.Color[1] || vid_fogcb.value!=FogStat.Color[2] || vid_fogca.value!=FogStat.Color[3])
	{
		FogStat.Color[0]=vid_fogcr.value;
		FogStat.Color[1]=vid_fogcg.value;
		FogStat.Color[2]=vid_fogcb.value;
		FogStat.Color[3]=vid_fogca.value;
		glFogfv(GL_FOG_COLOR, FogStat.Color);
	}
	if(vid_fog.value)
		glEnable(GL_FOG);
*/
}

void GL_SetDefaults(void)
{
	

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0.0, SIZE_2D_X, SIZE_2D_Y, 0.0); // FIXME: try -1
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glClearColor(0, 0, 0, 0);
	glEnable(GL_BLEND);


	// nVIDIA rules
	//#define GL_FOG_DISTANCE_MODE_NV           0x855A
	//#define GL_EYE_RADIAL_NV                  0x855B
	//#define GL_EYE_PLANE_ABSOLUTE_NV          0x855C
	//glFogi(GL_FOG_DISTANCE_MODE_NV, GL_EYE_RADIAL_NV);
	glFogi(GL_FOG_DISTANCE_MODE_NV, GL_EYE_RADIAL_NV); //by me
	//glh_init_extensions("GL_NV_fog_distance");

	/* funcs init */
	glDepthFunc(GL_LEQUAL); 
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glAlphaFunc(GL_GREATER, 0.5);

	//glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE); // do not let color to change textured poly

	/* hints */
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	glHint(GL_FOG_HINT, GL_NICEST);
//gl_dont_care - Lets opengl choose the kind of fog (per vertex of per pixel) and an unknown formula.
//gl_nicest - Makes the fog per pixel (look good)
//glfastest - Makes the fog per vertex (faster, but less nice) 
	
    

}

void GL_SwitchTo3DState(void)
{
	GLfloat lightAmbient[] = {4.f, 4.f, 4.f, 4.f} ;
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glPushAttrib(GL_ENABLE_BIT | GL_VIEWPORT_BIT | GL_TRANSFORM_BIT);
//	glPushAttrib(GL_ENABLE_BIT | GL_VIEWPORT_BIT | GL_TRANSFORM_BIT | GL_DEPTH_BUFFER_BIT);//by Idot

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);

	GL_CheckFOV();
	GL_CheckFog();

	/* matrixes */
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(cur_y_fov, (float)XRES/(float)YRES, 0.1f, 64.0f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	//glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
	glShadeModel(GL_SMOOTH);
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lightAmbient);
	glEnable(GL_LIGHTING);
}

void GL_Restore2DState(void)
{
	
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
	glPopAttrib();
	glDisable(GL_LIGHTING);
	
}

void GL_SetupProjection(int x, int y, int angle, int vangle)
{
	glRotatef		((float)(90-FINE2DEG(angle)), 0, 1, 0);
	glRotatef		((float)(FINE2DEG(vangle))*SinDgr(90-FINE2DEG(angle)), 0, 0, 1);
	glRotatef		((float)(FINE2DEG(vangle))*CosDgr(90-FINE2DEG(angle)), 1, 0, 0);
	glTranslatef(-x/FLOATTILE, 0, y/FLOATTILE);
}

void Vid_BeginFrame(void)
{
//	static char even=0; // z-trick

/*
	if(even)
	{
		glDepthRange(0.0,0.499999);
		glDepthFunc(GL_LEQUAL);
	}
	else
	{
		glDepthRange(1.0,0.5);
		glDepthFunc(GL_GEQUAL);
	}
	even=!even;
*/
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	glColor4ub(RGBA_WHITE);
}

void Vid_EndFrame(void)
{
//	glFinish();
	glFlush();
	GL_Swap();
}

#define MAX_CACHED_PICS 128
cache_pic cachepics[MAX_CACHED_PICS];
int numcachepics=0;

#define MAX_SIZE maxTexSize

cache_pic *Vid_CachePic(int name)
{
	return Vid_CachePicEx(VGA_REMAP[name]);
}

cache_pic *Vid_CachePicEx(int name)
{
	cache_pic *pic;
	int i;
	wl_pic WlPic;
	int	scaled_width, scaled_height;
	static unsigned char scaled[512*512*3];
	float border[4]={1, 1, 1, 0};

	for(pic=cachepics, i=0; i<numcachepics; pic++, i++)
		if(name==pic->name) return pic;

	if(numcachepics==MAX_CACHED_PICS)
	{
		Con_Printf("Warning! numcachepics == MAX_CACHED_PICS\n");
		return NULL;
	}
	if(!File_VGA_ReadPic(name, &WlPic)) return NULL;

	numcachepics++;
	pic->name=name;
	pic->width =VID_NEW_X(WlPic.width);
	pic->height=VID_NEW_Y(WlPic.height);
	
	for(scaled_width =1; scaled_width <WlPic.width ; scaled_width <<=1);
	for(scaled_height=1; scaled_height<WlPic.height; scaled_height<<=1);

	if(scaled_width >MAX_SIZE) scaled_width =MAX_SIZE;
	if(scaled_height>MAX_SIZE) scaled_height=MAX_SIZE;

	gluScaleImage(GL_RGB, WlPic.width, WlPic.height, GL_UNSIGNED_BYTE, WlPic.data, scaled_width, scaled_height, GL_UNSIGNED_BYTE, scaled);
//ResampleTexture(WlPic.data, WlPic.width, WlPic.height, scaled,  scaled_width, scaled_height, 3);
	free(WlPic.data);

	glGenTextures(1, &pic->PICtex);
	glBindTexture(GL_TEXTURE_2D, pic->PICtex);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);//GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);//GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, 2.f);
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, border);

	glTexImage2D(GL_TEXTURE_2D, 0, 3, scaled_width, scaled_height, 0, GL_RGB, GL_UNSIGNED_BYTE, scaled);
	return pic;
}

void Vid_UnCachePic(int name)
{
	cache_pic *pic;
	int i;

	for(pic=cachepics, i=0; i<numcachepics; pic++, i++)
		if(name==pic->name) break;

	if(name!=pic->name) return; // no pictures to uncache
	
	glDeleteTextures(1, &pic->PICtex);
	memmove(pic, pic+1, (int)&cachepics[--numcachepics]-(int)pic);
}

void Vid_UnCacheAllPics(void)
{
	cache_pic *pic;
	int i;

	for(pic=cachepics, i=0; i<numcachepics; pic++, i++)
		glDeleteTextures(1, &pic->PICtex);
	numcachepics=0;
}
