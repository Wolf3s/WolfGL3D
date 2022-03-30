/*************************************\
  module, which displays
	Wolf 'Rich Text'
	no platform dependent code
	Relies on Picture & Text Functions!
\*************************************/
#include <windows.h>
#include <stdio.h>
#include "WolfDef.h"

/*
=============================================================================

TEXT FORMATTING COMMANDS
------------------------
;													Comment
^P[enter]									Start new page, must be the first chars in a layout
^E[enter]									End of layout (all pages)
^C<hex digit>							Change text color (index in Wolf Palette)
^B<y>,<x>,<w>,<h>[enter]	Draw a box (background color) [x, y]-[x+w, y+h]
^G<y>,<x>,<pic>[enter]		Draw a graphic on x, y and push margins (pic is index in vgagraph)
^T<y>,<x>,<pic>,<t>[enter]Same as above, but after <t> tics
^L<x>,<y>[ENTER]					Locate to a specific spot, x in pixels, y in lines
^>												Draw text on right half on screen


=============================================================================
*/

/*
=============================================================================

						 LOCAL CONSTANTS

=============================================================================
*/
#define BACKCOLOR		0x11

#define WORDLIMIT			80 // max word len
#define FONTHEIGHT		24 // font height ...
#define	TOPMARGIN			38 
#define BOTTOMMARGIN	77 
#define LEFTMARGIN		32
#define RIGHTMARGIN		32
#define PICMARGIN			16 // ???
#define TEXTROWS			((480-TOPMARGIN-BOTTOMMARGIN)/FONTHEIGHT)
#define	SPACEWIDTH		14
#define SCREENPIXWIDTH	640
#define SCREENMID				(SCREENPIXWIDTH/2)

/*
=============================================================================

						 LOCAL VARIABLES

=============================================================================
*/
char *txtsrc=NULL, *text; // pointer to the start of the text
int txtsize;				// size of article
int numpages;				// number of pages
// variables in use
int	pagenum, rowon;
int leftmargin[TEXTROWS], rightmargin[TEXTROWS];
int px, py, fontcolor; // print position & color!
// temporary variables
int	picx, picy, picnum, picdelay;
bool layoutdone;
//===========================================================================


// ------------------------- * AUX * -------------------------

// scan to end of line
void Text_RipToEOL(void)
{
	while(*text++!='\n');
}

// get number from stream
// it will use 0x###### (hex), '#' (char), #### (decimal), -### (negative)
// uses Quake Functions
int	Text_ParseNumber(void)
{
	int number;

// scan until a number is found (skip ',', ';', ' ' etc)
	while((text[0]<'0' || text[0]>'9'))// && text[0]!='0' && text[1]!='x' && text[0]!='-')
		text++;

// get number
	number=Q_atoi(text);

// scan past number
	while(text[0]>='0' && text[0]<='9') text++;
	
	return number;
}

//reads ## from the stream, where # is hex digit 0-F and converts it to decimal!
//text will point just after the ##
int Text_ParseHexNumber(void)
{
	int c, num=0;

	c=toupper(*++text);
	if(c>='0' && c<='9')
		num=c-'0';
	else if(c>='A' && c<='F')
		num=c-'A'+10;
	
	num*=16;
	c=toupper(*++text);
	if(c>='0' && c<='9')
		num+=c-'0';
	else if(c>='A' && c<='F')
		num+=c-'A'+10;
	text++;

	return num;
}

/*
=====================
= ParsePicCommand
=
= Call with text pointing just after a ^P
= Upon exit text points to the start of next line
=====================
*/
void Text_ParsePicCommand(void)
{
	picy=VID_NEW_Y(Text_ParseNumber());
	picx=VID_NEW_X(Text_ParseNumber());
	picnum=Text_ParseNumber();
	Text_RipToEOL();
}

void Text_ParseTimedCommand(void)
{
	picy=VID_NEW_Y(Text_ParseNumber());
	picx=VID_NEW_X(Text_ParseNumber());
	picnum=Text_ParseNumber();
	picdelay=Text_ParseNumber();
	Text_RipToEOL();
}

/*
=====================
= BackPage
=
= Scans for a previous ^P
=====================
*/
void Text_BackPage(void)
{
	pagenum--;
	do
	{
		text--;
		if(text[0]=='^' && toupper(text[1])=='P') return;
	} while(1);
}

/*
=====================
= NextPage
=
= Scans for a next ^P
=====================
*/
void Text_NextPage(void)
{
	pagenum++;
	do
	{
		text++;
		if(text[0]=='^' && toupper(text[1])=='P') return;
	} while(1);
}

/*
=====================
= NewLine
=====================
*/
void Text_NewLine(void)
{
	char ch;

	if(++rowon==TEXTROWS)
	{
// overflowed the page, so skip until next page break
		layoutdone=true;
		do
		{
			if(text[0]=='^')
			{
				ch=toupper(text[1]);
				if(ch=='E' || ch=='P')
				{
					layoutdone=true;
					return;
				}
			}
			text++;
		} while(1);
	}
	px =leftmargin[rowon];
	py+=FONTHEIGHT;
}

/*
=====================
= HandleCtrls
=====================
*/
void Text_HandleCtrls(void)
{
	char ch;

	ch=*text++;			// get the character and advance

	if(ch=='\n')
	{
		Text_NewLine();
		return;
	}
}


/*
=====================
= HandleCommand
=====================
*/
void Text_HandleCommand(void)
{
	int	i, margin, top, bottom;
	int	picwidth, picheight, picmid;
	cache_pic *pdata;

	switch(toupper(*++text))
	{
	case 'B':
		picy=VID_NEW_X(Text_ParseNumber());
		picx=VID_NEW_Y(Text_ParseNumber());
		picwidth=VID_NEW_X(Text_ParseNumber());
		picheight=VID_NEW_Y(Text_ParseNumber());
		Vid_DrawBox(picx, picy, picwidth, picheight, RGBA_READB);
		Text_RipToEOL();
		break;
	case ';':		// comment
		Text_RipToEOL();
		break;
	case 'P':		// ^P is start of next page, ^E is end of file
	case 'E':
		layoutdone=true;
		text--;		// back up to the '^'
		break;
	case 'C':		// ^c<hex digit> changes text color
		fontcolor=Text_ParseHexNumber();
		break;
	case '>':
		px=320;
		text++;
		break;
	case 'L':
		py=Text_ParseNumber();
		rowon=(py-TOPMARGIN)/FONTHEIGHT;
		py=TOPMARGIN+rowon*FONTHEIGHT;
		px=VID_NEW_X(Text_ParseNumber());
// scan to end of line
		Text_RipToEOL();
		while(*text++!='\n');
		break;
	case 'T':		// ^Tyyy,xxx,ppp,ttt waits ttt tics, then draws pic
		Text_ParseTimedCommand();
		goto draw_pic;
		break;
	case 'G':		// ^Gyyy,xxx,ppp draws graphic
		Text_ParsePicCommand();
draw_pic:
		Vid_DrawPicEx(picx&~15, picy, picnum);
		pdata=Vid_CachePicEx(picnum);
		picwidth =pdata->width;
		picheight=pdata->height;
// adjust margins
		picmid=picx+picwidth/2;
		if(picmid>SCREENMID)
			margin=picx-PICMARGIN;					// new right margin
		else
			margin=picx+picwidth+PICMARGIN;	// new left margin

		top=(picy-TOPMARGIN)/FONTHEIGHT;
		if(top<0) top=0;
		bottom=(picy+picheight-TOPMARGIN)/FONTHEIGHT;
		if(bottom>=TEXTROWS) bottom=TEXTROWS-1;

		for(i=top; i<=bottom; i++)
			if(picmid>SCREENMID)
				rightmargin[i]=margin;
			else
				leftmargin[i]=margin;

		// adjust this line if needed
		if(px<leftmargin[rowon])
			px=leftmargin[rowon];
		break;
	}
}

/*
=====================
= HandleWord
=====================
*/
void Text_HandleWord(void)
{
	char word[WORDLIMIT];
	int wwidth, wheight, newpos, wordindex;
	unsigned long tmp;

// copy the next word into [word]
	word[0]=*text++;
	wordindex=1;
	while(*text>32)
	{
		word[wordindex]=*text++;
		if(++wordindex==WORDLIMIT)
		{
			Con_Printf("PageLayout: Word limit exceeded\n");
			break;
		}
	}
	word[wordindex]=0;		// stick a null at end for C

	// see if it fits on this line
	tmp=FNT_GetSize(word);
	wwidth =unpackshort1(tmp);
	wheight=unpackshort2(tmp);

	while(px+wwidth>rightmargin[rowon])
	{
		Text_NewLine();
		if(layoutdone) return; // overflowed page
	}

	// print it
	newpos=px+wwidth;

	FNT_SetFont(FNT_WOLF_SMALL);
	FNT_SetStyle(0, 0, 0);
	FNT_SetScale(1, 1);
	FNT_SetColor(RGBA_WOLFCOLOR(fontcolor));
	FNT_Print(px, py, word);
	px=newpos;

	// suck up any extra spaces
	while(*text==' ')
	{
		px+=SPACEWIDTH;
		text++;
	}
}

// ------------------------- * Main * -------------------------
/*
=====================
= Scans an entire layout file (until a ^E) marking all graphics used, and
= counting pages, then caches the graphics in
=====================
*/
void Text_ScanArticle(int cache)
{
	int ch;

	numpages=0;

	do
	{
		if(*text=='^')
		{
			ch=toupper(*++text);
			if(ch=='P') numpages++;	// start of a page
			if(ch=='E')	// end of file, so load graphics and return
			{
				if(cache)
				{
					Vid_CachePic(H_TOPWINDOWPIC);
					Vid_CachePic(H_LEFTWINDOWPIC);
					Vid_CachePic(H_RIGHTWINDOWPIC);
					Vid_CachePic(H_BOTTOMINFOPIC);
				}
				else
				{
					Vid_UnCachePic(H_TOPWINDOWPIC);
					Vid_UnCachePic(H_LEFTWINDOWPIC);
					Vid_UnCachePic(H_RIGHTWINDOWPIC);
					Vid_UnCachePic(H_BOTTOMINFOPIC);
				}
				text=txtsrc;
				return;
			}
// Graphics commands: Cache Picture!
			if(ch=='G')		// draw graphic command
			{
				Text_ParsePicCommand();
				if(cache)
					Vid_CachePicEx(picnum);
				else
					Vid_UnCachePic(picnum);
			}
			if(ch=='T')		// timed draw graphic command
			{
				Text_ParseTimedCommand();
				if(cache)
					Vid_CachePic(picnum);
				else
					Vid_UnCachePic(picnum);
			}
		}
		else
			text++;
	} while((text-txtsrc)<txtsize);

	text=txtsrc;
	Con_Printf("Text_ScanArticle: No ^E to terminate file!\n");
}

// this function will be called to indicate that new help screen
// need to be displayed, so zero all old data & cache all data!
void Text_NewArticle(char *src, int size)
{
/* DEBUG
	FILE *fp;

	fp=fopen("e:\\rrr.txt", "wb");
	fwrite(src, 1, size, fp);
	fclose(fp);
*/
// malloc & copy text data
	if(txtsrc) free(txtsrc);
	txtsrc=malloc(size);
	memcpy(txtsrc, src, size);
	text=txtsrc; txtsize=size;
// Scan Article to cache Graph & count Pages
	Text_ScanArticle(1); // cache
// Init vars
	pagenum=1; fontcolor=0;
// Setup Font
	FNT_SetFont(FNT_WOLF_SMALL);
	FNT_SetStyle(0, 0, 0);
	FNT_SetScale(1, 1);
	FNT_SetColor(RGBA_BLACK);
}

// called there when user presses Esc!
void Text_FreeArticle(void)
{
	if(txtsrc)
	{
		Text_ScanArticle(0); // uncache
		free(txtsrc);
		txtsrc=NULL;
	}
}

// Will render 1 Page!
void Text_DisplayPage(void)
{
	char *pagestart, ch;
	int n;

	if(text==NULL) return;	// no page loaded
	pagestart=text; // save!

// clear the screen
	Vid_DrawBox(0, 0, 640, 480, RGBA_READB);
	Vid_DrawPic(0, 0, H_TOPWINDOWPIC);
	Vid_DrawPic(0, 19, H_LEFTWINDOWPIC);
	Vid_DrawPic(624, 19, H_RIGHTWINDOWPIC);
	Vid_DrawPic(16, 422, H_BOTTOMINFOPIC);

// init
	for(n=0; n<TEXTROWS; n++)
	{
		leftmargin[n]=LEFTMARGIN;
		rightmargin[n]=SCREENPIXWIDTH-RIGHTMARGIN;
	}
	rowon=0; layoutdone=false;
	px=LEFTMARGIN; py=TOPMARGIN;

// make sure we are starting layout text (^P first command)
	while(*text<=32) text++;

	if(*text!='^' || toupper(*++text)!='P')
	{
		// Exit Text Display!
		Con_Printf("PageLayout: Text not headed with ^P\n");
		return;
	}
	Text_RipToEOL();

// process text stream
	do
	{
		ch=*text;

		if(ch=='^')
			Text_HandleCommand();
		else if(ch==9)
		{
		 px=(px+16)&~15;
		 text++;
		}
		else if(ch<=32)
			Text_HandleCtrls();
		else
			Text_HandleWord();
	} while(!layoutdone);

// Page Numbers!
	FNT_SetFont(FNT_WOLF_SMALL);
	FNT_SetStyle(0, 0, 0);
	FNT_SetScale(1, 1);

	FNT_SetColor(0x40, 0x40, 0x00, 0xFF); // 0x4F
	FNT_Printf(426, 439, "pg %d of %d", pagenum, numpages);
	FNT_SetColor(RGBA_BLACK);
// End Drawing

	text=pagestart; // restore!
}

// recieves keycode, returns 1 if we are finished!
int Text_Input(int key)
{
	switch(key)
	{
	case K_ESCAPE: // Exit
		Text_FreeArticle();
		return 1;
		break;
	case K_UPARROW:	// Prev Page
	case K_PGUP:
	case K_LEFTARROW:
		if(pagenum>1)
			Text_BackPage();
		break;
	case K_ENTER: // Next Page
	case K_DOWNARROW:
	case K_PGDN:
	case K_RIGHTARROW:
		if(pagenum<numpages)
			Text_NextPage();
		break;
	}
	return 0;
}