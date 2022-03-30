//USE extern!!!!!!! just for myself

//extern void DrawIronEagle(float x, float y, float z);
extern "C" void InitMD3Models(void);

extern "C" void ResetMapModels(void);


//Architectonic
extern "C" void AddIronEagle(int *x, int * y, int *state);
extern "C" void DrawIronEagle(float* x, float* y, int* x_, int* y_, float* z,
								float* d/*direction*/,int* lod);
extern "C" void FloorIronEagle(int* x, int* y);

extern "C" void DrawColumn(float* x, float* y,int* lod);
extern "C" void DrawMarbleColumn(float* x, float* y,int* lod);//SOD
//Lights
extern "C" void DrawLamp(float* x, float* y,int* lod);
extern "C" void DrawRedLamp(float* x, float* y,int* lod);//SOD
extern "C" void DrawChandelier(float* x, float* y,int* lod);
extern "C" void DrawTorcher(float* x, float* y,int* lod);

//Furniture
extern "C" void DrawTableChairs(float* x, float* y,int* lod);
extern "C" void DrawTable(float* x, float* y,int* lod);
extern "C" void DrawBed(float* x, float* y,int* lod);

//Kinghts
extern "C" void DrawKnight(float* x, float* y,int* lod);
extern "C" void DrawSpears(float* x, float* y,int* lod);

//Plants
extern "C" void DrawRoots(float* x, float* y,int* lod);
extern "C" void DrawRoundTree(float* x, float* y,int* lod);
extern "C" void DrawVasedTree(float* x, float* y,int* lod);

//Remains
extern "C" void DrawLyingSkeleton(float* x, float* y,int* lod);
extern "C" void DrawWell(float* x, float* y,int* lod);
extern "C" void DrawWaterWell(float* x, float* y,int* lod);
extern "C" void DrawCell(float* x, float* y, int* x_, int* y_, float* z,
								float* d/*direction*/,int* lod);
extern "C" void DrawCelledSkeleton(float* x, float* y, int* x_, int* y_, float* z,
								float* d/*direction*/,int* lod);



//Treasure
extern "C" void DrawGoldenKey(float* x, float* y,int* lod);
extern "C" void DrawSilverKey(float* x, float* y,int* lod);
extern "C" void DrawCross(float* x, float* y, int* lod);
extern "C" void DrawGoblet(float* x, float* y, int* lod);
extern "C" void DrawVase(float* x, float* y, int* lod);
extern "C" void DrawTreasureBox(float* x, float* y,int* lod);

//.....................
extern "C" void AddPortrait(int *x, int * y, int *state);
extern "C" void AddPoster(int *x, int * y, int *state);

extern "C" int CheckMapModificator(int *x, int * y);

//extern int	WhiteStoneDetails;//Detailed Textures {added by Idot}

