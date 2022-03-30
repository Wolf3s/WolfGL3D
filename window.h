extern HWND Win_hWnd;		// main window handle
extern RECT Win_WindowRect;
extern int Win_cx, Win_cy,
					Win_x, Win_y,
					Win_width, Win_height;
extern bool ActiveApp, Minimized;

extern int Win_Init(HINSTANCE hCurInstance);
extern void Win_ShutDown(void);
extern int Win_CreateWindow(bool FullScreen, bool Center, int width, int height, int bpp);
extern int Win_Show(bool show);
