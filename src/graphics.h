#ifndef _EGFX
#define _EGFX
#include "evar.h"

#define LIGHT_RED 0
#define LIGHT_GREEN 1
#define LIGHT_YELLOW 2
#define LIGHT_BLUE 3
#define LIGHT_PINK 4
#define CYAN 5
#define LIGHT_WHITE 6
#define GRAY 7
#define RED 8
#define GREEN 9
#define YELLOW 10
#define BLUE 11
#define PINK 12
#define AQUA 13
#define WHITE 14
#define BLACK 15

typedef struct TextCell {u8 bg,fg,ch;} cell;
typedef struct GrBuffer {cell *pal; u32 w,h;} gr;

extern char *grout; //converted sequences get stored here before being printed
extern u32 groff; //gr offset. How filled grout is

void GrInit(uvec2 res);

void GrCell(gr *buf, int x, int y, cell cl);

static inline void GrFree(gr *buf) {
	free(buf->pal);
}

gr GrBuffer(u32 width, u32 height);

void GrFill(gr *buf, const cell cl);

void gremit(u32 val, const u32 end);

void draw(gr *buf, uvec2 offset, uvec2 res);
#endif
