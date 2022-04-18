#ifndef _ELAFRI
#define _ELAFRI

#include <termios.h> //for term flags
#include "evar.h"
#include "graphics.h"

//F1 to F12 are ordered as 0-11 after the 255 escape char
#define INSERT 50 //in kbd[], this would be 255 50
#define DELETE 51
#define PgUP 53
#define PgDown 54
#define UP 65
#define DOWN 66
#define RIGHT 67
#define LEFT 68
#define END 70
#define HOME 72

typedef struct {
	uvec2 res;
	struct termios oldTermFlags;
	u32 max_fps;
	u8 kbd[64];
	u32 kbdSize;
	gr Gr;
	void* global_data;
} elafriContext;

typedef void (*frameHandler)(F32, elafriContext*);

uvec2 GetResolution();

elafriContext ElafriInit(); //Initialization procedures

void Exit(elafriContext);

u32 Input(); //Gets user input from Terminal

void MainLoop(frameHandler callback, elafriContext current);
#endif
