extern void IN_KeyDown(int KeyData);
extern void IN_KeyUp(int KeyData);
void IN_MouseEvent(int mstate);

void IN_UpdateClipCursor(void);
void IN_ShowMouse(void);
void IN_HideMouse(void);
void IN_ActivateMouse(void);
void IN_SetQuakeMouseState(void);
void IN_DeactivateMouse(void);
void IN_RestoreOriginalMouseState(void);
void IN_StartupMouse(void);
void IN_MouseEvent(int mstate);
void IN_Init(void);
void IN_Shutdown(void);
void IN_MouseMove(usercmd_t *cmd);
void IN_Move(usercmd_t *cmd);
void IN_Accumulate(void);
void IN_ClearStates(void);
