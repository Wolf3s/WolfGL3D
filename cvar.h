//#include "plain_C_is_bullshit.h"
typedef struct cvar_s
{
	char	*name;
	char	*string;
//	enum {false, true}	archive;		// set to true to cause it to be saved to vars.rc
//	enum {false, true}	server;			// notifies players when changed
	
	bool	archive;		// set to true to cause it to be saved to vars.rc
	bool	server;			// notifies players when changed
	float	value;
	struct cvar_s *next;
} cvar_t;

void Cvar_RegisterVariable(cvar_t *variable);
void Cvar_Set(char *var_name, char *value);
void Cvar_SetValue(char *var_name, float value);
float Cvar_VariableValue(char *var_name);
char *Cvar_VariableString(char *var_name);
char *Cvar_CompleteVariable(char *partial);
bool Cvar_Command(void);
void Cvar_WriteVariables(FILE *f);
cvar_t *Cvar_FindVar(char *var_name);

extern cvar_t	*cvar_vars;