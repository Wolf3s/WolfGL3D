//============================================================================
typedef struct sizebuf_s
{
	bool allowoverflow;	// if false, do a Sys_Error
	bool overflowed;			// set to true if the buffer size failed
	byte *data;
	int	maxsize;
	int	cursize;
} sizebuf_t;

extern void SZ_Alloc(sizebuf_t *buf, int startsize);
extern void SZ_Free(sizebuf_t *buf);
extern void SZ_Clear(sizebuf_t *buf);
extern void *SZ_GetSpace(sizebuf_t *buf, int length);
extern void SZ_Write(sizebuf_t *buf, void *data, int length);
extern void SZ_Print(sizebuf_t *buf, char *data);	// strcats onto the sizebuf
//============================================================================

extern	char		com_token[1024];

extern void Q_memset (void *dest, int fill, int count);
extern void Q_memcpy (void *dest, void *src, int count);
extern int Q_memcmp (void *m1, void *m2, int count);
extern void Q_strcpy (char *dest, char *src);
extern void Q_strncpy (char *dest, char *src, int count);
extern int Q_strlen (char *str);
extern char *Q_strrchr(char *s, char c);
extern void Q_strcat (char *dest, char *src);
extern int Q_strcmp (char *s1, char *s2);
extern int Q_strncmp (char *s1, char *s2, int count);
extern int Q_strncasecmp (char *s1, char *s2, int n);
extern int Q_strcasecmp (char *s1, char *s2);
extern int Q_atoi (char *str);
extern float Q_atof (char *str);
extern char	*COM_Parse (char *data);

extern char *va(char *format, ...);
