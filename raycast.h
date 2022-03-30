#define R_RAY_X 0
#define R_RAY_Y 1

int Distance_Square_;//debug added by Idot
int n_sprt_;//debug added by Idot
int n_skiped_;//debug added by Idot

extern void CheckDistanceSquares(void);
//extern int GetDistanceSquare(int x,int y);
extern unsigned int DistanceSquares[64][64];
extern unsigned char spotvis[64][64];
extern unsigned char floorvis[64][64];


extern void R_RayCast(int x, int y, int angle, int fov);
int R_CastRay(int x, int y, int angle, int n);
int R_RayCallback(int x, int y, int angle, int R_Ray_y);
