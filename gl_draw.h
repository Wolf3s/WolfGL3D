////////////////!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//REMOVED FROM PTHE PROJECT










void Vid_DrawPicture(int x1, int y1, int x2, int y2, int Tex, 
					 int tx1, int ty1, int tx2, int ty2, int texsize)


void Vid_DumpScreen(unsigned char *buffer)

// Console Background!
static GLuint conback_tex;
static GLuint ifloortex;
static GLuint iceltex;


void Vid_LoadConBackTexture(void)

void Vid_UnLoadConBackTexture(void)


void Vid_DrawConBack(int height, unsigned char transp)

// Draws Solid Filled box
void Vid_DrawBox(int x, int y, int w, int h, RGBA_PARAM(1))


void Vid_WMenuBack(void)


void Vid_SMenuBack(void)


void Vid_WDrawStripes(void)


void Vid_SDrawStripes(void)

void Vid_MenuBack(void)


void Vid_DrawStripes(void)


void Vid_DrawWindow(int x, int y, int w, int h, RGBA_PARAM(1), RGBA_PARAM(2), RGBA_PARAM(3))


void Vid_DrawMenuBox(int x, int y, int w, int h)


void Vid_Message(char *msg)


void Vid_DrawPic(int x, int y, int name)


void Vid_DrawPicEx(int x, int y, int name)


void Vid_DrawPicIndirect(int x, int y, cache_pic *pic)

// ------------------------- * Drawing 3D world (OpenGL) * -------------------------
extern unsigned int nOfPoly;

/*   Draws an X wall:
	//               ________
	//              |        |
	//  X-wall ---> |        | <----X-wall
	//              |________|
	//
*/
void Draw_X_Wall(int X, int Y_start, int tex, float TexInv)


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


// Draws X PushWall
void Draw_X_Pwall(int X, int Y_start, int tex, float TexInv)


// Draws Y PushWall
void Draw_Y_Pwall(int X_start, int Y, int tex, float TexInv)

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


	n_sprt=Spr_CreateVisList();
	if(!n_sprt) return; // nothing to draw


	ang=NormalizeAngle(Player.angle+ANG_90);

	sina=(float)(0.5*SinTable[ang]);
	cosa=(float)(0.5*CosTable[ang]);
	Ex0= cosa; Ey0= sina;
	Dx0=-cosa; Dy0=-sina;


	for(n=0; n<n_sprt; n++)
	{
		if(vislist[n].dist<MINDIST/2) continue; // little hack to save speed & z-buffer
		nOfPoly++;
		Vid_SelectTextureEx(vislist[n].tex|TEX_SPRITE_BIT);
		glBegin(GL_QUADS);
			Ex=Ex0+vislist[n].x/FLOATTILE; 
			Ey=Ey0+vislist[n].y/FLOATTILE;
			Dx=Dx0+vislist[n].x/FLOATTILE;
			Dy=Dy0+vislist[n].y/FLOATTILE;
			glTexCoord2f(0.0, 0.0);	glVertex3f(Ex, UPPERZCOORD, -Ey);
			glTexCoord2f(1.0, 0.0);	glVertex3f(Dx, UPPERZCOORD, -Dy);
			glTexCoord2f(1.0, 1.0);	glVertex3f(Dx, LOWERZCOORD, -Dy);
			glTexCoord2f(0.0, 1.0);	glVertex3f(Ex, LOWERZCOORD, -Ey);
		glEnd();
	}
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

#ifdef TEXTUREDFLOORCEL
	glEnable(GL_TEXTURE_2D);	// textured
	glColor3ub(255, 255, 255);
#else
	glDisable(GL_TEXTURE_2D);	// solid
#endif

	for(x=0; x<64; x++)
	{
		for(y=0; y<64; y++)
		{
			if(floorvis[x][y])
			{
#ifdef TEXTUREDFLOORCEL
			
			Vid_SelectCel();
			glBegin(GL_QUADS);
				glTexCoord2f(0.0, 0.0);	glVertex3f((float)x  ,		UPPERZCOORD, (float)(-y));
				glTexCoord2f(1.0, 0.0);	glVertex3f((float)(x+1),	UPPERZCOORD, (float)(-y));
				glTexCoord2f(1.0, 1.0);	glVertex3f((float)(x+1),	UPPERZCOORD, (float)(-y-1));
				glTexCoord2f(0.0, 1.0);	glVertex3f((float)x,			UPPERZCOORD, (float)(-y-1));
			glEnd();
						
			Vid_SelectFloor();
			glBegin(GL_QUADS);
				glTexCoord2f(0.0, 0.0);	glVertex3f((float)x,			LOWERZCOORD, (float)(-y));
				glTexCoord2f(1.0, 0.0);	glVertex3f((float)(x+1),  LOWERZCOORD, (float)(-y));
				glTexCoord2f(1.0, 1.0);	glVertex3f((float)(x+1),	LOWERZCOORD, (float)(-y-1));
				glTexCoord2f(0.0, 1.0);	glVertex3f((float)x,			LOWERZCOORD, (float)(-y-1));
			glEnd();
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