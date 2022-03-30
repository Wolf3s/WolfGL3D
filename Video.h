#define RGBA_WHITE 0xFF, 0xFF, 0xFF, 0xFF
#define RGBA_BLACK 0x00, 0x00, 0x00, 0xFF
#define RGBA_RED   0xFF, 0x00, 0x00, 0xFF
#define RGBA_GREEN 0x00, 0xFF, 0x00, 0xFF
#define RGBA_BLUE  0x00, 0x00, 0xFF, 0xFF

#define RGBA_WLMNU 0x8C, 0x8C, 0x8C, 0xFF // menu item
#define RGBA_WLMSL 0xC0, 0xC0, 0xC0, 0xFF // selected menu item
#define RGBA_WLMDS 0x70, 0x00, 0x00, 0xFF // disabled menu item
#define RGBA_WLMCU 0xB4, 0xAC, 0x00, 0xFF // current (yellow menu item)
#define RGBA_WLMCS 0xFC, 0xF4, 0x00, 0xFF // current + selected

// Read This! colors
#define RGBA_PGNUM 0x40, 0x40, 0x00, 0xFF // page number in Read This!
#define RGBA_READB 0xDC, 0xDC, 0xDC, 0xFF // background in Read This!

// 16 base colors!
#define RGBA_C15 0xFC, 0xFC, 0xFC, 0xFF

#define RGBA_WBKGC 0x58, 0x00, 0x00, 0xFF
#define RGBA_BORDC 0x88, 0x00, 0x00, 0xFF
#define RGBA_BRD2C 0xD4, 0x00, 0x00, 0xFF
#define RGBA_DEACT 0x70, 0x00, 0x00, 0xFF
#define RGBA_STRPC 0x64, 0x00, 0x00, 0xFF

#define RGBA_SBKGC 0x00, 0x00, 0x58, 0xFF
#define RGBA_SBORD 0x00, 0x00, 0x88, 0xFF
#define RGBA_SBRD2 0x00, 0x00, 0xD4, 0xFF
#define RGBA_SDEAC 0x00, 0x00, 0x70, 0xFF
#define RGBA_SSTRP 0x00, 0x00, 0x64, 0xFF

#define RGBA_READC 0xB4, 0xAC, 0x00, 0xFF
#define RGBA_READH 0xFC, 0xF4, 0x00, 0xFF
#define RGBA_NOEPC 0x04, 0x70, 0x00, 0xFF
#define RGBA_NOEPH 0x04, 0xA4, 0x00, 0xFF
#define RGBA_VIEWC 0x00, 0x40, 0x40, 0xFF
#define RGBA_TEXTC 0x8C, 0x8C, 0x8C, 0xFF
#define RGBA_HIGHL 0xC0, 0xC0, 0xC0, 0xFF

#define RGBA_PG13C 0x20, 0xA8, 0xFC, 0xFF
#define RGBA_INTER 0x00, 0x40, 0x40, 0xFF

// Wolf Palette
#define RGB_WOLFCOLOR(index)	WolfPal[index].r, WolfPal[index].g, WolfPal[index].b
#define RGBA_WOLFCOLOR(index) WolfPal[index].r, WolfPal[index].g, WolfPal[index].b, 0xFF

#define RGBA_PARAM(n) unsigned char r##n, unsigned char g##n, unsigned char b##n, unsigned char a##n
#define RGBA_USEPARAM(n) r##n, g##n, b##n, a##n

typedef struct
{
	unsigned char r;
	unsigned char g;
	unsigned char b;
} RGBcolor;
typedef struct
{
	unsigned char r;
	unsigned char g;
	unsigned char b;
	unsigned char a;
} RGBAcolor;

extern int Vid_Init(void);

extern void Vid_DrawPicture(int x1, int y1, int x2, int y2, int Tex, int tx1, int ty1, int tx2, int ty2, int texsize);
extern void Vid_DumpScreen(unsigned char *buffer);
extern void Vid_LoadConBackTexture(void);
extern void Vid_UnLoadConBackTexture(void);
extern void Vid_DrawConBack(int Pos, unsigned char transp);
extern void Vid_DrawBox(int x, int y, int w, int h, RGBA_PARAM(1));
extern void Vid_WMenuBack(void);
extern void Vid_SMenuBack(void);
extern void Vid_WDrawStripes(void);
extern void Vid_SDrawStripes(void);
extern void Vid_MenuBack(void);
extern void Vid_DrawStripes(void);
extern void Vid_DrawWindow(int x, int y, int w, int h, RGBA_PARAM(1), RGBA_PARAM(2), RGBA_PARAM(3));
extern void Vid_DrawMenuBox(int x, int y, int w, int h);
extern void Vid_Message(char *msg);
extern void Vid_DrawPic(int x, int y, int name);
extern void Vid_DrawPicEx(int x, int y, int name);
extern void Vid_DrawPicIndirect(int x, int y, cache_pic *pic);

extern void Vid_ScreenShot_f();


void Draw_X_Wall(int X, int Y_start, int tex, float TexInv);
void Draw_Y_Wall(int X_start, int Y, int tex, float TexInv);
void Draw_X_Pwall(int X, int Y_start, int tex, float TexInv);
void Draw_Y_Pwall(int X_start, int Y, int tex, float TexInv);
void Draw_X_Door(int X, int Y, int tex);
void Draw_Y_Door(int X, int Y, int tex);
void DrawSprites(void);
void DrawCelFloor(void);
void DrawGun(void);

