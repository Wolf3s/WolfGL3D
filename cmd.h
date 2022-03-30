typedef void (*xcommand_t) (void);

typedef enum
{
	src_client,		// came in over a net connection as a clc_stringcmd
								// host_client will be valid during this state.
	src_command		// from the command buffer
} cmd_source_t;

extern cmd_source_t cmd_source;

extern void Cbuf_Execute(void);
extern void Cbuf_InsertText(char *text);
extern void Cbuf_AddText(char *text);
void Cbuf_Init(void);

extern int  Cmd_Init(void);
extern char *Cmd_CompleteCommand(char *partial);
extern void Cmd_ExecuteString(char *cmd, cmd_source_t src);
extern void Cmd_AddCommand(char *cmd_name, xcommand_t function);
extern int	Cmd_Argc (void);
extern char *Cmd_Argv(int arg);
extern char *Cmd_Args(void);
extern int  Cmd_CheckParm(char *parm);
extern bool Cmd_Exists(char *cmd_name);

void Cmd_TokenizeString(char *text);
void Cmd_Exec_f(void);
void Cmd_Echo_f(void);
void Cmd_CmdList_f(void);
void Cmd_Alias_f(void);
