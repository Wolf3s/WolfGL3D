#include <windows.h>
#include <stdio.h>
#include "WolfDef.h"

cvar_t *cvar_vars=NULL;
char *cvar_null_string="";

/*
============
Cvar_FindVar
============
*/
cvar_t *Cvar_FindVar(char *var_name)
{
	cvar_t *var;
	
	for(var=cvar_vars; var; var=var->next)
		if(!Q_strcmp(var_name, var->name))
			return var;

	return NULL;
}

/*
============
Cvar_VariableValue
// returns 0 if not defined or non numeric
============
*/
float	Cvar_VariableValue(char *var_name)
{
	cvar_t	*var;
	
	var=Cvar_FindVar(var_name);
	if (!var) return 0;
	return Q_atof(var->string);
}

/*
============
Cvar_VariableString
// returns an empty string if not defined
============
*/
char *Cvar_VariableString(char *var_name)
{
	cvar_t *var;
	
	var=Cvar_FindVar(var_name);
	if(!var) return cvar_null_string;
	return var->string;
}

/*
============
Cvar_CompleteVariable
// attempts to match a partial variable name for command line completion
// returns NULL if nothing fits
============
*/
char *Cvar_CompleteVariable(char *partial)
{
	cvar_t *cvar;
	int len;
	
	len=Q_strlen(partial);
	
	if(!len) return NULL;
		
// check functions
	for(cvar=cvar_vars; cvar; cvar=cvar->next)
		if(!Q_strncmp(partial, cvar->name, len))
			return cvar->name;

	return NULL;
}


/*
============
Cvar_Set
// equivelant to "<name> <variable>" typed at the console
============
*/
void Cvar_Set(char *var_name, char *value)
{
	cvar_t *var;
	bool changed;
	
	var=Cvar_FindVar(var_name);
	if(!var)
	{	// there is an error in C code if this happens
		Con_Printf("Cvar: variable %s not found\n", var_name);
		return;
	}

	changed=Q_strcmp(var->string, value);
	
	free(var->string);	// free the old value string
	
	var->string=malloc(Q_strlen(value)+1);
	Q_strcpy(var->string, value);
	var->value=Q_atof(var->string);
/* FIXME!!!!
	if(var->server && changed)
	{
		if(sv.active)
			SV_BroadcastPrintf("\"%s\" changed to \"%s\"\n", var->name, var->string);
	}
*/
}

/*
============
Cvar_SetValue
// expands value to a string and calls Cvar_Set
============
*/
void Cvar_SetValue(char *var_name, float value)
{
	char val[32];
	
	sprintf(val, "%f", value);
	Cvar_Set(var_name, val);
}


/*
============
Cvar_RegisterVariable

// registers a cvar that allready has the name, string, and optionally the
// archive elements set.

Adds a freestanding variable to the variable list.
============
*/
void Cvar_RegisterVariable(cvar_t *variable)
{
	char *oldstr;
	
// first check to see if it has allready been defined
	if(Cvar_FindVar(variable->name))
	{
		Con_Printf("Cvar: Can't register variable %s, allready defined\n", variable->name);
		return;
	}

// check for overlap with a command
	if(Cmd_Exists(variable->name))
	{
		Con_Printf("Cvar: %s is a command\n", variable->name);
		return;
	}

// copy the value off, because future sets will Z_Free it
	oldstr=variable->string;
	variable->string=malloc(Q_strlen(variable->string)+1);	
	Q_strcpy(variable->string, oldstr);
	variable->value=Q_atof(variable->string);
	
// link the variable in
	variable->next=cvar_vars;
	cvar_vars=variable;
}

/*
============
Cvar_Command
// called by Cmd_ExecuteString when Cmd_Argv(0) doesn't match a known
// command.  Returns true if the command was a variable reference that
// was handled. (print or change)

Handles variable inspection and changing from the console
============
*/
bool Cvar_Command(void)
{
	cvar_t *v;

// check variables
	v=Cvar_FindVar(Cmd_Argv(0));
	if(!v) return false;
		
// perform a variable print or set
	if(Cmd_Argc()==1)
	{
		Con_Printf("\"%s\" is \"%s\"\n", v->name, v->string);
		return true;
	}

	Cvar_Set(v->name, Cmd_Argv(1));
	return true;
}


/*
============
Cvar_WriteVariables

Writes lines containing "set variable value" for all variables
with the archive flag set to true.
============
*/
void Cvar_WriteVariables(FILE *f)
{
	cvar_t	*var;
	
	for(var=cvar_vars; var; var=var->next)
		if(var->archive)
			fprintf(f, "%s \"%s\"\n", var->name, var->string);
}

