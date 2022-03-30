extern bool con_forcedup; // (full screen console) because no entities to refresh

extern int Con_Init(void);
extern void Con_Printf(char *fmt, ...);
extern void Msg_Printf(char *fmt, ...);
extern void Con_ClearScrMsg(void);
extern void Con_Process(void);
extern void Con_DrawConsole(void);
extern void Con_ToggleConsole_f(void);
extern void Con_KeyInput(int key);

void Con_LineFeed(void);
void Con_MsgLFeed(void);
void Con_AddMsg(char *text);
void Con_AddText(char *text);
void Con_DebugLog(char *text);
void Con_ProcMsg(void);
void Con_DrawMessages(void);
void Con_ClearInput(void);
void Con_Clear_f(void);
