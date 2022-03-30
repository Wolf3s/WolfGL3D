/*************************************************\
* Trighonomethry & math functions...              *
\*************************************************/
#include <windows.h>
#include <stdio.h>
#include <math.h>
#include "WolfDef.h"

double	TanTable[ANG_360+1], 
				CosTable[ANG_360+1], 
				SinTable[ANG_360+1], 
				XnextTable[ANG_360+1], 
				YnextTable[ANG_360+1];
int ColumnAngle[XRES]; // ViewAngle=PlayerAngle+ColumnAngle[curcolumn]; ! in fines!

char dx4dir[5]={1, 0, -1,  0, 0};  // dx & dy based on direction
char dy4dir[5]={0, 1,  0, -1, 0};
char dx8dir[9]={1, 1, 0, -1, -1, -1,  0,  1, 0};  // dx & dy based on direction
char dy8dir[9]={0, 1, 1,  1,  0, -1, -1, -1, 0};
dir4type opposite4[5]={2, 3, 0, 1, 4};
dir8type opposite8[9]={4, 5, 6, 7, 0, 1, 2, 3, 8};
dir8type dir4to8[5]={0, 2, 4, 6, 8};
dir8type diagonal[9][9]=
{
/* east */	{dir8_nodir, dir8_nodir, dir8_northeast, dir8_nodir, dir8_nodir, dir8_nodir, dir8_southeast, dir8_nodir, dir8_nodir},
			{dir8_nodir, dir8_nodir, dir8_nodir, dir8_nodir, dir8_nodir, dir8_nodir, dir8_nodir, dir8_nodir, dir8_nodir},
/* north */ {dir8_northeast, dir8_nodir, dir8_nodir, dir8_nodir, dir8_northwest, dir8_nodir, dir8_nodir, dir8_nodir, dir8_nodir},
			{dir8_nodir, dir8_nodir, dir8_nodir, dir8_nodir, dir8_nodir, dir8_nodir, dir8_nodir, dir8_nodir, dir8_nodir},
/* west */  {dir8_nodir, dir8_nodir, dir8_northwest, dir8_nodir, dir8_nodir, dir8_nodir, dir8_southwest, dir8_nodir, dir8_nodir},
			{dir8_nodir, dir8_nodir, dir8_nodir, dir8_nodir, dir8_nodir, dir8_nodir, dir8_nodir, dir8_nodir, dir8_nodir},
/* south */ {dir8_southeast, dir8_nodir, dir8_nodir, dir8_nodir, dir8_southwest, dir8_nodir, dir8_nodir, dir8_nodir, dir8_nodir},
			{dir8_nodir, dir8_nodir, dir8_nodir, dir8_nodir, dir8_nodir, dir8_nodir, dir8_nodir, dir8_nodir, dir8_nodir},
			{dir8_nodir, dir8_nodir, dir8_nodir, dir8_nodir, dir8_nodir, dir8_nodir, dir8_nodir, dir8_nodir, dir8_nodir}
};
// dir of delta tooks dx{-1|0|1}+1 & dy{-1|0|1}+1 and give direction
dir4type dir4d[3][3]={{dir4_nodir, dir4_west , dir4_nodir},
											{dir4_south, dir4_nodir, dir4_north},
											{dir4_nodir, dir4_east , dir4_nodir}};
int dir8angle[9]={ANG_0, ANG_45, ANG_90, ANG_135, ANG_180, ANG_225, ANG_270, ANG_315, ANG_0};
int dir4angle[5]={ANG_0, ANG_90, ANG_180, ANG_270, ANG_0};
// ------------------------- * Devider * -------------------------
int Math_Init(void)
{
	int i;
	double angle, tanfov2, tanval;

	for(i=0; i<=ANG_360; i++)
	{
		angle=FINE2RAD(i); //angle is in radians, i is in FINEs

		CosTable[i]=cos(angle);
		SinTable[i]=sin(angle);
		if(i==ANG_90 || i==ANG_270)
		{
			TanTable[i]=tan(FINE2RAD(i-0.5));	// infinity
			YnextTable[i]=FLOATTILE*tan(FINE2RAD(i-0.5)); // infinity
		}
		else
		{
			TanTable[i]=tan(angle);
			YnextTable[i]=FLOATTILE*tan(angle);
		}

		if(i==ANG_0 || i==ANG_360)
		{
			XnextTable[i]=FLOATTILE/tan(FINE2RAD(i+0.5));// infinity
		}
		else if(i==ANG_180)
		{
			XnextTable[i]=FLOATTILE/tan(FINE2RAD(i-0.5));// -infinity
		}
		else if(i==ANG_90 || i==ANG_270)
		{
			XnextTable[i]=0;
		}
		else
		{
			XnextTable[i]=FLOATTILE/tan(angle);
		}
	}

	tanfov2=TanDgr(CalcFov(FOV, XRES, YRES)/2.0)*((float)XRES/(float)YRES);
	for(i=0; i<XRES; i++)
	{
		tanval=tanfov2*(-1.0+2.0*(double)i/(double)(XRES-1) );
		ColumnAngle[i]=(int)RAD2FINE(atan(tanval));
	}

	Con_Printf("Math_Init: Done!\n");
	return 1;
}
// ------------------------- * Degree Tighonomethry * -------------------------
double TanDgr(double x) {return tan(DEG2RAD(x));}
double SinDgr(double x)	{return sin(DEG2RAD(x));}
double CosDgr(double x) {return cos(DEG2RAD(x));}
double ArcTanDgr(double x) {return RAD2DEG(atan(x));}
double ArcSinDgr(double x) {return RAD2DEG(asin(x));}
double ArcCosDgr(double x) {return RAD2DEG(acos(x));}

// ------------------------- * Angle Suff * -------------------------
int AngDiff(int alpha, int beta)
{
/*
	Function finds absolute difference between 2 angels (alpha & betta)
	By the shortest distance
	Assume angels are normalized [0..360]
*/
	int diff;

	diff=abs(alpha-beta);
	return diff<=ANG_180 ? diff : ANG_360-diff;
}

int WiseAngDiff(int alpha, int beta)
{
/*
	Function finds counterclockwise difference between 2 angels (alpha & betta)
	Angles can be not normalized, but result is normalized
*/
	return NormalizeAngle(beta-alpha);
}

int NormalizeAngle(int alpha)
{
	if(alpha>ANG_360) alpha%=ANG_360;
	if(alpha<ANG_0)		alpha=ANG_360-(-alpha)%ANG_360;
	return alpha;
}

int NormalizeVAngle(int beta)
{
	if(beta>ANG_22_5)  beta=ANG_22_5;
	if(beta<-ANG_22_5) beta=-ANG_22_5;
	return beta;
}

/*
====================
CalcFov (from Quake sources). Returns y_fov based on x_fov & width, height of window
====================
*/
float CalcFov(float fov_x, float width, float height)
{
  double a, x;

  if(fov_x<1 || fov_x>179) return 0;
  x=width/tan(DEG2RAD(fov_x)/2.0);
  a=atan(height/x);
  a=RAD2DEG(a)*2.0;
  return (float)a;
}

quadrant GetQuadrant(int angle)
{
	angle=NormalizeAngle(angle);

	if(angle<ANG_90) return q_first;
	else if(angle<ANG_180) return q_second;
	else if(angle<ANG_270) return q_third;
	else return q_fourth;
}

dir4type Get4dir(int angle)
{
	angle=NormalizeAngle(angle+ANG_45);

	if(angle<ANG_90) return dir4_east;
	else if(angle<ANG_180) return dir4_north;
	else if(angle<ANG_270) return dir4_west;
	else return dir4_south;
}

dir8type Get8dir(int angle)
{
	angle=NormalizeAngle(angle+ANG_22_5);

	     if(angle<=ANG_45 ) return dir8_east;
	else if(angle< ANG_90 ) return dir8_northeast;
	else if(angle<=ANG_135)	return dir8_north;
	else if(angle< ANG_180) return dir8_northwest;
	else if(angle<=ANG_225)	return dir8_west;
	else if(angle< ANG_270)	return dir8_southwest;
	else if(angle<=ANG_315)	return dir8_south;
	else return dir8_southeast;
}

// ------------------------- * Distances & angles * -------------------------

// advDist
// calculates a shortest distance from object to the line,
// which goes through point & is perpendicular to object viewline
int advDist(placeonplane obj, vec2_t point)
{
	double xt, yt;

	xt=(point[0]-obj.origin[0])*CosTable[obj.angle];
	yt=(point[1]-obj.origin[1])*SinTable[obj.angle];
	return (int)(xt+yt);
}

// smpDist
// returns a simple distance between 2 points on the plane
int smpDist(vec2_t point1, vec2_t point2)
{
	int tmp;

	tmp=(point1[0]-point2[0])*(point1[0]-point2[0])+(point1[1]-point2[1])*(point1[1]-point2[1]);
	return (int)sqrt(tmp);
}

/*
	Function returns angle in !UNITS!
            point2={x,y}
          / |
        /	  |	
 		  /     |
		/a______|----------> x
	point1={x, y}
*/
int advAng(vec2_t point1, vec2_t point2)
{
	int angle;

	angle=(int)RAD2FINE(atan2(point2[1]-point1[1], point2[0]-point1[0]));
	return NormalizeAngle(angle);
}

// ------------------------- * Subs that use above functions FIXME remove this (Player) * -------------------------
double AdvCalcDist(int x, int y)
{
	placeonplane obj;
	vec2_t point;
	
	obj.angle=Player.angle;
	obj.origin[0]=Player.x;
	obj.origin[1]=Player.y;
	point[0]=x;
	point[1]=y;
	
	return advDist(obj, point);
}

int TransformPoint(int x, int y)
{
	vec2_t point1, point2;

	point1[0]=Player.x;
	point1[1]=Player.y;
	point2[0]=x;
	point2[1]=y;

	return advAng(point1, point2);
}

/*
	Function returns integer distance between Player & point on the one plane
*/
int intDistance(int x, int y)
{
	vec2_t point1, point2;

	point1[0]=Player.x;
	point1[1]=Player.y;
	point2[0]=x;
	point2[1]=y;

	return smpDist(point1, point2);
}

// ------------------------- * Devider * -------------------------
/*
===================
GreatestCommonDivisor
====================
*/
int GreatestCommonDivisor(int i1, int i2)
{
	if(i1>i2)
	{
		if(i2==0) return i1;
		return GreatestCommonDivisor(i2, i1%i2);
	}
	else
	{
		if(i1==0)	return i2;
		return GreatestCommonDivisor(i1, i2%i1);
	}
}

// ------------------------- * Texture Resampling * -------------------------
/*
================
ResampleTexture (modified form Quake SRC's FIXME: make my own (GIMP source ???)
================
*/
void ResampleTexture(unsigned char *in, int inwidth, int inheight, unsigned char *out,  int outwidth, int outheight, int bits)
{
	int i, j, n;
	unsigned char *inrow;
	unsigned frac, fracstep;

	fracstep=inwidth*0x10000/outwidth;
	for(i=0; i<outheight; i++, out+=outwidth*bits)
	{
		inrow=in+bits*inwidth*(i*inheight/outheight);
		frac=fracstep>>1;
		for(j=0; j<outwidth; j++)
		{
			for(n=0; n<bits; n++)
				out[j*bits+n]=inrow[(frac>>16)*bits+n];
			frac+=fracstep;
		}
	}
}

// ------------------------- * CRC * -------------------------
// ------------------- * Code from Quake * -------------------
// this is a 16 bit, non-reflected CRC using the polynomial 0x1021
// and the initial and final xor values shown below...  in other words, the
// CCITT standard CRC used by XMODEM
#define CRC_INIT_VALUE	0xffff
#define CRC_XOR_VALUE		0x0000

static unsigned short crctable[256]=
{
	0x0000,	0x1021,	0x2042,	0x3063,	0x4084,	0x50a5,	0x60c6,	0x70e7,
	0x8108,	0x9129,	0xa14a,	0xb16b,	0xc18c,	0xd1ad,	0xe1ce,	0xf1ef,
	0x1231,	0x0210,	0x3273,	0x2252,	0x52b5,	0x4294,	0x72f7,	0x62d6,
	0x9339,	0x8318,	0xb37b,	0xa35a,	0xd3bd,	0xc39c,	0xf3ff,	0xe3de,
	0x2462,	0x3443,	0x0420,	0x1401,	0x64e6,	0x74c7,	0x44a4,	0x5485,
	0xa56a,	0xb54b,	0x8528,	0x9509,	0xe5ee,	0xf5cf,	0xc5ac,	0xd58d,
	0x3653,	0x2672,	0x1611,	0x0630,	0x76d7,	0x66f6,	0x5695,	0x46b4,
	0xb75b,	0xa77a,	0x9719,	0x8738,	0xf7df,	0xe7fe,	0xd79d,	0xc7bc,
	0x48c4,	0x58e5,	0x6886,	0x78a7,	0x0840,	0x1861,	0x2802,	0x3823,
	0xc9cc,	0xd9ed,	0xe98e,	0xf9af,	0x8948,	0x9969,	0xa90a,	0xb92b,
	0x5af5,	0x4ad4,	0x7ab7,	0x6a96,	0x1a71,	0x0a50,	0x3a33,	0x2a12,
	0xdbfd,	0xcbdc,	0xfbbf,	0xeb9e,	0x9b79,	0x8b58,	0xbb3b,	0xab1a,
	0x6ca6,	0x7c87,	0x4ce4,	0x5cc5,	0x2c22,	0x3c03,	0x0c60,	0x1c41,
	0xedae,	0xfd8f,	0xcdec,	0xddcd,	0xad2a,	0xbd0b,	0x8d68,	0x9d49,
	0x7e97,	0x6eb6,	0x5ed5,	0x4ef4,	0x3e13,	0x2e32,	0x1e51,	0x0e70,
	0xff9f,	0xefbe,	0xdfdd,	0xcffc,	0xbf1b,	0xaf3a,	0x9f59,	0x8f78,
	0x9188,	0x81a9,	0xb1ca,	0xa1eb,	0xd10c,	0xc12d,	0xf14e,	0xe16f,
	0x1080,	0x00a1,	0x30c2,	0x20e3,	0x5004,	0x4025,	0x7046,	0x6067,
	0x83b9,	0x9398,	0xa3fb,	0xb3da,	0xc33d,	0xd31c,	0xe37f,	0xf35e,
	0x02b1,	0x1290,	0x22f3,	0x32d2,	0x4235,	0x5214,	0x6277,	0x7256,
	0xb5ea,	0xa5cb,	0x95a8,	0x8589,	0xf56e,	0xe54f,	0xd52c,	0xc50d,
	0x34e2,	0x24c3,	0x14a0,	0x0481,	0x7466,	0x6447,	0x5424,	0x4405,
	0xa7db,	0xb7fa,	0x8799,	0x97b8,	0xe75f,	0xf77e,	0xc71d,	0xd73c,
	0x26d3,	0x36f2,	0x0691,	0x16b0,	0x6657,	0x7676,	0x4615,	0x5634,
	0xd94c,	0xc96d,	0xf90e,	0xe92f,	0x99c8,	0x89e9,	0xb98a,	0xa9ab,
	0x5844,	0x4865,	0x7806,	0x6827,	0x18c0,	0x08e1,	0x3882,	0x28a3,
	0xcb7d,	0xdb5c,	0xeb3f,	0xfb1e,	0x8bf9,	0x9bd8,	0xabbb,	0xbb9a,
	0x4a75,	0x5a54,	0x6a37,	0x7a16,	0x0af1,	0x1ad0,	0x2ab3,	0x3a92,
	0xfd2e,	0xed0f,	0xdd6c,	0xcd4d,	0xbdaa,	0xad8b,	0x9de8,	0x8dc9,
	0x7c26,	0x6c07,	0x5c64,	0x4c45,	0x3ca2,	0x2c83,	0x1ce0,	0x0cc1,
	0xef1f,	0xff3e,	0xcf5d,	0xdf7c,	0xaf9b,	0xbfba,	0x8fd9,	0x9ff8,
	0x6e17,	0x7e36,	0x4e55,	0x5e74,	0x2e93,	0x3eb2,	0x0ed1,	0x1ef0
};

void CRC_Init(unsigned short *crcvalue)
{
	*crcvalue=CRC_INIT_VALUE;
}

void CRC_ProcessByte(unsigned short *crcvalue, byte data)
{
	*crcvalue=(*crcvalue<<8)^crctable[(*crcvalue>>8)^data];
}

unsigned short CRC_Value(unsigned short crcvalue)
{
	return crcvalue^CRC_XOR_VALUE;
}

unsigned short CRC_Block(byte *start, int count)
{
	unsigned short crc;

	CRC_Init(&crc);
	while(count--)
		CRC_ProcessByte(&crc, *start++);

	return CRC_Value(crc);
}

// ------------------------- * Random Numbers * -------------------------
int US_RndT(void)
{
	return rand()&255; // same as %256
}