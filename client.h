typedef struct
{
// intended velocities
	float	forwardmove;
	float	sidemove;
} usercmd_t;

void CL_SendCmd(void);

//
// cl_input
//
typedef struct
{
	int		down[2];		// key nums holding it down
	int		state;			// low bit is down state
} kbutton_t;

extern kbutton_t in_left, in_right, in_forward, in_back;
extern kbutton_t in_moveleft, in_moveright;
extern kbutton_t in_strafe, in_speed, in_use, in_attack;

extern void CL_InitInput(void);
extern float CL_KeyState(kbutton_t *key);
void CL_BaseMove(usercmd_t *cmd);


//
// cvars
//
extern cvar_t cl_name;
extern cvar_t cl_color;

extern cvar_t	lookstrafe;
extern cvar_t	sensitivity;

extern cvar_t m_yaw;
extern cvar_t m_forward;
extern cvar_t m_side;

extern cvar_t cl_forwardspeed;
extern cvar_t cl_backspeed;
extern cvar_t cl_sidespeed;
extern cvar_t cl_yawspeed;

extern cvar_t cl_movespeedkey;
extern cvar_t cl_anglespeedkey;
extern cvar_t allwaysrun;


void CL_Init(void);
