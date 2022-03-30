// commands & cvars
#include <windows.h>
#include <stdio.h>

#include "WolfDef.h"

#define	MAX_ARGS 80
#define	MAX_ALIAS_NAME	32

typedef struct cmdalias_s
{
	struct cmdalias_s	*next;
	char	name[MAX_ALIAS_NAME];
	char	*value;
} cmdalias_t;
cmdalias_t *cmd_alias;

typedef struct cmd_function_s
{
	struct cmd_function_s	*next;
	char	*name;
	xcommand_t function;
} cmd_function_t;

static cmd_function_t *cmd_functions;		// possible commands to execute

static int   cmd_argc=0;
static char *cmd_argv[MAX_ARGS];
static char *cmd_args=NULL;
static char *cmd_null_string="";

cmd_source_t cmd_source;

bool cmd_wait;
//=============================================================================
/*
============
Cmd_Wait_f

Causes execution of the remainder of the command buffer to be delayed until
next frame.  This allows commands like:
bind g "impulse 5 ; +attack ; wait ; -attack ; impulse 2"
============
*/
void Cmd_Wait_f(void)
{
	cmd_wait=true;
}

/*
=============================================================================
						COMMAND BUFFER
=============================================================================
*/
sizebuf_t	cmd_text;
byte cmd_text_buf[8192];

/*
============
Cbuf_Init
============
*/
void Cbuf_Init(void)
{
// A Quake (Carmack) way
//	SZ_Alloc(&cmd_text, 8192);		// space for commands and script files
// A Hexen (Raven) way, I preffer a clever way.
	cmd_text.data=cmd_text_buf;
	cmd_text.maxsize=sizeof(cmd_text_buf);
}

/*
============
Cbuf_AddText: Adds command text at the end of the buffer
============
*/
void Cbuf_AddText(char *text)
{
	int	l;
	
	l=Q_strlen(text);

	if(cmd_text.cursize>=cmd_text.maxsize)
	{
		Con_Printf("Cbuf_AddText: overflow\n");
		return;
	}

	SZ_Write(&cmd_text, text, l);
}

/*
============
Cbuf_InsertText

Adds command text immediately after the current command
Adds a \n to the text
FIXME: actually change the command buffer to do less copying
============
*/
void Cbuf_InsertText(char *text)
{
	char *temp=NULL;
	int	 templen;

// copy off any commands still remaining in the exec buffer
	templen=cmd_text.cursize;
	if(templen)
	{
		temp=malloc(templen);
		Q_memcpy(temp, cmd_text.data, templen);
		SZ_Clear(&cmd_text);
	}

// add the entire text of the file
	Cbuf_AddText(text);
	
// add the copied off data
	if(templen)
	{
		SZ_Write(&cmd_text, temp, templen);
		free(temp);
	}
}

/*
============
Cbuf_Execute
============
*/
void Cbuf_Execute(void)
{
	int	 i;
	char *text;
	char line[1024];
	int	 quotes;
	
	while(cmd_text.cursize)
	{
// find a \n or ; line break
		text=(char *)cmd_text.data;

		quotes=0;
		for(i=0; i<cmd_text.cursize; i++)
		{
			if(text[i]=='"') quotes++;
			if(!(quotes&1) &&  text[i]==';') break;	// don't break if inside a quoted string
			if(text[i] == '\n') break;
		}
				
		memcpy(line, text, i);
		line[i]=0;
		
// delete the text from the command buffer and move remaining commands down
// this is necessary because commands (exec, alias) can insert data at the
// beginning of the text buffer

		if(i==cmd_text.cursize)
			cmd_text.cursize = 0;
		else
		{
			i++;
			cmd_text.cursize-=i;
			Q_memcpy(text, text+i, cmd_text.cursize);
		}

// execute the command line
		Cmd_ExecuteString(line, src_command);
		
		if(cmd_wait)
		{	// skip out while text still remains in buffer, leaving it
			// for next frame
			cmd_wait=false;
			break;
		}
	}
}

/*
==============================================================================
						SCRIPT COMMANDS
==============================================================================
*/

/*
===============
Cmd_Exec_f
===============
*/
void Cmd_Exec_f(void)
{
	char *f;

	if(Cmd_Argc()!=2)
	{
		Con_Printf("exec <filename> : execute a script file\n");
		return;
	}

	f=(char *)File_COM_LoadFile(Cmd_Argv(1));
	if(!f)
	{
		Con_Printf("couldn't exec %s\n", Cmd_Argv(1));
		return;
	}
	Con_Printf("execing %s\n", Cmd_Argv(1));
	
	Cbuf_InsertText(f);
	free(f);
}

void Cmd_Echo_f(void)
{
	Con_Printf("%s\n", Cmd_Args());
}

void Cmd_CmdList_f(void)
{
	cmd_function_t *cmd;

	Con_Printf("Console commands list:\n");

	for(cmd=cmd_functions; cmd; cmd=cmd->next)
	{
		Con_Printf("%s\n", cmd->name);
	}
}

/*
===============
Cmd_Alias_f

Creates a new command that executes a command string (possibly ; seperated)
===============
*/
char *CopyString(char *in)
{
	char *out;
	
	out=malloc(strlen(in)+1);
	strcpy(out, in);
	return out;
}

void Cmd_Alias_f(void)
{
	cmdalias_t *a;
	char cmd[1024];
	int	i, c;
	char *s;

	if(Cmd_Argc()==1)
	{
		Con_Printf("Current alias commands:\n");
		for(a=cmd_alias; a; a=a->next)
			Con_Printf ("%s : %s\n", a->name, a->value);
		return;
	}

	s=Cmd_Argv(1);
	if(strlen(s)>=MAX_ALIAS_NAME)
	{
		Con_Printf("Alias name is too long\n");
		return;
	}

	// if the alias allready exists, reuse it
	for (a=cmd_alias; a; a=a->next)
	{
		if (!strcmp(s, a->name))
		{
			free(a->value);
			break;
		}
	}

	if(!a)
	{
		a=malloc(sizeof(cmdalias_t));
		a->next=cmd_alias;
		cmd_alias=a;
	}
	strcpy(a->name, s);	

// copy the rest of the command line
	cmd[0]=0;	// start out with a null string
	c=Cmd_Argc();
	for(i=2; i<c; i++)
	{
		strcat(cmd, Cmd_Argv(i));
		if(i!=c) strcat(cmd, " ");
	}
	strcat(cmd, "\n");
	
	a->value=CopyString(cmd);
}

/*
=============================================================================
					COMMAND EXECUTION
=============================================================================
*/
int Cmd_Init(void)
{
//
// register our commands
//
	Cbuf_Init();

//	Cmd_AddCommand ("stuffcmds",Cmd_StuffCmds_f);
	Cmd_AddCommand("exec", Cmd_Exec_f);
	Cmd_AddCommand("echo", Cmd_Echo_f);
	Cmd_AddCommand("alias", Cmd_Alias_f);
//	Cmd_AddCommand("cmd", Cmd_ForwardToServer);
	Cmd_AddCommand("wait", Cmd_Wait_f);
	Cmd_AddCommand("cmdlist", Cmd_CmdList_f);
	return 1;
}

/*
============
Cmd_Argc
============
*/
int	Cmd_Argc (void)
{
	return cmd_argc;
}

/*
============
Cmd_Argv
============
*/
char *Cmd_Argv(int arg)
{
	if(arg>=cmd_argc)	return cmd_null_string;
	return cmd_argv[arg];	
}

/*
============
Cmd_Args
============
*/
char *Cmd_Args(void)
{
	return cmd_args;
}


/*
============
Cmd_TokenizeString: Parses the given string into command line tokens.
============
*/
void Cmd_TokenizeString(char *text)
{
	int i;

	for(i=0 ; i<cmd_argc ; i++) free(cmd_argv[i]);

	cmd_argc = 0;
	cmd_args = NULL;

	while(1)
	{
// skip whitespace up to a /n
		while (*text && *text <= ' ' && *text != '\n') text++;
		
		if (*text == '\n')
		{	// a newline seperates commands in the buffer
			text++;
			break;
		}
		if (!*text)	return;
		if (cmd_argc==1) cmd_args = text;
			
		text = COM_Parse(text);
		if(!text)	return;

		if (cmd_argc < MAX_ARGS)
		{
			cmd_argv[cmd_argc]=malloc(Q_strlen(com_token)+1);
			Q_strcpy(cmd_argv[cmd_argc], com_token);
			cmd_argc++;
		}
	}
}

/*
============
Cmd_AddCommand
============
*/
void Cmd_AddCommand(char *cmd_name, xcommand_t function)
{
	cmd_function_t	*cmd;
		
// fail if the command is a variable name
	if(Cvar_VariableString(cmd_name)[0])
	{
		Con_Printf ("Cmd_AddCommand: %s already defined as a var\n", cmd_name);
		return;
	}
	
// fail if the command already exists
	for(cmd=cmd_functions; cmd; cmd=cmd->next)
	{
		if(!Q_strcmp(cmd_name, cmd->name))
		{
			Con_Printf("Cmd_AddCommand: %s already defined\n", cmd_name);
			return;
		}
	}

	cmd=malloc(sizeof(cmd_function_t));
	cmd->name=cmd_name;
	cmd->function=function;
	cmd->next=cmd_functions;
	cmd_functions=cmd;
}

/*
============
Cmd_Exists
============
*/
bool Cmd_Exists(char *cmd_name)
{
	cmd_function_t *cmd;

	for(cmd=cmd_functions; cmd; cmd=cmd->next)
	{
		if(!Q_strcmp(cmd_name, cmd->name)) return true;
	}
	return false;
}

/*
============
Cmd_CompleteCommand
============
*/
char *Cmd_CompleteCommand(char *partial)
{
	cmd_function_t *cmd;
	int	len;
	
	len=Q_strlen(partial);
	
	if(!len) return NULL;
		
// check functions
	for(cmd=cmd_functions; cmd; cmd=cmd->next)
		if(!Q_strncmp(partial, cmd->name, len))
			return cmd->name;

	return NULL;
}

/*
============
Cmd_ExecuteString

A complete command line has been parsed, so try to execute it
FIXME: lookupnoadd the token to speed search?
============
*/
void Cmd_ExecuteString(char *text, cmd_source_t src)
{
	cmd_function_t *cmd;
	cmdalias_t *a;

	cmd_source=src;

	Cmd_TokenizeString(text);
	if(!Cmd_Argc()) return;	// no tokens

// FIXME: maybe first process aliases, to let UNIX like hacks: alias FOV "FOV 90"
// check functions
	for(cmd=cmd_functions; cmd; cmd=cmd->next)
	{
		if(!Q_strcasecmp(cmd_argv[0], cmd->name))
		{
			cmd->function();
			return;
		}
	}

// check alias
	for(a=cmd_alias; a; a=a->next)
	{
		if(!Q_strcasecmp(cmd_argv[0], a->name))
		{
			Cbuf_InsertText(a->value);
			return;
		}
	}
	
// check cvars
	if(!Cvar_Command())
		Con_Printf ("Unknown command \"%s\"\n", Cmd_Argv(0));
}

/*
================
Cmd_CheckParm

Returns the position (1 to argc-1) in the command's argument list
where the given parameter apears, or 0 if not present
================
*/
int Cmd_CheckParm(char *parm)
{
	int i;
	
	if (!parm) return 0;

	for (i=1; i<Cmd_Argc(); i++)
		if (!Q_strcasecmp(parm, Cmd_Argv (i)))
			return i;
			
	return 0;
}

