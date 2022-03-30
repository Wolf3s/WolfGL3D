#include <windows.h>
#include <stdio.h>
#include "WolfDef.h"

// mouse variables
cvar_t m_filter={"m_filter", "0"};

int		mouse_buttons;
int		mouse_oldbuttonstate;
POINT	current_pos;
int		mouse_x, mouse_y, old_mouse_x, old_mouse_y, mx_accum, my_accum;

static bool restore_spi;
static int originalmouseparms[3], newmouseparms[3]={0, 0, 1};

unsigned int uiWheelMessage;
bool mouseactive;
bool mouseinitialized;
static bool mouseparmsvalid, mouseactivatetoggle;
static bool mouseshowtoggle = 1;
static bool dinput_acquired;

static unsigned int		mstate_di;

byte scantokey[128]=
{
  0, 27, '1', '2', '3', '4', '5', '6',
	'7', '8', '9', '0', '-', '=', K_BACKSPACE, 9,// 0
	'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 
	'o', 'p', '[', ']',  13, K_CTRL, 'a', 's',// 1
	'd', 'f', 'g', 'h', 'j', 'k', 'l', ';',
	'\'', '`', K_SHIFT, '\\', 'z', 'x', 'c', 'v',// 2 
	'b', 'n', 'm', ',', '.', '/', K_SHIFT, '*',
	K_ALT, ' ', 0, K_F1, K_F2, K_F3, K_F4, K_F5,// 3 
	K_F6, K_F7, K_F8, K_F9, K_F10, K_PAUSE, 0, K_HOME,
	K_UPARROW, K_PGUP, '-', K_LEFTARROW, '5', K_RIGHTARROW, '+', K_END,//4 
	K_DOWNARROW, K_PGDN, K_INS, K_DEL, 0, 0, 0, K_F11,
	K_F12, 0, 0, 0, 0, 0, 0, 0,// 5 
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,// 6 
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,// 7 
};

/*
=======
MapKey: Map from windows to dos keynums
=======
*/
int MapKey(int key)
{
	key=(key>>16)&255; // 16...23 bits are scan-code!
	if(key>127)	return 0;
	if(scantokey[key]==0)
		Msg_Printf("Input: key 0x%02X has no translation", key);
	return scantokey[key];
}

void IN_KeyDown(int KeyData)
{
	Key_Event(MapKey(KeyData), true);
}

void IN_KeyUp(int KeyData)
{
	Key_Event(MapKey(KeyData), false);
}

// ------------------------- * Mouse * -------------------------
/*
===========
IN_UpdateClipCursor
===========
*/
void IN_UpdateClipCursor(void)
{
	if(mouseinitialized && mouseactive)
	{
		ClipCursor(&Win_WindowRect);
	}
}

/*
===========
IN_ShowMouse
===========
*/
void IN_ShowMouse(void)
{
	if(!mouseshowtoggle)
	{
		ShowCursor(TRUE);
		mouseshowtoggle=1;
	}
}

/*
===========
IN_HideMouse
===========
*/
void IN_HideMouse(void)
{
	if(mouseshowtoggle)
	{
		ShowCursor(FALSE);
		mouseshowtoggle=0;
	}
}

/*
===========
IN_ActivateMouse
===========
*/
void IN_ActivateMouse(void)
{
	mouseactivatetoggle = true;

	if(mouseinitialized)
	{
		if(mouseparmsvalid)
			restore_spi=SystemParametersInfo(SPI_SETMOUSE, 0, newmouseparms, 0);

		SetCursorPos(Win_cx, Win_cy);
		SetCapture(Win_hWnd);
		ClipCursor(&Win_WindowRect);

		mouseactive=true;
	}
}

/*
===========
IN_SetQuakeMouseState
===========
*/
void IN_SetQuakeMouseState(void)
{
	if(mouseactivatetoggle)	IN_ActivateMouse();
}

/*
===========
IN_DeactivateMouse
===========
*/
void IN_DeactivateMouse(void)
{
	mouseactivatetoggle = false;

	if(mouseinitialized)
	{
		if(restore_spi)
			SystemParametersInfo (SPI_SETMOUSE, 0, originalmouseparms, 0);

		ClipCursor (NULL);
		ReleaseCapture ();

		mouseactive = false;
	}
}

/*
===========
IN_RestoreOriginalMouseState
===========
*/
void IN_RestoreOriginalMouseState(void)
{
	if(mouseactivatetoggle)
	{
		IN_DeactivateMouse();
		mouseactivatetoggle=true;
	}

// try to redraw the cursor so it gets reinitialized, because sometimes it
// has garbage after the mode switch
	ShowCursor(TRUE);
	ShowCursor(FALSE);
}

/*
===========
IN_StartupMouse
===========
*/
void IN_StartupMouse(void)
{
	mouseinitialized=true;
	mouseparmsvalid=SystemParametersInfo(SPI_GETMOUSE, 0, originalmouseparms, 0);

	mouse_buttons=3;

// if a fullscreen video mode was set before the mouse was initialized,
// set the mouse state appropriately
	if(mouseactivatetoggle) IN_ActivateMouse();
}

/*
===========
IN_MouseEvent
===========
*/
void IN_MouseEvent(int mstate)
{
	int	i;

	if(mouseactive)
	{
	// perform button actions
		for (i=0; i<mouse_buttons; i++)
		{
			if( (mstate & (1<<i)) && !(mouse_oldbuttonstate & (1<<i)) )
			{
				Key_Event(K_MOUSE1+i, true);
			}
			if ( !(mstate & (1<<i)) && (mouse_oldbuttonstate & (1<<i)) )
			{
				Key_Event(K_MOUSE1+i, false);
			}
		}	
		mouse_oldbuttonstate = mstate;
	}
}

/*
===========
IN_Init
===========
*/
void IN_Init(void)
{
	// mouse variables
	Cvar_RegisterVariable(&m_filter);

	IN_StartupMouse();
}

/*
===========
IN_Shutdown
===========
*/
void IN_Shutdown(void)
{
	IN_DeactivateMouse();
	IN_ShowMouse();
}

// ------------------------- * Devider * -------------------------
/*
===========
IN_MouseMove
===========
*/
void IN_MouseMove(usercmd_t *cmd)
{
	int mx, my;

	if(!mouseactive) return;

	GetCursorPos(&current_pos);
	mx=current_pos.x-Win_cx+mx_accum;
	my=current_pos.y-Win_cy+my_accum;
	mx_accum=0;
	my_accum=0;

	if(m_filter.value)
	{
		mouse_x=(mx+old_mouse_x)/2;
		mouse_y=(my+old_mouse_y)/2;
	}
	else
	{
		mouse_x=mx;
		mouse_y=my;
	}

	old_mouse_x=mx;
	old_mouse_y=my;

	mouse_x=(int)(mouse_x*sensitivity.value);
	mouse_y=(int)(mouse_y*sensitivity.value);

// add mouse X/Y movement to cmd
	if(in_strafe.state&1 || lookstrafe.value)
		cmd->sidemove+=m_side.value*mouse_x;
	else
		Player.angle-=(int)(m_yaw.value*mouse_x);
	//if ((mouse_y>15)||(mouse_y<-15))
		Player.vangle-=(int)(mouse_y);

// if the mouse has moved, force it to the center, so there's room to move
	if(mx || my)
	{
		SetCursorPos(Win_cx, Win_cy);
	}
}

/*
===========
IN_Move
===========
*/
void IN_Move(usercmd_t *cmd)
{
	if(ActiveApp && !Minimized)
	{
		IN_MouseMove(cmd);
	}
}


/*
===========
IN_Accumulate
===========
*/
void IN_Accumulate(void)
{
	if (mouseactive)
	{
		GetCursorPos(&current_pos);

		mx_accum+=current_pos.x-Win_cx;
		my_accum+=current_pos.y-Win_cy;

	// force the mouse to the center, so there's room to move
		SetCursorPos(Win_cx, Win_cy);
	}
}

/*
===================
IN_ClearStates
===================
*/
void IN_ClearStates(void)
{
	if(mouseactive)
	{
		mx_accum=0;
		my_accum=0;
		mouse_oldbuttonstate=0;
	}
}