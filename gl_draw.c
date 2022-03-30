// OpenGL
#include <Windows.h>
#include <stdio.h>
/*#include <GL\gl.h>
#include "nVidia GL\glext.h"
#include <GL\glu.h>
*/
//#include "Md3.h"

#include <GL\gl.h>
#include "nVidia GL\glext.h"
#include <GL\glu.h>
#include "WolfDef.h"
//#include "plain_C_is_bullshit.h" //instead WolfDef.h
//#include "Md3.h"
//#include "MD3_Man.h"

int lamps=0;//added by Idot

int WallDetailsDS=26;//Wall Detailzation Distance Square

PFNGLMULTITEXCOORD2FARBPROC glMultiTexCoord2fARB = NULL;
PFNGLACTIVETEXTUREARBPROC glActiveTextureARB = NULL;
PFNGLCLIENTACTIVETEXTUREARBPROC glClientActiveTextureARB = NULL;

extern void EnableGLExtentions(void){
	if (IsMultiTexEnabled){
		glMultiTexCoord2fARB = (PFNGLMULTITEXCOORD2FARBPROC)wglGetProcAddress("glMultiTexCoord2fARB");
		glActiveTextureARB = (PFNGLACTIVETEXTUREARBPROC)wglGetProcAddress("glActiveTextureARB");
		glClientActiveTextureARB = (PFNGLCLIENTACTIVETEXTUREARBPROC)wglGetProcAddress("glClientActiveTextureARB");
		}
}

/*void
	x=Player.x;
	y=Player.y;*/


extern 

void Vid_DrawPicture(int x1, int y1, int x2, int y2, int Tex, 
					 int tx1, int ty1, int tx2, int ty2, int texsize)
{
	float xs, ys, xe, ye;

	xs=tx1/(float)texsize;
	xe=tx2/(float)texsize;
	ys=ty1/(float)texsize;
	ye=ty2/(float)texsize;

	glColor4ub(0xFF, 0xFF, 0xFF, 0xFF);
	//GL_Bind(Tex);
	glBegin(GL_QUADS);
		glTexCoord2f(xe, ys); glVertex2i(x2, y1);
		glTexCoord2f(xs, ys); glVertex2i(x1, y1);
		glTexCoord2f(xs, ye); glVertex2i(x1, y2);
		glTexCoord2f(xe, ye); glVertex2i(x2, y2);
	glEnd();
}


void Vid_DumpScreen(unsigned char *buffer)
{
	glReadPixels(0, 0, XRES, YRES, GL_RGB, GL_UNSIGNED_BYTE, buffer); 
}

// Console Background!
static GLuint conback_tex;
static GLuint ifloortex;
static GLuint iceltex;


void Vid_LoadConBackTexture(void)
{
// maybe some console texturin? heh ;] disabled for some reasons
/*	unsigned char *data;


	data=malloc(128*128*3);

	glGenTextures(1, &conback_tex);
	glBindTexture(GL_TEXTURE_2D, conback_tex);
	File_RAW_Read(BASEDIR"consol32.raw", 0, data, 128, 128, 3);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

	glTexImage2D(GL_TEXTURE_2D, 0, 3, 128, 128, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

	free(data);*/
}

void Vid_UnLoadConBackTexture(void)
{
	glDeleteTextures(1, &conback_tex);
}


void Vid_DrawConBack(int height, unsigned char transp)
{
	float tex_low;

	tex_low=1.0f-height/(float)YRES;
	glBindTexture(GL_TEXTURE_2D, conback_tex);
	glColor4ub(0x44, 0x44, 0x44, transp);

	glBegin(GL_QUADS);
		glTexCoord2f(0.0f, tex_low); glVertex2i(	 0, 0);
		glTexCoord2f(1.0f, tex_low); glVertex2i(XRES, 0);
		glTexCoord2f(1.0f, 1.0f);		 glVertex2i(XRES, height);
		glTexCoord2f(0.0f, 1.0f);		 glVertex2i(   0, height);
	glEnd();
}

// Draws Solid Filled box
void Vid_DrawBox(int x, int y, int w, int h, RGBA_PARAM(1))
{
	glDisable(GL_TEXTURE_2D);
	glBegin(GL_QUADS);
	glColor4ub(RGBA_USEPARAM(1));
		glVertex2i(x+w, y);
		glVertex2i(  x, y);
		glVertex2i(  x, y+h);
		glVertex2i(x+w, y+h);
	glEnd();
}


void Vid_WMenuBack(void)
{
	Vid_DrawBox(0, 0, XRES, YRES, RGBA_BORDC);
}


void Vid_SMenuBack(void)
{
	Vid_DrawPic(0, 0, C_BACKDROPPIC);
}


void Vid_WDrawStripes(void)
{
	Vid_DrawBox(0, 24, XRES, 58, RGBA_BLACK);
	Vid_DrawBox(0, 77, XRES,  2, RGBA_STRPC);
}


void Vid_SDrawStripes(void)
{
	Vid_DrawBox(0, 24, XRES, 53, RGBA_BLACK);
	Vid_DrawBox(0, 79, XRES,  2, RGBA_BLACK);
}

void Vid_MenuBack(void)
{
	if(IS_SPEAR)
		Vid_SMenuBack();
	else
		Vid_WMenuBack();
}

void Vid_DrawStripes(void)
{
	if(IS_SPEAR)
		Vid_SDrawStripes();
	else
		Vid_WDrawStripes();
}


void Vid_DrawWindow(int x, int y, int w, int h, RGBA_PARAM(1), RGBA_PARAM(2), RGBA_PARAM(3))
{

	Vid_DrawBox(x+2, y+2, w-4, h-4, RGBA_USEPARAM(1)); // inner window
	Vid_DrawBox(x, y, w, 2, RGBA_USEPARAM(2)); // upper border
	Vid_DrawBox(x, y, 2, h, RGBA_USEPARAM(2)); // left  border
	Vid_DrawBox(x, y+h, w, -2, RGBA_USEPARAM(3)); // lower border
	Vid_DrawBox(x+w, y, -2, h, RGBA_USEPARAM(3)); // right border
}

void Vid_DrawMenuBox(int x, int y, int w, int h)
{
	if(IS_SPEAR)
		Vid_DrawWindow(x, y, w, h, RGBA_SBKGC, RGBA_SDEAC, RGBA_SBRD2);
	else
		Vid_DrawWindow(x, y, w, h, RGBA_WBKGC, RGBA_DEACT, RGBA_BRD2C);
}


void Vid_Message(char *msg)
{
	int x, y, h, w;
	unsigned long tmp;

	FNT_SetFont(FNT_WOLF_BIG);
	FNT_SetStyle(0, 0, 0);
	FNT_SetColor(RGBA_BLACK);
	FNT_SetScale(1, 1);

	tmp=FNT_GetSize(msg);
	w=unpackshort1(tmp);
	h=unpackshort2(tmp);

	x=(XRES-w)/2;
	y=(YRES-h)/2;

	Vid_DrawWindow(x-10, y-12, w+20, h+24, RGBA_TEXTC, RGBA_HIGHL, RGBA_BLACK);
	FNT_Print(x, y, msg);
}


void Vid_DrawPic(int x, int y, int name)
{
	cache_pic *pic;

	pic=Vid_CachePic(name);
	Vid_DrawPicIndirect(x, y, pic);
}


void Vid_DrawPicEx(int x, int y, int name)
{
	cache_pic *pic;

	pic=Vid_CachePicEx(name);
	Vid_DrawPicIndirect(x, y, pic);
}


void Vid_DrawPicIndirect(int x, int y, cache_pic *pic)
{
	if(pic==NULL) return;

	glColor4ub(0xFF, 0xFF, 0xFF, 0xFF);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, pic->PICtex);

	glBegin(GL_QUADS);
		glTexCoord2f(1, 0); glVertex2i(x+pic->width, y);
		glTexCoord2f(0,	0); glVertex2i(x,						 y);
		glTexCoord2f(0,	1); glVertex2i(x,						 y+pic->height);
		glTexCoord2f(1, 1); glVertex2i(x+pic->width, y+pic->height);
	glEnd();
}

// ------------------------- * Drawing 3D world (OpenGL) * -------------------------
extern unsigned int nOfPoly;


void Draw_Detailed_Wall(float x_begin, float x_end, float y_begin, float y_end,
						int tex, float TexInv, UINT DetailedTex, int DistanceSquare)//added by Idot
{int TexInv_,Tex_Inv;
TexInv_=TexInv-0.0f;
Tex_Inv=1.0f-TexInv;
		Distance_Square_=DistanceSquare;//debug
	if (DistanceSquare<WallDetailsDS){
//	if ((IsMultiTexEnabled)&&(DistanceSquare<WallDetailsDS)){// fix IsMultiTexEnabled!!!!
		glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
		//glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_ADD);
		//glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_BLEND);
		//GL_MODULATE and GL_TEXTURE_ENV_COLOR
		glActiveTextureARB(GL_TEXTURE0_ARB); 
		glEnable(GL_TEXTURE_2D);
		Vid_SelectTexture(tex); 
		glActiveTextureARB(GL_TEXTURE1_ARB); 
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, DetailedTex); 
		glBegin(GL_QUADS);
			glMultiTexCoord2fARB(GL_TEXTURE0_ARB, 0.0f,  0.0f); 
			glMultiTexCoord2fARB(GL_TEXTURE1_ARB, 0.0f, 0.0f); 
				glVertex3f(x_end,	UPPERZCOORD, y_begin);
			glMultiTexCoord2fARB(GL_TEXTURE1_ARB, 8.0f,  0.0f); 
			glMultiTexCoord2fARB(GL_TEXTURE0_ARB, 1.0f, 0.0f); 
				glVertex3f(x_begin, UPPERZCOORD, y_end);
			glMultiTexCoord2fARB(GL_TEXTURE1_ARB, 8.0f,  8.0f); 
			glMultiTexCoord2fARB(GL_TEXTURE0_ARB, 1.0f, 1.0f); 
				glVertex3f(x_begin,	LOWERZCOORD, y_end);
			glMultiTexCoord2fARB(GL_TEXTURE1_ARB, 0.0f,  8.0f); 
			glMultiTexCoord2fARB(GL_TEXTURE0_ARB, 0.0f, 1.0f); 
				glVertex3f(x_end,	LOWERZCOORD, y_begin);
		/*
			glMultiTexCoord2fARB(GL_TEXTURE0_ARB, 0.0f,  0.0f); 
			glMultiTexCoord2fARB(GL_TEXTURE1_ARB, Tex_Inv, 0.0); 
				glVertex3f(x_end,	UPPERZCOORD, y_begin);
			glMultiTexCoord2fARB(GL_TEXTURE1_ARB, 8.0f,  0.0f); 
			glMultiTexCoord2fARB(GL_TEXTURE0_ARB, TexInv_, 0.0); 
				glVertex3f(x_begin, UPPERZCOORD, y_end);
			glMultiTexCoord2fARB(GL_TEXTURE1_ARB, 8.0f,  8.0f); 
			glMultiTexCoord2fARB(GL_TEXTURE0_ARB, TexInv_, 1.0); 
				glVertex3f(x_begin,	LOWERZCOORD, y_end);
			glMultiTexCoord2fARB(GL_TEXTURE1_ARB, 0.0f,  8.0f); 
			glMultiTexCoord2fARB(GL_TEXTURE0_ARB, Tex_Inv, 1.0); 
				glVertex3f(x_end,	LOWERZCOORD, y_begin);*/
		glEnd();				
		(*glActiveTextureARB)(GL_TEXTURE1_ARB);
		glDisable(GL_TEXTURE_2D);
        (*glActiveTextureARB)(GL_TEXTURE0_ARB);
        glDisable(GL_TEXTURE_2D);
		glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	}else{
		Vid_SelectTexture(tex);
		glBegin(GL_QUADS);
			glTexCoord2f(Tex_Inv, 0.0);	glVertex3f(x_end,	UPPERZCOORD, y_begin);
			glTexCoord2f(TexInv_	, 0.0);	glVertex3f(x_begin, UPPERZCOORD, y_end);
			glTexCoord2f(TexInv_	, 1.0);	glVertex3f(x_begin,	LOWERZCOORD, y_end);
			glTexCoord2f(Tex_Inv, 1.0);	glVertex3f(x_end,	LOWERZCOORD, y_begin);
/*
				glTexCoord2f(1.0f-TexInv, 0.0);	glVertex3f(xend,   UPPERZCOORD, y);
				glTexCoord2f(TexInv-0.0f, 0.0);	glVertex3f(xstart, UPPERZCOORD, y);
				glTexCoord2f(TexInv-0.0f, 1.0);	glVertex3f(xstart, LOWERZCOORD, y);
				glTexCoord2f(1.0f-TexInv, 1.0);	glVertex3f(xend,   LOWERZCOORD, y);

				glTexCoord2f(1.0f-TexInv, 0.0);	glVertex3f(x,UPPERZCOORD, ystart);
				glTexCoord2f(TexInv-0.0f, 0.0);	glVertex3f(x, UPPERZCOORD, yend);
				glTexCoord2f(TexInv-0.0f, 1.0);	glVertex3f(x,	LOWERZCOORD, yend);
				glTexCoord2f(1.0f-TexInv, 1.0);	glVertex3f(x,	LOWERZCOORD, ystart);
*/
		glEnd();
		//if (useDetail && distance < DETAIL_DISTANCE)
		if (DistanceSquare<WallDetailsDS){
			glDepthFunc(GL_EQUAL);
			glBlendFunc(GL_DST_COLOR, GL_SRC_COLOR);
			glBindTexture(GL_TEXTURE_2D, WhiteStoneDetails);
			glBegin(GL_QUADS);
				glTexCoord2f(0.0f, 0.0f);	glVertex3f(x_end,	UPPERZCOORD, y_begin);
				glTexCoord2f(8.0f, 0.0f);	glVertex3f(x_begin,	UPPERZCOORD, y_end);
				glTexCoord2f(8.0f, 8.0f);	glVertex3f(x_begin,	LOWERZCOORD, y_end);
				glTexCoord2f(0.0f, 8.0f);	glVertex3f(x_end,	LOWERZCOORD, y_begin);
			glEnd();
			glDepthFunc(GL_LEQUAL); 
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}		}	}


/*   Draws an X wall:
	//               ________
	//              |        |
	//  X-wall ---> |        | <----X-wall
	//              |________|
	//
*/
void Draw_X_Wall(int X, int Y_start, int tex, float TexInv)
{

	float x, yend, ystart, y, z, d;
	int x_,DistanceSquare,lod;

	x			=(float)X;
	yend  =(float)(-1-Y_start);
	ystart=(float)(-Y_start);
	y=(ystart+yend)/2;	
	z=LOWERZCOORD;
	d=2+TexInv;
	nOfPoly++;
//	tex_=tex;
	//+TexInv;
	x_=X-TexInv;

	DistanceSquare=DistanceSquares[x_][Y_start];
	lod=0;
	if (DistanceSquare<20) lod=1;
	if (DistanceSquare<3) lod=2;
	
	switch	(tex){
/*
25 48 purple
46 50 white stone (with yellow moss
86 88 orange stone
94 look at it!!!

SOD 
98 100 brown and grey stones 
106 108 gray stones
110 red stones'
112 gray......(stone??? cement??)
114 116 another kind of  brown and grey stones 
120 look at it!!
122 look at it!!
124 purple brocks

*/
			case 0: case 2:	case 52: //White Stone
				Draw_Detailed_Wall(x, x, ystart, yend, tex+1, TexInv, WhiteStoneDetails, DistanceSquare);
				break;
			case 6: //Posters
				Draw_Detailed_Wall(x, x, ystart, yend, 0+1, TexInv, WhiteStoneDetails, DistanceSquare);
				glEnable(GL_TEXTURE_2D);
				glBindTexture(GL_TEXTURE_2D, Posters[CheckMapModificator(&x_, &Y_start)]);
				glBegin(GL_QUADS);
					glTexCoord2f(1.0f-TexInv, 0.0);	glVertex3f(x-(1-2*TexInv)*0.01, UPPERZCOORD-0.1, ystart-0.1);
					glTexCoord2f(TexInv-0.0f, 0.0);	glVertex3f(x-(1-2*TexInv)*0.01, UPPERZCOORD-0.1, yend+0.1);
					glTexCoord2f(TexInv-0.0f, 1.0);	glVertex3f(x-(1-2*TexInv)*0.01, LOWERZCOORD+0.1, yend+0.1);
					glTexCoord2f(1.0f-TexInv, 1.0);	glVertex3f(x-(1-2*TexInv)*0.01, LOWERZCOORD+0.1, ystart-0.1);
				glEnd();
				glDisable(GL_TEXTURE_2D);
				break;
			case 14: case 16: case 66: case 70: case 78: //BlueStone
				Draw_Detailed_Wall(x, x, ystart, yend, tex+1, TexInv, BlueStoneDetails, DistanceSquare);
				break;
			case 20: //Portraits
				Vid_SelectTexture(22+1);
				glBegin(GL_QUADS);
					glTexCoord2f(1.0f-TexInv, 0.0);	glVertex3f(x, UPPERZCOORD, ystart);
					glTexCoord2f(TexInv-0.0f, 0.0);	glVertex3f(x, UPPERZCOORD, yend);
					glTexCoord2f(TexInv-0.0f, 1.0);	glVertex3f(x,	LOWERZCOORD, yend);
					glTexCoord2f(1.0f-TexInv, 1.0);	glVertex3f(x,	LOWERZCOORD, ystart);
				glEnd();
				//if (useDetail && distance < DETAIL_DISTANCE)
				// DrawIronEagle(&x, &y, &x_, &Y_start, &z, &d);
				{
					
					glBindTexture(GL_TEXTURE_2D, Portraits[CheckMapModificator(&x_, &Y_start)]);
					glBegin(GL_QUADS);
						glTexCoord2f(1.0f-TexInv, 0.0);	glVertex3f(x-(1-2*TexInv)*0.01, UPPERZCOORD-0.1, ystart-0.15);
						glTexCoord2f(TexInv-0.0f, 0.0);	glVertex3f(x-(1-2*TexInv)*0.01, UPPERZCOORD-0.1, yend+0.15);
						glTexCoord2f(TexInv-0.0f, 1.0);	glVertex3f(x-(1-2*TexInv)*0.01, LOWERZCOORD+0.1, yend+0.15);
						glTexCoord2f(1.0f-TexInv, 1.0);	glVertex3f(x-(1-2*TexInv)*0.01, LOWERZCOORD+0.1, ystart-0.15);
					glEnd();
					}
				break;
			case 32: case 74: //Bricks
				Draw_Detailed_Wall(x, x, ystart, yend, tex+1, TexInv, BrickDetails, DistanceSquare);
				break;
			case 56: case 58: case 60: case 62: //Rocks
				Draw_Detailed_Wall(x, x, ystart, yend, tex+1, TexInv, RockDetails, DistanceSquare);
				break;
			case 68: case 72: case 76: //Cement blocks
				Draw_Detailed_Wall(x, x, ystart, yend, tex+1, TexInv, CementDetails, DistanceSquare);
				break;
			case 82: case 90: //Orange Marble
				glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,128);//max 128
				Draw_Detailed_Wall(x, x, ystart, yend, tex+1, TexInv, MarbleDetails, DistanceSquare);
				//glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,0);
				break;
			case 86: case 88: //Orange Stone
				Draw_Detailed_Wall(x, x, ystart, yend, tex+1, TexInv, OrangeStoneDetails, DistanceSquare);
				Vid_SelectTexture(tex+1);
				break;
			case 8://Cell
				DrawCell(&x, &y, &x_, &Y_start, &z, &d,&lod);
				floorvis[x_][Y_start]=1;
				break;
			case 10://Iron-Eagle Staue (Dark)
				DrawIronEagle(&x, &y, &x_, &Y_start, &z, &d,&lod);
				floorvis[x_][Y_start]=1;
				break;
			case 12://Celled Skeleton
				DrawCelledSkeleton(&x, &y, &x_, &Y_start, &z, &d,&lod);
				floorvis[x_][Y_start]=1;
				break;
			default:
			Vid_SelectTexture(tex+1);
			glBegin(GL_QUADS);
				glTexCoord2f(1.0f-TexInv, 0.0);	glVertex3f(x, UPPERZCOORD, ystart);
				glTexCoord2f(TexInv-0.0f, 0.0);	glVertex3f(x, UPPERZCOORD, yend);
				glTexCoord2f(TexInv-0.0f, 1.0);	glVertex3f(x,	LOWERZCOORD, yend);
				glTexCoord2f(1.0f-TexInv, 1.0);	glVertex3f(x,	LOWERZCOORD, ystart);
			glEnd();
			}
}

/*   Draws an Y wall:
	//
	//         | an Y-wall
	//     ____V___
	//    |        |
	//    |        |
	//    |________|
	//         ^
	//         | an Y-wall
*/
void Draw_Y_Wall(int X_start, int Y, int tex, float TexInv)
{
	float xstart, xend, x, y, z;
	int y_,DistanceSquare,lod;//int tex_,i,j,k=0;

	y			=(float)(-Y);
	xstart=(float)(X_start);
	xend	=(float)(X_start+1);
	x=(xstart+xend)/2;
	z=LOWERZCOORD;
	nOfPoly++;
	//tex_=tex;
	y_=Y-TexInv;

	DistanceSquare=DistanceSquares[X_start][y_];
	lod=0;
	if (DistanceSquare<20) lod=1;
	if (DistanceSquare<3) lod=2;

	switch	(tex){
			case 0: case 2:	case 52://White Stone
				Draw_Detailed_Wall(xstart, xend, y, y, tex, TexInv, WhiteStoneDetails, DistanceSquare);
				break;
			case 6: //Posters
				Draw_Detailed_Wall(xstart, xend, y, y, 0, TexInv, WhiteStoneDetails, DistanceSquare);
				glEnable(GL_TEXTURE_2D);
				glBindTexture(GL_TEXTURE_2D, Posters[CheckMapModificator(&X_start, &y_)]);
				glBegin(GL_QUADS);
					glTexCoord2f(1.0f-TexInv, 0.0);	glVertex3f(xend-0.1,   UPPERZCOORD-0.1, y+(1-2*TexInv)*0.01);
					glTexCoord2f(TexInv-0.0f, 0.0);	glVertex3f(xstart+0.1, UPPERZCOORD-0.1, y+(1-2*TexInv)*0.01);
					glTexCoord2f(TexInv-0.0f, 1.0);	glVertex3f(xstart+0.1, LOWERZCOORD+0.1, y+(1-2*TexInv)*0.01);
					glTexCoord2f(1.0f-TexInv, 1.0);	glVertex3f(xend-0.1,   LOWERZCOORD+0.1, y+(1-2*TexInv)*0.01);
				glEnd();
				glDisable(GL_TEXTURE_2D);
				break;
			case 14: case 16: case 66: case 70: case 78: //BlueStone
				Draw_Detailed_Wall(xstart, xend, y, y, tex, TexInv, BlueStoneDetails, DistanceSquare);
				break;
			case 20: //Portraits
				Vid_SelectTexture(22);
				glBegin(GL_QUADS);
					glTexCoord2f(1.0f-TexInv, 0.0);	glVertex3f(xend,   UPPERZCOORD, y);
					glTexCoord2f(TexInv-0.0f, 0.0);	glVertex3f(xstart, UPPERZCOORD, y);
					glTexCoord2f(TexInv-0.0f, 1.0);	glVertex3f(xstart, LOWERZCOORD, y);
					glTexCoord2f(1.0f-TexInv, 1.0);	glVertex3f(xend,   LOWERZCOORD, y);
				glEnd();
				//if (useDetail && distance < DETAIL_DISTANCE)
				{
					
					glBindTexture(GL_TEXTURE_2D, Portraits[CheckMapModificator(&X_start, &y_)]);
					glBegin(GL_QUADS);
						glTexCoord2f(1.0f-TexInv, 0.0);	glVertex3f(xend-0.15,   UPPERZCOORD-0.1, y+(1-2*TexInv)*0.01);
						glTexCoord2f(TexInv-0.0f, 0.0);	glVertex3f(xstart+0.15, UPPERZCOORD-0.1, y+(1-2*TexInv)*0.01);
						glTexCoord2f(TexInv-0.0f, 1.0);	glVertex3f(xstart+0.15, LOWERZCOORD+0.1, y+(1-2*TexInv)*0.01);
						glTexCoord2f(1.0f-TexInv, 1.0);	glVertex3f(xend-0.15,   LOWERZCOORD+0.1, y+(1-2*TexInv)*0.01);
					glEnd();
					}
				break;
			case 32: case 74: //Bricks
				Draw_Detailed_Wall(xstart, xend, y, y, tex, TexInv, BrickDetails, DistanceSquare);
				break;
			case 56: case 58: case 60: case 62: //Rocks
				Draw_Detailed_Wall(xstart, xend, y, y, tex, TexInv, RockDetails, DistanceSquare);
				break;
			case 68: case 72: case 76: //Cement blocks
				Draw_Detailed_Wall(xstart, xend, y, y, tex, TexInv, CementDetails, DistanceSquare);
				break;
			case 82: case 90: //Orange Marble
				glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,128);//max 128
				Draw_Detailed_Wall(xstart, xend, y, y, tex, TexInv, MarbleDetails, DistanceSquare);
				glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,0);
				break;
			case 86: case 88: //Orange Stone
				Draw_Detailed_Wall(xstart, xend, y, y, tex, TexInv, OrangeStoneDetails, DistanceSquare);
				break;
			case 8://Cell
				DrawCell(&x, &y, &X_start, &y_, &z, &TexInv,&lod);
				floorvis[X_start][y_]=1;
				break;
			case 10://Iron-Eagle Staue(Dark)
				DrawIronEagle(&x, &y, &X_start, &y_, &z, &TexInv,&lod);
				floorvis[X_start][y_]=1;
				break;
			case 12://Celled Skeleton
				DrawCelledSkeleton(&x, &y, &X_start, &y_, &z, &TexInv,&lod);
				floorvis[X_start][y_]=1;
				break;
			default:
			Vid_SelectTexture(tex);
			glBegin(GL_QUADS);
				glTexCoord2f(1.0f-TexInv, 0.0);	glVertex3f(xend,   UPPERZCOORD, y);
				glTexCoord2f(TexInv-0.0f, 0.0);	glVertex3f(xstart, UPPERZCOORD, y);
				glTexCoord2f(TexInv-0.0f, 1.0);	glVertex3f(xstart, LOWERZCOORD, y);
				glTexCoord2f(1.0f-TexInv, 1.0);	glVertex3f(xend,   LOWERZCOORD, y);
			glEnd();
			}
}

// Draws X PushWall
void Draw_X_Pwall(int X, int Y_start, int tex, float TexInv)
{
	float x, y, z, yend, ystart,d;
	int x_,lod=2;

	x			=(float)X;
	yend  =(float)(-1-Y_start);
	ystart=(float)(-Y_start);
	y=(ystart+yend)/2;	

	x			+=PWall.dx*PWall.PWpointsmoved/128.0f;
	yend	-=PWall.dy*PWall.PWpointsmoved/128.0f;
	ystart-=PWall.dy*PWall.PWpointsmoved/128.0f;
	x_=x-TexInv;
	z=LOWERZCOORD;
	d=2+TexInv;

	nOfPoly++;

	switch	(tex){
			//case 9://Iron-Eagle Staue
			//	k=-1;
			case 10://Iron-Eagle Staue (Dark)
				DrawIronEagle(&x, &y, &x_, &Y_start, &z, &d, &lod);
				break;
			default:
			Vid_SelectTexture(tex+1);
			glBegin(GL_QUADS);
				glTexCoord2f(1.0f-TexInv, 0.0);	glVertex3f(x, UPPERZCOORD, ystart);
				glTexCoord2f(TexInv-0.0f, 0.0);	glVertex3f(x, UPPERZCOORD, yend);
				glTexCoord2f(TexInv-0.0f, 1.0);	glVertex3f(x,	LOWERZCOORD, yend);
				glTexCoord2f(1.0f-TexInv, 1.0);	glVertex3f(x,	LOWERZCOORD, ystart);
		glEnd();
	}
}

// Draws Y PushWall
void Draw_Y_Pwall(int X_start, int Y, int tex, float TexInv)
{
	float xstart, xend, x, y, z;
	int y_,lod=2;

	y			=(float)(-Y);
	xstart=(float)(X_start);
	xend	=(float)(X_start+1);

	xstart+=PWall.dx*PWall.PWpointsmoved/128.0f;
	xend	+=PWall.dx*PWall.PWpointsmoved/128.0f;
	y			-=PWall.dy*PWall.PWpointsmoved/128.0f;

	x=(xstart+xend)/2;
	z=LOWERZCOORD;
	y_=Y-TexInv;

	nOfPoly++;

	switch	(tex){
			//case 9://Iron-Eagle Staue
			//	k=-1;//break;
			case 10://Iron-Eagle Staue(Dark)
				DrawIronEagle(&x, &y, &X_start, &y_, &z, &TexInv,&lod);
				break;
			default:
			Vid_SelectTexture(tex);
			glBegin(GL_QUADS);
				glTexCoord2f(1.0f-TexInv, 0.0);	glVertex3f(xend,   UPPERZCOORD, y);
				glTexCoord2f(TexInv-0.0f, 0.0);	glVertex3f(xstart, UPPERZCOORD, y);
				glTexCoord2f(TexInv-0.0f, 1.0);	glVertex3f(xstart, LOWERZCOORD, y);
			glTexCoord2f(1.0f-TexInv, 1.0);	glVertex3f(xend,   LOWERZCOORD, y);
			glEnd();
	}
}

// Draws X Door
void Draw_X_Door(int X, int Y, int tex)
{
	float x, yend, ystart;
	
	if(!DoorMap[X][Y])
	{
		Con_Printf("No door to draw!\n");
		return;
	}
	if(DoorMap[X][Y]->action==dr_open)
	{
		Con_Printf("Trying to draw an opened door!\n");
		return;
	}
	x			=(float)X+0.5f;
	yend  =(float)(-1-Y);
	ystart=(float)(-Y);

	yend	+=DoorMap[X][Y]->tickcount/63.0f;
	ystart+=DoorMap[X][Y]->tickcount/63.0f;

	nOfPoly++;

	Vid_SelectTexture(tex+1);
	glBegin(GL_QUADS);
		glTexCoord2f(1.0f, 0.0); glVertex3f(x, UPPERZCOORD, ystart);
		glTexCoord2f(0.0f, 0.0); glVertex3f(x, UPPERZCOORD, yend);
		glTexCoord2f(0.0f, 1.0); glVertex3f(x,	LOWERZCOORD, yend);
		glTexCoord2f(1.0f, 1.0); glVertex3f(x,	LOWERZCOORD, ystart);
	glEnd();
}

// Draws Y Door
void Draw_Y_Door(int X, int Y, int tex)
{
	float xstart, xend, y;

	if(!DoorMap[X][Y])
	{
		Con_Printf("No door to draw!\n");
		return;
	}
	if(DoorMap[X][Y]->action==dr_open)
	{
		Con_Printf("Trying to draw an opened door!\n");
		return;
	}
	y			=-(float)Y-0.5f;
	xstart=(float)X;
	xend	=(float)X+1.0f;

	xend	+= DoorMap[X][Y]->tickcount/63.0f;
	xstart+= DoorMap[X][Y]->tickcount/63.0f;

	nOfPoly++;

	Vid_SelectTexture(tex);
	glBegin(GL_QUADS);
		glTexCoord2f(1.0f, 0.0); glVertex3f(xend,   UPPERZCOORD, y);
		glTexCoord2f(0.0f, 0.0); glVertex3f(xstart, UPPERZCOORD, y);
		glTexCoord2f(0.0f, 1.0); glVertex3f(xstart, LOWERZCOORD, y);
		glTexCoord2f(1.0f, 1.0); glVertex3f(xend,	 LOWERZCOORD, y);
	glEnd();
}

// Draws all visible sprites
void DrawSprites(void)
{
	int n;
	float sina, cosa;
	float Ex, Ey, Dx, Dy;
	float Ex0, Ey0, Dx0, Dy0;
	int ang;
	int n_sprt;
	float x_,y_;//added by Idot
	int DistanceSquare,lod;//added by Idot
	//int lamps=0;//added by Idot


	n_sprt=Spr_CreateVisList();
	n_sprt_=n_sprt;///debug added by Idot
	if(!n_sprt) return; // nothing to draw

	ang=NormalizeAngle(Player.angle+ANG_90);

	sina=(float)(0.5*SinTable[ang]);
	cosa=(float)(0.5*CosTable[ang]);
	Ex0= cosa; Ey0= sina;
	Dx0=-cosa; Dy0=-sina;
	
//	for (n=lamps;n>-1; n--)glDisable(GL_LIGHT0+lamps);
///		};

	for(n=0; n<n_sprt; n++)
	{
		if(vislist[n].dist<MINDIST/2) continue; // little hack to save speed & z-buffer
		nOfPoly++;
				Ex=Ex0+vislist[n].x/FLOATTILE; 
				Ey=Ey0+vislist[n].y/FLOATTILE;
				Dx=Dx0+vislist[n].x/FLOATTILE;
				Dy=Dy0+vislist[n].y/FLOATTILE;
				x_=(Ex+Dx)/2;
				y_=(Ey+Dy)/2;
		DistanceSquare=DistanceSquares[(int)x_][(int)y_];
		//DistanceSquare=GetDistanceSquare(&x_,&y_);
		lod=0;
		if (DistanceSquare<20) lod=1;
		if (DistanceSquare<3) lod=2;
if (DistanceSquare<2000){//use sprites or md3
//if (DistanceSquare<200){//use sprites or md3
	if (!IS_SPEAR)switch (vislist[n].tex){
		///////////////////////////////////////////////////////////////////////////////////////////////////////////
		//Wolfenstain
		///////////////////////////////////////////////////////////////////////////////////////////////////////////
			/*
			2 лужа
			3 металическа€ бочка
			7 повешенный за руки скелет
			8 собачий корм
			12 раковина
			черепа нанизанные на кол
			17 вис€щие сковродки
			кровавые останки в клетке
			19 пуста€ клетка
			20 скелет в клетке
			21 груда костей
			24 кушетка
			черепа в клетке
			25 корзина
			26 окорок с ганиром
			27 аптечка
			28 магазин
			29 шмайсер
			30 прототип венома
			34 корона
			35 сфера с жизнью
			36 кровавые останки с кост€ми
			37 дерев€нна€ бочка
			40 кровь на полу
			41 флаг на флагштоке
			42 красна€ лампа (SOD only!!!)
			43-45 мусор на полу
			46 вис€щие кастрюл€ сковрода и поварЄшка
			кровавый крест с черепом
			47 печь-буружуйка
			кровавые останки в колодце
			48 копь€ в стойке
			стату€ демона		
			*/
			case 4://Table with Chairs
				DrawTableChairs(&x_, &y_,&lod);
				break;
			case 5://Torcher
				DrawTorcher(&x_, &y_,&lod);
				break;
			case 6://Chandelier
				DrawChandelier(&x_, &y_,&lod);
				break;
			case 9://Column
				DrawColumn(&x_, &y_,&lod);
				break;
			case 10://Round Tree
				DrawRoundTree(&x_, &y_,&lod);
				break;
			case 11://Lying Skeleton
				DrawLyingSkeleton(&x_, &y_,&lod);
				break;
			case 13://Vased Tree
				DrawVasedTree(&x_, &y_,&lod);
				break;
			case 14://Vase
				DrawVase(&x_, &y_,&lod);
				break;
			case 15://Standalone Table
				DrawTable(&x_, &y_,&lod);
				break;
			case 16://Lamp
				DrawLamp(&x_, &y_,&lod);
				break;
			case 18://Knight
				DrawKnight(&x_, &y_,&lod);
				break;
			case 22://Golden Key
				DrawGoldenKey(&x_, &y_,&lod);
				break;
			case 23://Silver Key
				DrawSilverKey(&x_, &y_,&lod);
				break;
			case 24://Bed
				DrawBed(&x_, &y_,&lod);
				break;
			case 31://Cross
				DrawCross(&x_, &y_,&lod);
				break;
			case 32://Goblet
				DrawGoblet(&x_, &y_,&lod);
				break;
			case 33://Treasure Box
				DrawTreasureBox(&x_, &y_,&lod);
				break;
			case 38://Water Well
				DrawWaterWell(&x_, &y_,&lod);
				break;
			case 39://Empty Well
				DrawWell(&x_, &y_,&lod);
				break;			
			case 48://Holder with Spears
				DrawSpears(&x_, &y_,&lod);
				break;
			case 49://Roots
				DrawRoots(&x_, &y_,&lod);
				break;

			default:
			Vid_SelectTextureEx(vislist[n].tex|TEX_SPRITE_BIT);
			glBegin(GL_QUADS);
				glTexCoord2f(0.0, 0.0);	glVertex3f(Ex, UPPERZCOORD, -Ey);
				glTexCoord2f(1.0, 0.0);	glVertex3f(Dx, UPPERZCOORD, -Dy);
			glTexCoord2f(1.0, 1.0);	glVertex3f(Dx, LOWERZCOORD, -Dy);
			glTexCoord2f(0.0, 1.0);	glVertex3f(Ex, LOWERZCOORD, -Ey);
			glEnd();}
	else switch (vislist[n].tex){
		///////////////////////////////////////////////////////////////////////////////////////////////////////////
		//Spear of Destiny
		///////////////////////////////////////////////////////////////////////////////////////////////////////////
			/*
			SOD only:
			50 мраморна€ колонна
			51 €щик с патронами
			52 грузовичок
			53 копьЄ судьбы
			*/
			case 4://Table with Chairs
				DrawTableChairs(&x_, &y_,&lod);
				break;
			case 5://Torcher
				DrawTorcher(&x_, &y_,&lod);
				break;
			case 6://Chandelier
				DrawChandelier(&x_, &y_,&lod);
				break;
			case 9://Column
				DrawColumn(&x_, &y_,&lod);
				break;
			case 10://Round Tree
				DrawRoundTree(&x_, &y_,&lod);
				break;
			case 11://Lying Skeleton
				DrawLyingSkeleton(&x_, &y_,&lod);
				break;
			case 13://Vased Tree
				DrawVasedTree(&x_, &y_,&lod);
				break;
			case 14://Vase
				DrawVase(&x_, &y_,&lod);
				break;
			case 15://Standalone Table
				DrawTable(&x_, &y_,&lod);
				break;
			case 16://Lamp
				DrawLamp(&x_, &y_,&lod);
				break;
			case 18://Knight
				DrawKnight(&x_, &y_,&lod);
				break;
			case 22://Golden Key
				DrawGoldenKey(&x_, &y_,&lod);
				break;
			case 23://Silver Key
				DrawSilverKey(&x_, &y_,&lod);
				break;
			case 31://Cross
				DrawCross(&x_, &y_,&lod);
				break;
			case 32://Goblet
				DrawGoblet(&x_, &y_,&lod);
				break;
			case 33://Treasure Box
				DrawTreasureBox(&x_, &y_,&lod);
				break;
			case 38://Water Well
				DrawWaterWell(&x_, &y_,&lod);
				break;
			case 39://Empty Well
				DrawWell(&x_, &y_,&lod);
				break;
			case 42://Red Lamp
				DrawRedLamp(&x_, &y_,&lod);
				break;		
			case 49://Roots
				DrawRoots(&x_, &y_,&lod);
				break;
			case 50://MrableColumn
				DrawMarbleColumn(&x_, &y_,&lod);
				break;

			default:
			Vid_SelectTextureEx(vislist[n].tex|TEX_SPRITE_BIT);
			glBegin(GL_QUADS);
				glTexCoord2f(0.0, 0.0);	glVertex3f(Ex, UPPERZCOORD, -Ey);
				glTexCoord2f(1.0, 0.0);	glVertex3f(Dx, UPPERZCOORD, -Dy);
			glTexCoord2f(1.0, 1.0);	glVertex3f(Dx, LOWERZCOORD, -Dy);
			glTexCoord2f(0.0, 1.0);	glVertex3f(Ex, LOWERZCOORD, -Ey);
			glEnd();
	}}else{
			Vid_SelectTextureEx(vislist[n].tex|TEX_SPRITE_BIT);
			glBegin(GL_QUADS);
				glTexCoord2f(0.0, 0.0);	glVertex3f(Ex, UPPERZCOORD, -Ey);
				glTexCoord2f(1.0, 0.0);	glVertex3f(Dx, UPPERZCOORD, -Dy);
			glTexCoord2f(1.0, 1.0);	glVertex3f(Dx, LOWERZCOORD, -Dy);
			glTexCoord2f(0.0, 1.0);	glVertex3f(Ex, LOWERZCOORD, -Ey);
			glEnd();
	}	}
/*
	glEnable(GL_BLEND);
	glDisable(GL_ALPHA_TEST);
*/
}

#define TEXTUREDFLOORCEL


// Draws Celing & Floor
void DrawCelFloor(void)
{
	int x,y,i;
	int DistanceSquare,lod,Xmap_Pos,Ymap_Pos;//added by Idot

#define TEXTUREDFLOORCEL //added by Idot
#ifdef TEXTUREDFLOORCEL
	glEnable(GL_TEXTURE_2D);	// textured
	glColor3ub(255, 255, 255);
#else
	glDisable(GL_TEXTURE_2D);	// solid
#endif

	Xmap_Pos=POS2TILE(Player.x);
	Ymap_Pos=POS2TILE(Player.y);

//	Xmap_Pos=31;
//	Ymap_Pos=6;




			Vid_SelectCel();
			glBegin(GL_QUADS);
				glTexCoord2f(0.0, 0.0);	glVertex3f((float)(0),				UPPERZCOORD, (float)(0));
				glTexCoord2f(1.0, 0.0);	glVertex3f((float)(Xmap_Pos-2),		UPPERZCOORD, (float)(0));
				glTexCoord2f(1.0, 1.0);	glVertex3f((float)(Xmap_Pos-2),		UPPERZCOORD, (float)(-64));
				glTexCoord2f(0.0, 1.0);	glVertex3f((float)(0),				UPPERZCOORD, (float)(-64));

				glTexCoord2f(0.0, 0.0);	glVertex3f((float)(Xmap_Pos-2),		UPPERZCOORD, (float)(0));
				glTexCoord2f(1.0, 0.0);	glVertex3f((float)(Xmap_Pos+3),		UPPERZCOORD, (float)(0));
				glTexCoord2f(1.0, 1.0);	glVertex3f((float)(Xmap_Pos+3),		UPPERZCOORD, (float)(-Ymap_Pos+2));
				glTexCoord2f(0.0, 1.0);	glVertex3f((float)(Xmap_Pos-2),		UPPERZCOORD, (float)(-Ymap_Pos+2));

				/*if (!IsMultiTexEnabled){
				glTexCoord2f(0.0, 0.0);	glVertex3f((float)(Xmap_Pos-2),		UPPERZCOORD, (float)(-Ymap_Pos+2));
				glTexCoord2f(1.0, 0.0);	glVertex3f((float)(Xmap_Pos+3),		UPPERZCOORD, (float)(-Ymap_Pos+2));
				glTexCoord2f(1.0, 1.0);	glVertex3f((float)(Xmap_Pos+3),		UPPERZCOORD, (float)(-Ymap_Pos-3));
				glTexCoord2f(0.0, 1.0);	glVertex3f((float)(Xmap_Pos-2),		UPPERZCOORD, (float)(-Ymap_Pos-3));
				}*/

				glTexCoord2f(0.0, 0.0);	glVertex3f((float)(Xmap_Pos-2),		UPPERZCOORD, (float)(-Ymap_Pos-3));
				glTexCoord2f(1.0, 0.0);	glVertex3f((float)(Xmap_Pos+3),		UPPERZCOORD, (float)(-Ymap_Pos-3));
				glTexCoord2f(1.0, 1.0);	glVertex3f((float)(Xmap_Pos+3),		UPPERZCOORD, (float)(-64));
				glTexCoord2f(0.0, 1.0);	glVertex3f((float)(Xmap_Pos-2),		UPPERZCOORD, (float)(-64));


				glTexCoord2f(0.0, 0.0);	glVertex3f((float)(Xmap_Pos+3),		UPPERZCOORD, (float)(0));
				glTexCoord2f(1.0, 0.0);	glVertex3f((float)(64),				UPPERZCOORD, (float)(0));
				glTexCoord2f(1.0, 1.0);	glVertex3f((float)(64),				UPPERZCOORD, (float)(-64));
				glTexCoord2f(0.0, 1.0);	glVertex3f((float)(Xmap_Pos+3),		UPPERZCOORD, (float)(-64));
			glEnd();
			//if (useDetail && distance < DETAIL_DISTANCE)
			//if(DistanceSquare<10){
				glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
				glActiveTextureARB(GL_TEXTURE0_ARB); 
				glEnable(GL_TEXTURE_2D);
				Vid_SelectCel(); 
				glActiveTextureARB(GL_TEXTURE1_ARB); 
				glEnable(GL_TEXTURE_2D);
				glBindTexture(GL_TEXTURE_2D, CeilDetails); 
				glBegin(GL_QUADS);
					glMultiTexCoord2fARB(GL_TEXTURE0_ARB, 0.0f,  0.0f); 
					glMultiTexCoord2fARB(GL_TEXTURE1_ARB, 0.0f, 0.0f); 
						glVertex3f((float)(Xmap_Pos-2),		UPPERZCOORD, (float)(-Ymap_Pos+2));
					glMultiTexCoord2fARB(GL_TEXTURE1_ARB, 8.0f,  0.0f); 
					glMultiTexCoord2fARB(GL_TEXTURE0_ARB, 1.0f, 0.0f); 
						glVertex3f((float)(Xmap_Pos+3),		UPPERZCOORD, (float)(-Ymap_Pos+2));
					glMultiTexCoord2fARB(GL_TEXTURE1_ARB, 8.0f,  8.0f); 
					glMultiTexCoord2fARB(GL_TEXTURE0_ARB, 1.0f, 1.0f); 
						glVertex3f((float)(Xmap_Pos+3),		UPPERZCOORD, (float)(-Ymap_Pos-3));
					glMultiTexCoord2fARB(GL_TEXTURE1_ARB, 0.0f,  8.0f); 
					glMultiTexCoord2fARB(GL_TEXTURE0_ARB, 0.0f, 1.0f); 
						glVertex3f((float)(Xmap_Pos-2),		UPPERZCOORD, (float)(-Ymap_Pos-3));
				glEnd();				
				(*glActiveTextureARB)(GL_TEXTURE1_ARB);
				glDisable(GL_TEXTURE_2D);
				(*glActiveTextureARB)(GL_TEXTURE0_ARB);
				glDisable(GL_TEXTURE_2D);
				glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
			//if (IsMultiTexEnabled){//Fix IsMultiTexEnabled !!!!
				/*}else{
				glDepthFunc(GL_EQUAL);
				glBlendFunc(GL_DST_COLOR, GL_SRC_COLOR);
				glBindTexture(GL_TEXTURE_2D, CeilDetails);
				glBegin(GL_QUADS);

					glTexCoord2f(0.0, 0.0);	glVertex3f((float)(Xmap_Pos-2),		UPPERZCOORD, (float)(-Ymap_Pos+2));
					glTexCoord2f(10.0, 0.0);	glVertex3f((float)(Xmap_Pos+3),		UPPERZCOORD, (float)(-Ymap_Pos+2));
					glTexCoord2f(10.0, 10.0);	glVertex3f((float)(Xmap_Pos+3),		UPPERZCOORD, (float)(-Ymap_Pos-3));
					glTexCoord2f(0.0, 10.0);	glVertex3f((float)(Xmap_Pos-2),		UPPERZCOORD, (float)(-Ymap_Pos-3));
				glEnd();
				glDepthFunc(GL_LEQUAL); 
				glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
				}*/	
						
			Vid_SelectFloor();
			glBegin(GL_QUADS);
				glTexCoord2f(0.0, 0.0);	glVertex3f((float)(0),				LOWERZCOORD, (float)(0));
				glTexCoord2f(1.0, 0.0);	glVertex3f((float)(Xmap_Pos-2),		LOWERZCOORD, (float)(0));
				glTexCoord2f(1.0, 1.0);	glVertex3f((float)(Xmap_Pos-2),		LOWERZCOORD, (float)(-64));
				glTexCoord2f(0.0, 1.0);	glVertex3f((float)(0),				LOWERZCOORD, (float)(-64));

				glTexCoord2f(0.0, 0.0);	glVertex3f((float)(Xmap_Pos-2),		LOWERZCOORD, (float)(0));
				glTexCoord2f(1.0, 0.0);	glVertex3f((float)(Xmap_Pos+3),		LOWERZCOORD, (float)(0));
				glTexCoord2f(1.0, 1.0);	glVertex3f((float)(Xmap_Pos+3),		LOWERZCOORD, (float)(-Ymap_Pos+2));
				glTexCoord2f(0.0, 1.0);	glVertex3f((float)(Xmap_Pos-2),		LOWERZCOORD, (float)(-Ymap_Pos+2));

				/*if (!IsMultiTexEnabled){
				glTexCoord2f(0.0, 0.0);	glVertex3f((float)(Xmap_Pos-2),		LOWERZCOORD, (float)(-Ymap_Pos+2));
				glTexCoord2f(1.0, 0.0);	glVertex3f((float)(Xmap_Pos+3),		LOWERZCOORD, (float)(-Ymap_Pos+2));
				glTexCoord2f(1.0, 1.0);	glVertex3f((float)(Xmap_Pos+3),		LOWERZCOORD, (float)(-Ymap_Pos-3));
				glTexCoord2f(0.0, 1.0);	glVertex3f((float)(Xmap_Pos-2),		LOWERZCOORD, (float)(-Ymap_Pos-3));
				}*/

				glTexCoord2f(0.0, 0.0);	glVertex3f((float)(Xmap_Pos-2),		LOWERZCOORD, (float)(-Ymap_Pos-3));
				glTexCoord2f(1.0, 0.0);	glVertex3f((float)(Xmap_Pos+3),		LOWERZCOORD, (float)(-Ymap_Pos-3));
				glTexCoord2f(1.0, 1.0);	glVertex3f((float)(Xmap_Pos+3),		LOWERZCOORD, (float)(-64));
				glTexCoord2f(0.0, 1.0);	glVertex3f((float)(Xmap_Pos-2),		LOWERZCOORD, (float)(-64));


				glTexCoord2f(0.0, 0.0);	glVertex3f((float)(Xmap_Pos+3),		LOWERZCOORD, (float)(0));
				glTexCoord2f(1.0, 0.0);	glVertex3f((float)(64),				LOWERZCOORD, (float)(0));
				glTexCoord2f(1.0, 1.0);	glVertex3f((float)(64),				LOWERZCOORD, (float)(-64));
				glTexCoord2f(0.0, 1.0);	glVertex3f((float)(Xmap_Pos+3),		LOWERZCOORD, (float)(-64));
			glEnd();
			//if (useDetail && distance < DETAIL_DISTANCE)
			//if(DistanceSquare<10){
				glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
				glActiveTextureARB(GL_TEXTURE0_ARB); 
				glEnable(GL_TEXTURE_2D);
				Vid_SelectFloor(); 
				glActiveTextureARB(GL_TEXTURE1_ARB); 
				glEnable(GL_TEXTURE_2D);
				glBindTexture(GL_TEXTURE_2D, FloorDetails); 
				glBegin(GL_QUADS);
					glMultiTexCoord2fARB(GL_TEXTURE0_ARB, 0.0f,  0.0f); 
					glMultiTexCoord2fARB(GL_TEXTURE1_ARB, 0.0f, 0.0f); 
						glVertex3f((float)(Xmap_Pos-2),		LOWERZCOORD, (float)(-Ymap_Pos+2));
					glMultiTexCoord2fARB(GL_TEXTURE1_ARB, 8.0f,  0.0f); 
					glMultiTexCoord2fARB(GL_TEXTURE0_ARB, 1.0f, 0.0f); 
						glVertex3f((float)(Xmap_Pos+3),		LOWERZCOORD, (float)(-Ymap_Pos+2));
					glMultiTexCoord2fARB(GL_TEXTURE1_ARB, 8.0f,  8.0f); 
					glMultiTexCoord2fARB(GL_TEXTURE0_ARB, 1.0f, 1.0f); 
						glVertex3f((float)(Xmap_Pos+3),		LOWERZCOORD, (float)(-Ymap_Pos-3));
					glMultiTexCoord2fARB(GL_TEXTURE1_ARB, 0.0f,  8.0f); 
					glMultiTexCoord2fARB(GL_TEXTURE0_ARB, 0.0f, 1.0f); 
						glVertex3f((float)(Xmap_Pos-2),		LOWERZCOORD, (float)(-Ymap_Pos-3));
				glEnd();				
				(*glActiveTextureARB)(GL_TEXTURE1_ARB);
				glDisable(GL_TEXTURE_2D);
				(*glActiveTextureARB)(GL_TEXTURE0_ARB);
				glDisable(GL_TEXTURE_2D);
				glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
			//if (IsMultiTexEnabled){//Fix IsMultiTexEnabled !!!!
				/*}else{
				glDepthFunc(GL_EQUAL);
				glBlendFunc(GL_DST_COLOR, GL_SRC_COLOR);
				glBindTexture(GL_TEXTURE_2D, FloorDetails);
				glBegin(GL_QUADS);

					glTexCoord2f(0.0, 0.0);	glVertex3f((float)(Xmap_Pos-2),		LOWERZCOORD, (float)(-Ymap_Pos+2));
					glTexCoord2f(10.0, 0.0);	glVertex3f((float)(Xmap_Pos+3),		LOWERZCOORD, (float)(-Ymap_Pos+2));
					glTexCoord2f(10.0, 10.0);	glVertex3f((float)(Xmap_Pos+3),		LOWERZCOORD, (float)(-Ymap_Pos-3));
					glTexCoord2f(0.0, 10.0);	glVertex3f((float)(Xmap_Pos-2),		LOWERZCOORD, (float)(-Ymap_Pos-3));
				glEnd();
				glDepthFunc(GL_LEQUAL); 
				glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
				}*/	




/*
	for(x=0; x<64; x++)
	{
		for(y=0; y<64; y++)
		{
			if(floorvis[x][y])
			{
#ifdef TEXTUREDFLOORCEL
		DistanceSquare=DistanceSquares[x][y];
			
			Vid_SelectCel();
			glBegin(GL_QUADS);
				glTexCoord2f(0.0, 0.0);	glVertex3f((float)x  ,		UPPERZCOORD, (float)(-y));
				glTexCoord2f(1.0, 0.0);	glVertex3f((float)(x+1),	UPPERZCOORD, (float)(-y));
				glTexCoord2f(1.0, 1.0);	glVertex3f((float)(x+1),	UPPERZCOORD, (float)(-y-1));
				glTexCoord2f(0.0, 1.0);	glVertex3f((float)x,		UPPERZCOORD, (float)(-y-1));
			glEnd();
			//if (useDetail && distance < DETAIL_DISTANCE)
			/*
			{
				glDepthFunc(GL_EQUAL);
				glBlendFunc(GL_DST_COLOR, GL_SRC_COLOR);
				glBindTexture(GL_TEXTURE_2D, CementDetails);
				glBegin(GL_QUADS);
					glTexCoord2f(0.0, 0.0);	glVertex3f((float)x,		UPPERZCOORD, (float)(-y));
					glTexCoord2f(2.0, 0.0);	glVertex3f((float)(x+1),	UPPERZCOORD, (float)(-y));
					glTexCoord2f(2.0, 2.0);	glVertex3f((float)(x+1),	UPPERZCOORD, (float)(-y-1));
					glTexCoord2f(0.0, 2.0);	glVertex3f((float)x,		UPPERZCOORD, (float)(-y-1));
				glEnd();
				glDepthFunc(GL_LEQUAL); 
				glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
				}	*/
/*						
			Vid_SelectFloor();
			glBegin(GL_QUADS);
				glTexCoord2f(0.0, 0.0);	glVertex3f((float)x,			LOWERZCOORD, (float)(-y));
				glTexCoord2f(1.0, 0.0);	glVertex3f((float)(x+1),  LOWERZCOORD, (float)(-y));
				glTexCoord2f(1.0, 1.0);	glVertex3f((float)(x+1),	LOWERZCOORD, (float)(-y-1));
				glTexCoord2f(0.0, 1.0);	glVertex3f((float)x,			LOWERZCOORD, (float)(-y-1));
			glEnd();
			//if (useDetail && distance < DETAIL_DISTANCE)
			if(DistanceSquare<10){
				glDepthFunc(GL_EQUAL);
				glBlendFunc(GL_DST_COLOR, GL_SRC_COLOR);
				glBindTexture(GL_TEXTURE_2D, FloorDetails);
				glBegin(GL_QUADS);
					glTexCoord2f(0.0, 0.0);	glVertex3f((float)x,			LOWERZCOORD, (float)(-y));
					glTexCoord2f(2.0, 0.0);	glVertex3f((float)(x+1),  LOWERZCOORD, (float)(-y));
					glTexCoord2f(2.0, 2.0);	glVertex3f((float)(x+1),	LOWERZCOORD, (float)(-y-1));
					glTexCoord2f(0.0, 2.0);	glVertex3f((float)x,			LOWERZCOORD, (float)(-y-1));
				glEnd();
				glDepthFunc(GL_LEQUAL); 
				glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			}	
#else
			glBegin(GL_QUADS);
				glColor3ub(CurMapData.celing.r, CurMapData.celing.g, CurMapData.celing.b);
				glVertex3f((float)x  ,		UPPERZCOORD, (float)(-y));
				glVertex3f((float)(x+1),	UPPERZCOORD, (float)(-y));
				glVertex3f((float)(x+1),	UPPERZCOORD, (float)(-y-1));
				glVertex3f((float)x,			UPPERZCOORD, (float)(-y-1));

				glColor3ub(CurMapData.floor.r, CurMapData.floor.g, CurMapData.floor.b);
				glVertex3f((float)x,			LOWERZCOORD, (float)(-y));
				glVertex3f((float)(x+1),  LOWERZCOORD, (float)(-y));
				glVertex3f((float)(x+1),	LOWERZCOORD, (float)(-y-1));
				glVertex3f((float)x,			LOWERZCOORD, (float)(-y-1));
			glEnd();
#endif
			}
		}
	}
*/

	glColor3ub(255, 255, 255); // restore colors
}

#include "sprt_def.h" 
// Draw Player Hands & weapon
void DrawGun(void)
{
	glColor4ub(255, 255, 255, 255);
	Vid_SelectTextureEx((SPR_KNIFEREADY+Player.curweapon*5+Player.weapframe)|TEX_SPRITE_BIT);
	glEnable(GL_TEXTURE_2D);
	glBegin(GL_QUADS);
		glTexCoord2f(1.0f, 0.0f);	glVertex2i(480, 0);
		glTexCoord2f(0.0f, 0.0f);	glVertex2i(160, 0);
		glTexCoord2f(0.0f, 1.0f);	glVertex2i(160, 384);
		glTexCoord2f(1.0f, 1.0f);	glVertex2i(480, 384);
	glEnd();
}