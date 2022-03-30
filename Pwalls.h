typedef struct
{
	bool active;
	int PWtilesmoved;
	int PWpointsmoved;
	dir4type dir;
	int x, y;
	int dx, dy;
	int tex;
} Pwall_t;

extern Pwall_t PWall;

void PW_Reset(void);
void PW_Push(int x, int y, dir4type dir);
void PW_Process(void);
