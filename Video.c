// high level video
#include <Windows.h>
#include <stdio.h>
#include "WolfDef.h"

int Vid_Init(void)
{
	Vid_LoadConBackTexture(); // FIXME!
	Cmd_AddCommand("screenshot", Vid_ScreenShot_f);
	return 1;
}

/*
============================================================================== 
						SCREEN SHOTS 
============================================================================== 
typedef struct _TargaHeader
{
	unsigned char 	id_length, colormap_type, image_type;
	unsigned short	colormap_index, colormap_length;
	unsigned char		colormap_size;
	unsigned short	x_origin, y_origin, width, height;
	unsigned char		pixel_size, attributes;
} TargaHeader;
================== 
Vid_ScreenShot_f
================== 
*/  
void Vid_ScreenShot_f() 
{
	unsigned char	*buffer;
	char pcxname[13]; 
	char checkname[MAX_OSPATH];
	int i, c, temp;

// find a file name to save it to 
	strcpy(pcxname, "nWolf00.tga");
		
	for (i=0 ; i<=99 ; i++) 
	{ 
		pcxname[5] = i/10 + '0'; 
		pcxname[6] = i%10 + '0'; 
		sprintf(checkname, BASEDIR"%s", pcxname);
		if(!File_COM_Exist(checkname)) break;	// file doesn't exist
	} 
	if (i==100) 
	{
		Con_Printf("Video_HI: Couldn't write screenshot\n"); 
		return;
 	}

	buffer = malloc(XRES*YRES*3 + 18);
	memset(buffer, 0, 18); // reset header
	buffer[2] = 2;		// uncompressed type
	buffer[12] = XRES&255;
	buffer[13] = XRES>>8;
	buffer[14] = YRES&255;
	buffer[15] = YRES>>8;
	buffer[16] = 24;	// pixel size

	Vid_DumpScreen(buffer+18);

	// swap rgb to bgr
	c = 18+XRES*YRES*3;
	for (i=18 ; i<c ; i+=3)
	{
		temp = buffer[i];
		buffer[i] = buffer[i+2];
		buffer[i+2] = temp;
	}
	File_COM_Write(pcxname, buffer, XRES*YRES*3 + 18 );

	free(buffer);
	Msg_Printf ("Wrote %s", pcxname);
} 
