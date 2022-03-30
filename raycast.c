#include <windows.h>
#include <stdio.h>
#include "WolfDef.h"


// can player see this tile?
unsigned char spotvis[64][64];
// draw floor here?
unsigned char floorvis[64][64];
//values of Distance Squaress added by Idot
unsigned int DistanceSquares[64][64];

int skip_angle;

// checkded Xtile, Ytile
unsigned char x_ch[64][64], y_ch[64][64];
char drawnXpw=0, drawnYpw=0;
long lightcount=0;
int Xmap_Pos, Ymap_Pos, Xmap_Pos_Old, Ymap_Pos_Old, Angle_Old;//Moved by Idot
unsigned char x_ch_mem[64][64], y_ch_mem[64][64];//Added by Idot, memorizing checked
unsigned char x_ch_vis[64][64], y_ch_vis[64][64];//Added by Idot, visible part of memorized checked


int GetDistanceSquare(int x,int y){//Made by Idot
	int x_d,y_d,d;

	//Xmap_Pos=(int)abs(Player.x/FLOATTILE);//POS2TILE(Player.x));
	//Ymap_Pos=(int)abs(Player.y/FLOATTILE);//POS2TILE(Player.y));
	x_d=Xmap_Pos-x;
	y_d=Ymap_Pos-y;
	d=x_d*x_d+y_d*y_d;
	return d;
}

extern void CheckDistanceSquares(){//optimize
	int x,y;
	for(x=0;x<64;x++)for(y=0;y<64;y++)DistanceSquares[x][y]=GetDistanceSquare(x,y);
}


// ------------------------- * Devider * -------------------------

/*
** Here we will check for walls
**
** return :
**  1 - exit prev sub
**  0 - continue
*/
int CheckXwall(int x, int y, int angle)//modified by Idot
{
	if(!(CurMapData.tile_info[x][y] & (TILE_IS_WALL | TILE_IS_DOOR))) return 0; // continue
	
	if(x_ch[x][y]) // was here?
	{
// check if it is a door & it is not closed continue tracing ray
		if(CurMapData.tile_info[x][y]&TILE_IS_DOOR)
			if(DoorMap[x][y]->action!=dr_closed) return 0; // continue tracing
		return 1; // end of a ray
	}
	
	x_ch[x][y]=1; // was here!
	
	if(CurMapData.tile_info[x][y]&TILE_IS_DOOR)
	{
		if(DoorMap[x][y]->action==dr_open) return 0; // just continue!
		switch(DoorMap[x][y]->type)
		{
		case DOOR_VERT:
			Draw_X_Door(x, y, TEX_DDOOR);
			break;
		case DOOR_E_VERT:
			Draw_X_Door(x, y, TEX_DELEV);
			break;
		case DOOR_S_VERT:
		case DOOR_G_VERT:
			Draw_X_Door(x, y, TEX_DLOCK);
			break;
		}
		if(DoorMap[x][y]->action==dr_closed)
		{ // closed door
			Draw_Y_Wall(x, y, TEX_PLATE, 1);
			Draw_Y_Wall(x, y+1, TEX_PLATE, 0);
			floorvis[x][y]=1; // hack!
			return 1;
		}
		return 0;
	}

	if(angle<ANG_90 || angle >ANG_270)
	{
		if(CurMapData.tile_info[x-1][y]&TILE_IS_DOOR)
			if(!DoorMap[x-1][y]->vertical)
			{
				Draw_X_Wall(x, y, TEX_PLATE, 0);
				return 1;
			}

		if(PWall.active && PWall.x==x && PWall.y==y)
		{
			if(!drawnXpw)
			{
				Draw_X_Pwall(x, y, PWall.tex, 0);
				drawnXpw=1;
			}
			x_ch[x][y]=0; // fix
			return 0;
		}

		Draw_X_Wall(x, y, CurMapData.wall_tex[x][y], 0);
		return 1;
	}
	else
	{
		if(CurMapData.tile_info[x+1][y]&TILE_IS_DOOR)
			if(!DoorMap[x+1][y]->vertical)
			{
				Draw_X_Wall(x+1, y, TEX_PLATE, 1);
				return 1;
			}

		if(PWall.active && PWall.x==x && PWall.y==y)
		{
			if(!drawnXpw)
			{
				Draw_X_Pwall(x+1, y, PWall.tex, 1);
				drawnXpw=1;
			}
			x_ch[x][y]=0; // fix
			return 0;
		}

		Draw_X_Wall(x+1, y, CurMapData.wall_tex[x][y], 1);
		return 1;
	}
	Msg_Printf("Falled through all Xcheck!");
	return 0; // just in case !

/*
	if(!(CurMapData.tile_info[x][y] & (TILE_IS_WALL | TILE_IS_DOOR))) return 0; // continue
	
	if(x_ch[x][y]) // was here?
	{
// check if it is a door & it is not closed continue tracing ray
		if(CurMapData.tile_info[x][y]&TILE_IS_DOOR)
			if(DoorMap[x][y]->action!=dr_closed) return 0; // continue tracing
		return 1; // end of a ray
	}
	
	x_ch[x][y]=1; // was here!
	
	if(CurMapData.tile_info[x][y]&TILE_IS_DOOR)
	{
		if(DoorMap[x][y]->action==dr_open) return 0; // just continue!
		/*switch(DoorMap[x][y]->type)
		{
		case DOOR_VERT:
			Draw_X_Door(x, y, TEX_DDOOR);
			break;
		case DOOR_E_VERT:
			Draw_X_Door(x, y, TEX_DELEV);
			break;
		case DOOR_S_VERT:
		case DOOR_G_VERT:
			Draw_X_Door(x, y, TEX_DLOCK);
			break;
		}*/
	/*
		if(DoorMap[x][y]->action==dr_closed)
		{ // closed door
			/*Draw_Y_Wall(x, y, TEX_PLATE, 1);
			Draw_Y_Wall(x, y+1, TEX_PLATE, 0);*/
	/*		floorvis[x][y]=1; // hack!
			return 1;
		}
		return 0;
	}

	if(angle<ANG_90 || angle >ANG_270)
	{
		if(CurMapData.tile_info[x-1][y]&TILE_IS_DOOR)
			if(!DoorMap[x-1][y]->vertical)
			{
				//Draw_X_Wall(x, y, TEX_PLATE, 0);
				return 1;
			}

		if(PWall.active && PWall.x==x && PWall.y==y)
		{
			if(!drawnXpw)
			{
				//Draw_X_Pwall(x, y, PWall.tex, 0);
				//drawnXpw=1;
			}
			x_ch[x][y]=0; // fix
			return 0;
		}
		//Draw_X_Wall(x, y, CurMapData.wall_tex[x][y], 0);
		return 1;
	}
	else
	{
		if(CurMapData.tile_info[x+1][y]&TILE_IS_DOOR)
			if(!DoorMap[x+1][y]->vertical)
			{
				//Draw_X_Wall(x+1, y, TEX_PLATE, 1);
				return 1;
			}

		if(PWall.active && PWall.x==x && PWall.y==y)
		{
			if(!drawnXpw)
			{
				//Draw_X_Pwall(x+1, y, PWall.tex, 1);
				//drawnXpw=1;
			}
			x_ch[x][y]=0; // fix
			return 0;
		}

		//Draw_X_Wall(x+1, y, CurMapData.wall_tex[x][y], 1);
		return 1;
	}
	Msg_Printf("Falled through all Xcheck!");
	return 0; // just in case !
	*/
}

DrawXwall_(int x, int y, int angle)//made from CheckXwall by Idot
{
	if(!(CurMapData.tile_info[x][y] & (TILE_IS_WALL | TILE_IS_DOOR))) return 0; // continue
	
	if(x_ch_mem[x][y]) // was here?
	{
// check if it is a door & it is not closed continue tracing ray
		if(CurMapData.tile_info[x][y]&TILE_IS_DOOR)
			if(DoorMap[x][y]->action!=dr_closed) return 0; // continue tracing
		return 1; // end of a ray
	}
	
	x_ch[x][y]=1; // was here!
	if (!x_ch_mem[x][y]) return;
	
	if(CurMapData.tile_info[x][y]&TILE_IS_DOOR)
	{
		if(DoorMap[x][y]->action==dr_open) return 0; // just continue!
		switch(DoorMap[x][y]->type)
		{
		case DOOR_VERT:
			Draw_X_Door(x, y, TEX_DDOOR);
			break;
		case DOOR_E_VERT:
			Draw_X_Door(x, y, TEX_DELEV);
			break;
		case DOOR_S_VERT:
		case DOOR_G_VERT:
			Draw_X_Door(x, y, TEX_DLOCK);
			break;
		}
		if(DoorMap[x][y]->action==dr_closed)
		{ // closed door
			Draw_Y_Wall(x, y, TEX_PLATE, 1);
			Draw_Y_Wall(x, y+1, TEX_PLATE, 0);
			floorvis[x][y]=1; // hack!
			return 1;
		}
		return 0;
	}

	if(angle<ANG_90 || angle >ANG_270)
	{
		if(CurMapData.tile_info[x-1][y]&TILE_IS_DOOR)
			if(!DoorMap[x-1][y]->vertical)
			{
				Draw_X_Wall(x, y, TEX_PLATE, 0);
			if ((TEX_PLATE==8)||(TEX_PLATE==12)) return 0;//Cell => Continue checking
				return 1;
			}

		if(PWall.active && PWall.x==x && PWall.y==y)
		{
			if(!drawnXpw)
			{
				Draw_X_Pwall(x, y, PWall.tex, 0);
				drawnXpw=1;
			}
			x_ch[x][y]=0; // fix
			return 0;
		}
		Draw_X_Wall(x, y, CurMapData.wall_tex[x][y], 0);
		if ((CurMapData.wall_tex[x][y]==8)||(CurMapData.wall_tex[x][y]==12)) return 0;//Cell => Continue checking
		return 1;
	}
	else
	{
		if(CurMapData.tile_info[x+1][y]&TILE_IS_DOOR)
			if(!DoorMap[x+1][y]->vertical)
			{
				Draw_X_Wall(x+1, y, TEX_PLATE, 1);
			if ((TEX_PLATE==8)||(TEX_PLATE==12)) return 0;//Cell => Continue checking
				return 1;
			}

		if(PWall.active && PWall.x==x && PWall.y==y)
		{
			if(!drawnXpw)
			{
				Draw_X_Pwall(x+1, y, PWall.tex, 1);
				drawnXpw=1;
			}
			x_ch[x][y]=0; // fix
			return 0;
		}

		Draw_X_Wall(x+1, y, CurMapData.wall_tex[x][y], 1);
		if ((CurMapData.wall_tex[x][y]==8)||(CurMapData.wall_tex[x][y]==12)) return 0;//Cell => Continue checking
		return 1;
	}
	Msg_Printf("Falled through all Xcheck!");
	return 0; // just in case !
}

/*
** Here we will check for walls
**
** return :
**  1 - exit prev sub
**  0 - continue
*/
int checklight(int x, int y, int angle)
{
/*	int xx, yy, yymap;
	GLfloat lightPosition[4];
    GLfloat lightAmbient2[]  = {1.f, 1.0f, 1.0f, 0.01f} ;
    GLfloat lightDiffuse[]  = {1.f, 1.0f, 1.0f, 0.01f} ;
    GLfloat lightSpecular[] = {1.f, 1.0f, 1.0f, 0.01f} ;
	GLfloat light2Direction[] = {0.0f, -2.0f, 0.0f, 1.0f} ;

	//if(!(CurMapData.tile_info[x][y] & TILE_IS_DRESS)) return 0;
	lightcount=0;
	for(xx=0; xx<64; xx++)
	for(yy=0, yymap=63; yy<64; yy++, yymap--)
	{
	if(CurMapData.tile_info[xx][yymap]&TILE_IS_DRESS) 
	{
	lightcount++;
	lightPosition[0] = (float)xx;
	lightPosition[1] = (UPPERZCOORD-LOWERZCOORD)/2;
	lightPosition[2] = (float)yy;
	lightPosition[3] = 1.0f;
	glLightfv(GL_LIGHT0+lightcount, GL_SPOT_DIRECTION, light2Direction) ; 
	//glLightfv(GL_LIGHT0+lightcount, GL_AMBIENT,lightAmbient2);
    glLightf(GL_LIGHT0+lightcount, GL_SPOT_EXPONENT, 64.0f);
	glLightf(GL_LIGHT2, GL_SPOT_CUTOFF, 180.0f) ;
    glLightfv(GL_LIGHT0+lightcount, GL_DIFFUSE,lightDiffuse);
    //glLightfv(GL_LIGHT0+lightcount, GL_SPECULAR,lightSpecular);
	glLightfv(GL_LIGHT0+lightcount, GL_POSITION, lightPosition);
	glEnable(GL_LIGHT0+lightcount);
	}}*/
return 0;//added by Iodt to avoid warning C4716
}
int CheckYwall(int x, int y, int angle)
{
	if(!(CurMapData.tile_info[x][y] & (TILE_IS_WALL | TILE_IS_DOOR))) return 0; // continue

	if(y_ch[x][y]) // was here?
	{
// check if it is a door & it is not closed continue tracing ray
		if(CurMapData.tile_info[x][y]&TILE_IS_DOOR)
			if(DoorMap[x][y]->action!=dr_closed) return 0; // continue tracing
		return 1; // end of a ray
	}

	y_ch[x][y]=1; // was here!
	
	if(CurMapData.tile_info[x][y]&TILE_IS_DOOR)
	{
		if(DoorMap[x][y]->action==dr_open) return 0; // just continue!
		switch(DoorMap[x][y]->type)
		{
		case DOOR_HORIZ:
			Draw_Y_Door(x, y, TEX_DDOOR);
			break;
		case DOOR_E_HORIZ:
			Draw_Y_Door(x, y, TEX_DELEV);
			break;
		case DOOR_S_HORIZ:
		case DOOR_G_HORIZ:
			Draw_Y_Door(x, y, TEX_DLOCK);
			break;
		}
		if(DoorMap[x][y]->action==dr_closed)
		{ // closed door
			Draw_X_Wall(x, y, TEX_PLATE, 1);
			Draw_X_Wall(x+1, y, TEX_PLATE, 0);
			floorvis[x][y]=1; // hack!
			return 1;
		}
		return 0;
	}

	if(angle<ANG_180)
	{
		if(CurMapData.tile_info[x][y-1]&TILE_IS_DOOR)
			if(DoorMap[x][y-1]->vertical)
			{
				Draw_Y_Wall(x, y, TEX_PLATE, 0);
			if ((TEX_PLATE==8)||(TEX_PLATE==12)) return 0;//Cell => Continue checking
				return 1;
			}

		if(PWall.active && PWall.x==x && PWall.y==y)
		{
			if(!drawnYpw)
			{
				Draw_Y_Pwall(x, y, PWall.tex, 0);
				drawnYpw=1;
			}
			y_ch[x][y]=0; // fix
		//if ((TEX_PLATE==8)||(TEX_PLATE==12) return 0;//Cell => Continue checking
			return 0;
		}

		Draw_Y_Wall(x, y, CurMapData.wall_tex[x][y], 0);
		if ((CurMapData.wall_tex[x][y]==8)||(CurMapData.wall_tex[x][y]==12)) return 0;//Cell => Continue checking
		return 1;
	}
	else
	{
		if(CurMapData.tile_info[x][y+1]&TILE_IS_DOOR)
			if(DoorMap[x][y+1]->vertical)
			{
				Draw_Y_Wall(x, y+1, TEX_PLATE, 1);
			if ((TEX_PLATE==8)||(TEX_PLATE==12)) return 0;//Cell => Continue checking
				return 1;
			}

		if(PWall.active && PWall.x==x && PWall.y==y)
		{
			if(!drawnYpw)
			{
				Draw_Y_Pwall(x, y+1, PWall.tex, 1);
				drawnYpw=1;
			}
			y_ch[x][y]=0; // fix
		//if ((TEX_PLATE==8)||(TEX_PLATE==12) return 0;//Cell => Continue checking
		//if (PWall.tex==10) return 0;//Iron-Eagle => Continue checking
			return 0;
		}

		Draw_Y_Wall(x, y+1, CurMapData.wall_tex[x][y], 1);
		if ((CurMapData.wall_tex[x][y]==8)||(CurMapData.wall_tex[x][y]==12)) return 0;//Cell => Continue checking
		return 1;
	}
	Msg_Printf("Falled through all Ycheck!");
	return 0; // just in case !
}

// ------------------------- * RayCaster's Callback * -------------------------

/*
** RayCaster will call this function each intersection
**
** retrun 1 to stop tracing ray
**        0 to continue raycasting
*/
int R_RayCallback(int x, int y, int angle, int R_Ray_y)
{
	spotvis[x][y]=1;// set that this map area is visible
	AM_AutoMap.vis[x][y]=1;

	if(!R_Ray_y)
	{
		//checklight(x,y,angle);
		//if(CheckXwall(x, y, angle)) return 1;
		return CheckXwall(x, y, angle);
	}
	else
	{ 
		//checklight(x,y,angle);
		//if(CheckYwall(x, y, angle)) return 1;
		return CheckYwall(x, y, angle);
	}
	floorvis[x][y]=1;
	// no intersection, go on!
	return 0;
}
// ------------------------- * Actual Casting * -------------------------

/*
** Main raycasting function! Will
** render view (tell what to render)
** for given pos, angle, fov.
** may be called recursively
** to create mirror & portal effects!
** --------- Takes -------------
** x, y : position of the eye
** angle: direction of view in FINES
** fov  : field of view in FINES
** --------- Sets --------------
**
** Platform Independent. Please no
** gl- code here!
*/
void R_RayCast(int x, int y, int angle, int fov)
{
	int n,m;//,i,j;
	int skipN=0;//added by Idot
	int skip_N=0;//added by Idot

	memset(x_ch, 0, sizeof(x_ch));
	memset(y_ch, 0, sizeof(y_ch));
	drawnXpw=drawnYpw=0;
//	for (n=1;n<=lightcount;n++) glDisable(GL_LIGHT0+n);//added by Idot
//	for (i=0;i<64; i++)	for (j=0;j<64; j++)spotvis[i][j]=0;

	checklight(x,y,angle);
	//lightcount=0;
	Xmap_Pos=POS2TILE(Player.x);
	Ymap_Pos=POS2TILE(Player.y);
//for(n=0; n<XRES; n+=8) // let's cast some rays!//by Idot temporary

	n_skiped_=0;



/*	if ((Xmap_Pos!=Xmap_Pos_Old)||(Ymap_Pos!=Ymap_Pos_Old)){//cast rays again
		Xmap_Pos_Old=Xmap_Pos;
		Ymap_Pos_Old=Ymap_Pos;
		Angle_Old=Player.angle;
		for(n=0; n<XRES; n++) 
			R_CastRay(x, y, NormalizeAngle(angle+ColumnAngle[n]));
		return;
		}


	/*if (Angle_Old!=Player.angle){//////////////DO IT!!!!!!! Idot!
		for(n=0; n<XRES; n++) 
			if (Player.angle-Angle_Old)
				ColumnAngle[0]-ColumnAngle[n]
				n
			R_CastRay(x, y, NormalizeAngle(angle+ColumnAngle[n]));
		}*/
		
	Angle_Old=Player.angle;

	for(n=0; n<XRES; n++) // let's cast some rays!
	{
		if (skip_N){skip_N--;continue;};//skipping rays
		skipN=R_CastRay(x, y, angle, n);
		n_skiped_+=skipN;//total # of the skipped rays
		skip_N=skipN;//how many rays we should skip
	}
}


int x_tile_step[4]={1, -1, -1,  1};
int y_tile_step[4]={1,  1, -1, -1};

/*
** Function, which handles each
** ray individally, do not include
** checking for {walls|sprites}
** accepts normalized angle
*/
int R_CastRay(int x, int y, int angle, int n)
//int R_CastRay(int x, int y, int angle)//original
{
	int xtilestep, ytilestep;
	int xstep, ystep;
	int xtile, ytile;
	int xintercept, yintercept;
	int YmapPos, XmapPos;
	quadrant q;
	float t;//a buffer or temporary calculations

	int skipN=0;//adedd by Idot
	int entry=0;//adedd by Idot -1 vertcheck : 1 horizcheck
	int angleN=NormalizeAngle(angle+ColumnAngle[n+skipN]);//adedd by Idot
	//skipN=0;//adedd by Idot
	//entry=0;//adedd by Idot -1 vertcheck : 1 horizcheck
	//angleN=NormalizeAngle(angle+ColumnAngle[n+skipN]);//adedd by Idot

// Setup for raycasting
	q=GetQuadrant(angleN);
	xtilestep=x_tile_step[q];//x_tile_step[4]={1, -1, -1,  1};
	ytilestep=y_tile_step[q];//y_tile_step[4]={1,  1, -1, -1};
	xtile=Xmap_Pos+xtilestep;
	ytile=Ymap_Pos+ytilestep;
	xstep=(ytilestep>0) ? XnextTable[angleN] : -XnextTable[angleN];
	ystep=(xtilestep>0) ? YnextTable[angleN] : -YnextTable[angleN];
	xintercept=(int)( ( ((ytilestep==-1 ? ytile+1 : ytile)<<TILESHIFT)-y )/TanTable[angleN])+x;
	yintercept=(int)( ( ((xtilestep==-1 ? xtile+1 : xtile)<<TILESHIFT)-x )*TanTable[angleN])+y;


	//xtile=(x>>TILESHIFT)+xtilestep;
	//ytile=(y>>TILESHIFT)+ytilestep;
//	xstep=ytilestep*(int)XnextTable[angle];
//	ystep=xtilestep*(int)YnextTable[angle];
//	xintercept=(int)( ( ((ytilestep==-1 ? ytile+1 : ytile)<<TILESHIFT)-y )/(int)TanTable[angle])+x;
//	yintercept=(int)( ( ((xtilestep==-1 ? xtile+1 : xtile)<<TILESHIFT)-x )*(int)TanTable[angle])+y;


	   skip_angle=angle;

	YmapPos=yintercept>>TILESHIFT; // toXray
	XmapPos=xintercept>>TILESHIFT; // toYray
//	Ymap_Pos=ytile;
//	Xmap_Pos=xtile;
// Start of raycasting
vertcheck: // an anolugue for X-Ray
	if( (ytilestep==-1 && ( YmapPos <= ytile ) ) || (ytilestep==1 && ( YmapPos >= ytile ) ) )
	{// weird, but better than Carmack's self modifying stuff
		goto horizentry;
	}
vertentry:
// check if we are out of the map
	if(xtile<0 || xtile>=64) return 0;
	if(YmapPos<0 || YmapPos>=64) return 0;

// call check fuction!
//  if(R_RayCallback(xtile, YmapPos, angle, R_RAY_X)) return 1;
	if(R_RayCallback(xtile, YmapPos, angleN, R_RAY_X)){entry=-1;goto nextangle;}

// prepare for next step
	xtile+=xtilestep;
	yintercept+=ystep;
	YmapPos=yintercept>>TILESHIFT;
	goto vertcheck;

horizcheck: // an anolugue for Y-Ray
	if( (xtilestep==-1 && ( XmapPos <= xtile ) ) || (xtilestep==1 && ( XmapPos >= xtile ) ) )
	{// weird, but better than Carmack's self modifying stuff
		goto vertentry;
	}
horizentry:
// check if we are out of the map
	if(ytile<0 || ytile>=64) return 0;
	if(XmapPos<0 || XmapPos>=64) return 0;

// call check fuction!
//	if(R_RayCallback(XmapPos, ytile, angle, R_RAY_Y)) return 1;
	if(R_RayCallback(XmapPos, ytile, angleN, R_RAY_Y)){entry=1;goto nextangle;}

// prepare for next step
	ytile+=ytilestep;
	xintercept+=xstep;
	XmapPos=xintercept>>TILESHIFT;
	goto horizcheck;

	return 0;
nextangle://adedd by Idot
	//return 0;// TEMPORARY!!!!!!!!!!!!!!!!!!
	if (entry>0)//horizcheck
		t=((ytilestep==-1 ? ytile+1 : ytile)<<TILESHIFT)-y;
	if (entry<0)//verticheck
		t=((xtilestep==-1 ? xtile+1 : xtile)<<TILESHIFT)-x;
	skipN++;
	while(n+skipN<XRES){
		angleN=NormalizeAngle(angle+ColumnAngle[n+skipN]);
		if (q!=GetQuadrant(angleN))break;
		if (entry>0){//horizcheck
//			if(xintercept!=((int)(t/TanTable[angleN])+x)) break;
			if(XmapPos!=(((int)(t/TanTable[angleN])+x)>>TILESHIFT)) break;
			skipN++;}
		if (entry<0){//verticheck
//			if(yintercept!=((int)(t*TanTable[angleN])+y)) break;
			if(YmapPos!=(((int)(t*TanTable[angleN])+y)>>TILESHIFT)) break;
			skipN++;}
		if (entry==0)break;//just in case! ;-)
		}
	skipN--;
	return skipN;
}