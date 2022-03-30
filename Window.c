#include <windows.h>
#include <stdio.h>
#include "WolfDef.h"
#include "resource.h"


LONG WINAPI MainWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

HWND Win_hWnd;		// main window handle
RECT Win_WindowRect;
int Win_cx, Win_cy,
		Win_x, Win_y,
		Win_width, Win_height;
int Win_bpp;
static bool windowed;
bool ActiveApp=1, Minimized=0;
HINSTANCE hInst;

int Win_Init(HINSTANCE hCurInstance)
{
	WNDCLASS wc;
	hInst=hCurInstance;

	wc.style        =CS_OWNDC;
  wc.lpfnWndProc  =(WNDPROC)MainWndProc;
  wc.cbClsExtra   =0;
  wc.cbWndExtra   =0;
	wc.hInstance    =hCurInstance;
  wc.hIcon        =LoadIcon	 (hInst, MAKEINTRESOURCE(IDI_ICON1));
  wc.hCursor      =LoadCursor(NULL, IDC_ARROW);
  wc.hbrBackground=NULL;
  wc.lpszMenuName =0;
  wc.lpszClassName="WolfGL-3D";

  if(!RegisterClass(&wc))
	{
		MessageBox(0, "Failed to Register Window Class", "WolfGL", MB_ICONERROR | MB_OK);
		return 0;
  }
	return 1;
}

int Win_CreateWindow(bool FullScreen, bool Center, int width, int height, int bpp)
{
	DWORD style;
	RECT adjust;

	style=FullScreen ? WS_POPUP : WS_OVERLAPPED | WS_BORDER | 
																WS_CAPTION | WS_SYSMENU |	
																WS_MINIMIZEBOX;

	if(FullScreen || !Center)
		Win_x=Win_y=0;
	else
	{
		Win_x=(GetSystemMetrics(SM_CXSCREEN)-width)/2;
		Win_y=(GetSystemMetrics(SM_CYSCREEN)-height)/2;
		if(Win_x>Win_y*2) Win_x>>=1; // dual screens
		Win_x=(Win_x<0) ? 0: Win_x;
		Win_x=(Win_x<0) ? 0: Win_x;
	}
	adjust.left=Win_x;
	adjust.top =Win_y;
	adjust.right =Win_x+width;
	adjust.bottom=Win_y+height;

	AdjustWindowRect(&adjust, style, 0);
	Win_width=width; Win_height=height;
	Win_cx=Win_x+Win_width/2;
	Win_cy=Win_y+Win_height/2;
	windowed=!FullScreen;
	Win_bpp=bpp;
  if(!FullScreen && adjust.left<0 && adjust.top<0)
	{ // make header visible!
		int dx, dy;
		
		dx=-adjust.left;
		dy=-adjust.top;
		Win_x+=dx;
		Win_y+=dy;
		Win_cx+=dx;
		Win_cy+=dy;
		adjust.left=adjust.top=0;
		adjust.right+=dx;
		adjust.bottom+=dy;
	}
	Win_hWnd=CreateWindow("WolfGL-3D", "WolfGL 3D",
//	Win_hWnd=CreateWindow("WolfGL", "WolfGl for Nvidia",
												style, adjust.left, adjust.top, 
												adjust.right-adjust.left, adjust.bottom-adjust.top, 
												NULL, NULL, hInst, NULL);
	GetClientRect(Win_hWnd, &Win_WindowRect);

  if(!Win_hWnd)
	{
    MessageBox(0, "Failed to Create Window", "WolfGL 3D", MB_ICONERROR | MB_OK);
		return 0;
  }

	return 1;
}

int Win_Show(bool show)
{
	if(!windowed) // full screen
	{
		if(show)
		{
			DEVMODE dmScreenSettings;	// FullScreen

			memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));	// Makes Sure Memory's Cleared
			dmScreenSettings.dmSize=sizeof(dmScreenSettings);				// Size Of The Devmode Structure
			dmScreenSettings.dmPelsWidth =Win_width;	// Selected Screen Width
			dmScreenSettings.dmPelsHeight=Win_height;	// Selected Screen Height
			dmScreenSettings.dmBitsPerPel=Win_bpp;	// Selected Bits Per Pixel
			dmScreenSettings.dmFields=DM_BITSPERPEL|DM_PELSWIDTH|DM_PELSHEIGHT;
			// Try To Set Selected Mode And Get Results.  NOTE: CDS_FULLSCREEN Gets Rid Of Start Bar.
			if(ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN)!=DISP_CHANGE_SUCCESSFUL)
			{
				Con_Printf("Error setting screen resolution to %dx%dx%d\n", Win_width, Win_height, Win_bpp);
			}
		}
		else // !show /hide/
		{
			ChangeDisplaySettings(NULL, 0);
		}
	}
	if(show)
	{
		ShowWindow(Win_hWnd, SW_SHOWNORMAL);
		IN_ActivateMouse();
		IN_HideMouse();
	}
	else
	{
		IN_DeactivateMouse();
		IN_ShowMouse();
	}

	return 1;
}

void Win_ShutDown(void)
{
	ChangeDisplaySettings(NULL, 0); // exit fullscreen!
	DestroyWindow(Win_hWnd);
}

extern void CleanUp(void);
extern void NewFrame(void);
extern char quit;

#ifndef WM_MOUSEWHEEL
	#define WM_MOUSEWHEEL 0x020A // Intellimouse wheel
#endif


/*
================
ClearAllStates
================
*/
void ClearAllStates(void)
{
	int i;
	
// send an up event for each key, to make sure the server clears them all
	for(i=0; i<256; i++)
	{
		Key_Event(i, false);
	}

	//Key_ClearStates();
	IN_ClearStates();
}

// Perform all jobs when window looses/gets focus i.e. hide/show mouse etc.
void Win_AppActivate(bool fActive, bool minimize)
{
	if(fActive) // Got focus: restore operations
	{
		Win_Show(true);
	}
	else // Lost focus: suspend app & return mouse & video to user!
	{
		Win_Show(false);
	}
}

LONG WINAPI MainWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	LONG lRet = 0;
	int temp;

  switch(uMsg)
	{
	case WM_KILLFOCUS:
		if(!quit)
			ShowWindow(hWnd, SW_SHOWMINNOACTIVE);
		break;
	case WM_ACTIVATE:
		if(!quit)
		{
			Win_AppActivate(LOWORD(wParam)!=WA_INACTIVE, (bool)HIWORD(wParam));
			// fix the leftover Alt from any Alt-Tab or the like that switched us away
			ClearAllStates();
		}
		break;
	case WM_SIZE:
    break;
  case WM_CLOSE:
    DestroyWindow(hWnd);
    break;
  case WM_DESTROY:
		CleanUp();
		break;
  case WM_PAINT:
		NewFrame();
    break;
	case WM_LBUTTONDOWN:
	case WM_LBUTTONUP:
	case WM_RBUTTONDOWN:
	case WM_RBUTTONUP:
	case WM_MBUTTONDOWN:
	case WM_MBUTTONUP:
	case WM_MOUSEMOVE:
		temp = 0;
		if(wParam & MK_LBUTTON) temp|=1;
		if(wParam & MK_RBUTTON)	temp|=2;
		if(wParam & MK_MBUTTON)	temp|=4;
		IN_MouseEvent(temp);
		break;
	case WM_MOUSEWHEEL: // Mouse wheel (Microsoft Intellimouse) /Coool thing!/
		if((short)HIWORD(wParam)>0)
		{
			Key_Event(K_MWHEELUP, true);
			Key_Event(K_MWHEELUP, false);
		}
		else
		{
			Key_Event(K_MWHEELDOWN, true);
			Key_Event(K_MWHEELDOWN, false);
		}
		break;
	case WM_KEYDOWN:
	case WM_SYSKEYDOWN:
		IN_KeyDown(lParam);
		break;
	case WM_KEYUP:
	case WM_SYSKEYUP:
		IN_KeyUp(lParam);
		break;
	case WM_SYSCHAR:
	// keep Alt-Space from happening
		break;
  default:
    lRet=DefWindowProc(hWnd, uMsg, wParam, lParam);
    break;
  }
  return lRet;
}
