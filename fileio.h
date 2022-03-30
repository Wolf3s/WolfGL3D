#define MAX_OSPATH MAX_PATH 
#define MAX_WLPATH 64 
typedef struct
{
	short width;
	short height;
	unsigned char *data;
} wl_pic;

extern RGBcolor WolfPal[256];

extern int File_Init(void);
extern void File_ShutDown(void);

extern void File_LoadPalette(void);
extern int File_PML_LoadWall(int n, unsigned char *block);
extern int File_PML_LoadSprite(int n, unsigned char *block);
extern int File_PML_LoadDigiMap(void **buff);
extern int File_PML_LoadSound(int st, int len, unsigned char *sndbuff);

extern int File_COM_Exist(char *filename);
extern long File_COM_Size(char *filename);
extern int File_COM_filelength(FILE *fp);
extern int File_COM_FileOpenRead(char *path, FILE **hndl);
extern void File_COM_Write(char *filename, void *data, int len);
extern unsigned char *File_COM_LoadFile(char *filename);
extern int File_COM_CRCfile(char *path);

extern int File_RAW_Read(char *filename, long offset, unsigned char *data, int width, int height, char planes);
extern int File_TGA_Read(char *filename, unsigned char *data);

int File_PML_OpenPageFile(char *filename);
int File_PML_ReadPage(int n, unsigned char *data);

int File_VGA_OpenVgaFiles(char *headername, char *dictname, char *vganame);;
extern long File_VGA_GetChunkSize(int n);
extern int File_VGA_ReadChunk(int n, unsigned char *target);
int File_VGA_ReadPic(int chunk, wl_pic *pic);

extern int File_MAP_OpenMapFiles(char *filename, char *mapfile);
extern int File_MAP_ReadData(int level, unsigned short *layer1, unsigned short *layer2, unsigned short *layer3, char *name);
void File_MAP_Expand(word *source, word *dest);

int File_AUD_OpenAudioFiles(char *filename, char *audfile);
int File_AUD_ReadChunk(int n, unsigned char *buff);

void File_CarmackExpand(word *source, word *dest, word length);
void File_RLEWexpand(word *source, word *dest, long length, word rlewtag);

void File_Dir_f(void);
void File_CRC_f(void);

int File_LoadTexture(int name, unsigned char **data, cache_tex *tex);
