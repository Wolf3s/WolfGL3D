// Font Manager
#include <windows.h>
#include <stdio.h>
#include <GL\gl.h>
#include "WolfDef.h"

#define MAX_FONTS 3

typedef struct
{
	char height;
	char width[256];
	float left[256], right[256];
	float top[256], bottom[256];
	unsigned int FNTtex;
} font_t;

typedef struct
{
	int italic;
	int shadow;
	int gradient;
	RGBAcolor color1, color2, cshadow; // normal, gradient, shaddow
	int numerator, divisor;
} fstyle_t;

typedef struct
{
	short height;
	short location[256];
	char width[256];
} fontstruct;

fstyle_t sFNT;
font_t fonts[MAX_FONTS], *curFNT;

int FNT_Init(void)
{// called after GLinit, Con_Init, File_Init & CMD, cvar init
	Con_Printf("FNT_Init: Initailizing font subsystem:\n");
	Con_Printf("-> Total Fonts: %d\n", MAX_FONTS);
	FNT_LoadWolfFont(FNT_WOLF_SMALL, 1);
	FNT_LoadWolfFont(FNT_WOLF_BIG, 2);
	Con_Printf("-> Total WolfFonts: 2\n");
	FNT_LoadRAWfont(FNT_CONSOLE, 256, 256, BASEDIR"font32.raw");

	sFNT.color1.r=sFNT.color1.g=sFNT.color1.b=sFNT.color1.a=0xFF;
	sFNT.color2.r=sFNT.color2.g=sFNT.color2.b=sFNT.color2.a=0xFF;
	sFNT.cshadow.r=sFNT.cshadow.g=sFNT.cshadow.b=0x00; sFNT.cshadow.a=0xFF;
	sFNT.italic=sFNT.shadow=sFNT.gradient=0;
	sFNT.numerator=sFNT.divisor=1;

	curFNT=&fonts[0];
	return 1;
}

void FNT_ShutDown(void)
{
	int n;

	for(n=0; n<MAX_FONTS; n++)
		glDeleteTextures(1, &fonts[n].FNTtex);
}

// ------------------------- * Devider * -------------------------
void FNT_LoadWolfFont(int fnt, int page)
{
	unsigned char outdata[256][256][4];
	unsigned char *databuff;
	long psize;
	fontstruct *header;
	font_t *newFNT;
	int n;
	int curbyte;
	int outx, outy, x, y;

	if(fnt>MAX_FONTS || fnt <0) return;
	newFNT=&fonts[fnt];
	memset(newFNT, 0x00, sizeof(fonts[fnt])); // clear font data

	psize=File_VGA_GetChunkSize(page);
	if(!psize) return;
	databuff=malloc(psize);
	header=(fontstruct *)databuff;
	File_VGA_ReadChunk(page, databuff);

	for(x=0; x<256; x++)
	{
		for(y=0; y<256; y++)
		{
			outdata[y][x][0]=outdata[y][x][1]=
			outdata[y][x][2]=0xFF;
			outdata[y][x][3]=0x00;
		}
	}

	newFNT->height=(char)(header->height*2.4); // 2.4 = oure 480 / original 200
	// make letter 0 (square, like in windows to map to it all letters, do not have symbol!
	for(y=1; y<(header->height-1); y++)
		outdata[y][header->height/6][3]=outdata[y][header->height/3][3]=0xFF;
	for(x=header->height/6; x<header->height/3; x++)
		outdata[1][x][3]=outdata[header->height-1][x][3]=0xFF;

	outx=header->height/2+1;
	outy=0;

	for(n=0; n<256; n++)
	{
		if(header->width[n] && n!=0)
		{ // have entry for this letter!
			if(outx+header->width[n] > 255)
			{
				outy+=header->height+1;
				outx=0;
			}
			curbyte=header->location[n];
			
			for(y=0; y<header->height; y++)
				for(x=0; x<header->width[n]; x++)
					if(databuff[curbyte++])	outdata[outy+y][outx+x][3]=0xFF;

			newFNT->width[n] =header->width[n]*2; // 2=640/320
			newFNT->left[n]  =(float)(outx/256.0);
			newFNT->right[n] =(float)((outx+header->width[n])/256.0);
			newFNT->top[n]	 =(float)outy; // we'll devide it later by height!
			newFNT->bottom[n]=(float)outy+header->height;
			outx+=header->width[n]+1;
		}
		else
		{
			newFNT->width[n] =(char)(newFNT->height/2);
			newFNT->left[n]  =0;
			newFNT->right[n] =(float)((header->height/2)/256.0);
			newFNT->top[n]   =0;
			newFNT->bottom[n]=(float)(header->height);
		}
	}
	
	outy+=header->height;
	free(databuff);
	for(curbyte=1; curbyte<outy; curbyte<<=1);
	for(n=0; n<256; n++)
	{
		newFNT->top[n]   /=curbyte;
		newFNT->bottom[n]/=curbyte;
	}
	
	glGenTextures(1, &newFNT->FNTtex);
	glBindTexture(GL_TEXTURE_2D, newFNT->FNTtex);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST); //GL_NEAREST); // for more nostalgic look
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexImage2D(GL_TEXTURE_2D, 0, 4, 256, curbyte, 0, GL_RGBA, GL_UNSIGNED_BYTE, outdata);
}

void FNT_LoadRAWfont(int fnt, int width, int height, char *filename)
{
	unsigned char *data;
	font_t *newFNT;
	int n;

	if(fnt>MAX_FONTS || fnt <0) return;
	newFNT=&fonts[fnt];
	memset(newFNT, 0x00, sizeof(fonts[fnt])); // clear font data

	newFNT->height=16;
	for(n=0; n<256; n++)
	{
		newFNT->width[n]=16;
		newFNT->left[n]	 =(n%16)	*0.0625f;
		newFNT->right[n] =(n%16+1)*0.0625f;
		newFNT->top[n]	 =(n/16)	*0.0625f;
		newFNT->bottom[n]=(n/16+1)*0.0625f;
	}

	data=malloc(width*height*4);  // all 4 components!
	
	File_RAW_Read(filename, 0, data, width, height, 0);

	glGenTextures(1, &newFNT->FNTtex);
	glBindTexture(GL_TEXTURE_2D, newFNT->FNTtex);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexImage2D(GL_TEXTURE_2D, 0, 4, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

	free(data);
}

// ------------------------- * Devider * -------------------------
void FNT_SetFont(int font)
{
	if(font>=MAX_FONTS || font<0) return;
	curFNT=&fonts[font];
}

void FNT_SetStyle(int italic, int shadow, int gradient)
{
	sFNT.italic=italic;
	sFNT.shadow=shadow;
	sFNT.gradient=gradient;
}

void FNT_SetColor(unsigned char R, unsigned char G, unsigned char B, unsigned char A)
{
	sFNT.color1.r=R;
	sFNT.color1.g=G;
	sFNT.color1.b=B;
	sFNT.color1.a=A;
}

void FNT_SetRGBAcolor(RGBAcolor *c)
{
	sFNT.color1=*c;
}

void FNT_SetColor2(unsigned char R, unsigned char G, unsigned char B, unsigned char A)
{
	sFNT.color2.r=R;
	sFNT.color2.g=G;
	sFNT.color2.b=B;
	sFNT.color2.a=A;
}

void FNT_SetRGBAcolor2(RGBAcolor *c)
{
	sFNT.color2=*c;
}

void FNT_SetColorSh(unsigned char R, unsigned char G, unsigned char B, unsigned char A)
{
	sFNT.cshadow.r=R;
	sFNT.cshadow.g=G;
	sFNT.cshadow.b=B;
	sFNT.cshadow.a=A;
}

void FNT_SetRGBAcolorSh(RGBAcolor *c)
{
	sFNT.cshadow=*c;
}

void FNT_SetScale(int numerator, int divisor)
{
	sFNT.numerator=numerator;
	sFNT.divisor=divisor;
}

// ------------------------- * Printing * -------------------------
void FNT_Print(int x, int y, char *text)
{
	int cx, cy, sw, sh;
	unsigned char c;

	cx=x; cy=y;
	sh=curFNT->height*sFNT.numerator/sFNT.divisor; // scaled height

	glColor4ubv((unsigned char *)&sFNT.color1);
	glEnable(GL_BLEND);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, curFNT->FNTtex);

	glBegin(GL_QUADS);
	while( (c=*text++) )
	{
		if(c=='\n')
		{
			cx=x;
			cy+=sh;
			continue;
		}
		// scaled width
		sw=(sFNT.numerator!=1 || sFNT.divisor!=1) ? curFNT->width[c]*sFNT.numerator/sFNT.divisor : curFNT->width[c];
		if(sFNT.shadow)
		{
			int shd;
			shd=sFNT.shadow;
			glColor4ubv((unsigned char *)&sFNT.cshadow);
			glTexCoord2f(curFNT->right[c], curFNT->top[c]);		 glVertex2i(cx+sw+sFNT.italic+shd, cy+shd);
			glTexCoord2f(curFNT->left[c],	curFNT->top[c]);		 glVertex2i(cx+sFNT.italic+shd, cy+shd);
			glTexCoord2f(curFNT->left[c],	curFNT->bottom[c]);	 glVertex2i(cx+shd, cy+sh+shd);
			glTexCoord2f(curFNT->right[c], curFNT->bottom[c]); glVertex2i(cx+sw+shd, cy+sh+shd);
			glColor4ubv((unsigned char *)&sFNT.color1);
		}
		glTexCoord2f(curFNT->right[c], curFNT->top[c]);		 glVertex2i(cx+sw+sFNT.italic, cy);
		glTexCoord2f(curFNT->left[c],	curFNT->top[c]);		 glVertex2i(cx+sFNT.italic, cy);
		if(sFNT.gradient) glColor4ubv((unsigned char *)&sFNT.color2);
		glTexCoord2f(curFNT->left[c],	curFNT->bottom[c]);	 glVertex2i(cx,	cy+sh);
		glTexCoord2f(curFNT->right[c], curFNT->bottom[c]); glVertex2i(cx+sw, cy+sh);
		if(sFNT.gradient && !sFNT.shadow) glColor4ubv((unsigned char *)&sFNT.color1);
		cx+=sw;
	}
	glEnd();
}

void FNT_PrintPos(int xl, int yl, char *text)
{
	FNT_Print(xl*curFNT->width[' ']*sFNT.numerator/sFNT.divisor, yl*curFNT->height*sFNT.numerator/sFNT.divisor, text);
}

void FNT_PrintPosOff(int xl, int yl, int ox, int oy, char *text)
{
	FNT_Print(ox+xl*curFNT->width[' ']*sFNT.numerator/sFNT.divisor, oy+yl*curFNT->height*sFNT.numerator/sFNT.divisor, text);
}

char msg[1024];

void FNT_Printf(int x, int y, char *fmt, ...)
{
	va_list	argptr;

	va_start(argptr,fmt);
	vsprintf(msg, fmt, argptr);
	va_end(argptr);

	FNT_Print(x, y, msg);
}

void FNT_PrintfPos(int xl, int yl, char *fmt, ...)
{
	va_list	argptr;

	va_start(argptr,fmt);
	vsprintf(msg, fmt, argptr);
	va_end(argptr);

	FNT_PrintPos(xl, yl, msg);
}

void FNT_PrintfPosOff(int xl, int yl, int ox, int oy, char *fmt, ...)
{
	va_list	argptr;

	va_start(argptr,fmt);
	vsprintf(msg, fmt, argptr);
	va_end(argptr);

	FNT_PrintPosOff(xl, yl, ox, oy, msg);
}

// y in pixels, x_s, x_w in pixels!
void FNT_PrintC(int y, int x_s, int x_w, char *msg)
{
	int w;

	w=unpackshort1(FNT_GetSize(msg));
	FNT_Print(x_s+(x_w-w)/2, y, msg);
}

// ------------------------- * Size Calculation * -------------------------
// returns width (x) in lword & height (y) in hword
unsigned long FNT_GetPos(int xl, int yl)
{
	return pack2shorts(xl*curFNT->width[' ']*sFNT.numerator/sFNT.divisor, yl*curFNT->height*sFNT.numerator/sFNT.divisor);
}

unsigned long FNT_GetSize(char *text)
{
	int h=0, w=0, mw=0, sh, sw;
	unsigned char c;

	sh=curFNT->height*sFNT.numerator/sFNT.divisor; // scaled height

	h=sh;
	while(c=*text++)
	{
		if(c=='\n')
		{
			if(w>mw) mw=w;
			w=0;
			h+=sh;
		}
		else
		{
			sw=(sFNT.numerator!=1 || sFNT.divisor!=1) ? curFNT->width[c]*sFNT.numerator/sFNT.divisor : curFNT->width[c];
			w+=sw;
		}
	}
	if(w>mw) mw=w;
	return pack2shorts(mw, h);
}
