#ifndef _ELAFRI
#define _ELAFRI

#include <termios.h> //for term flags
#include "evar.h"

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

extern u8 kbd[64]; //parsed keyboard input
extern u32 kbdSize; //size of input buffer
extern uvec2 res; //term resolution

void GetResolution();

void ElafriInit(); //Initialization procedures

void Exit();

void Input(); //Gets user input from Terminal
#endif
