#include <unistd.h>
#include "graphics.h"
#include "evar.h"

char *grout; //converted sequences get stored here before being printed
u32 groff; //gr offset. How filled grout is

void GrInit(uvec2 res) {
	grout = malloc(7 + 23*res.x*res.y + res.y);
	grout[0] = '\e', grout[1] = '[', grout[2] = 'H'; //first 3 bytes always do home escape code
}

gr GrBuffer(u32 width, u32 height) {
	return (gr){calloc(width*height, sizeof(cell)), width, height};
}
void GrCell(gr *buf, int x, int y, cell cl) {
	if (x >= 0 && y >= 0 && x < (int)buf->w && y < (int)buf->h)
		buf->pal[y*buf->w+x] = cl;
}
void GrFill(gr *buf, const cell cl) {
	cell *ptr = buf->pal; //point to start of graphics buffer
	u32 size = buf->w*buf->h;
	while (size--)
		*ptr++ = cl;
}

void gremit(u32 val, const u32 end) { //convert u8 to ASCII decimal + add an extra char
	u32 pair = end; //lower part is set to ending character. Things are printed low to high
	u32 size = 1;
	while (val) {
		pair <<= 8;
		pair ^= '0' + val % 10;
		val /= 10;
		size++;
	}
	*(u32*)(&grout[groff]) = pair; //emit pair to grout
	groff += size;
}
void draw(gr *buf, uvec2 offset, uvec2 res) {
	groff = 3; //first 3 bytes will always be the escape code to go to 0,0
	u8 last_fg = WHITE, last_bg = BLACK;
	u32 x=0, y=0;
	cell c;

	draw:
	c = buf->pal[(y+offset.y)*buf->w + x + offset.x];
	if (c.bg != last_bg) {
		*(u64*)(&grout[groff]) = 0x3b353b38345b1b; // \e[48;5; backwards
		groff += 7;
		gremit(c.bg+1, 'm'); //emit color value in ascii
	}
	if (c.fg != last_fg) {
		*(u64*)(&grout[groff]) = 0x3b353b38335b1b; // \e[38;5; backwards
		groff += 7;
		gremit(c.fg+1, 'm'); //emit color value in ascii
	}
	last_fg = c.fg, last_bg = c.bg;
	grout[groff] = (!c.ch * ' ') ^ c.ch; //output char (NUL is space)
	groff++;
	x++;
	if (x == res.x) { //at end of line do newline
		grout[groff] = '\n';
		groff++;
		x=0, y++;
		if (y == res.y) { //check if this is the last line
			*(u32*)(&grout[groff]) = 0x6d305b1b; // \e[0m (set color to default)
			groff += 4;
			write(0,grout,groff); //finally output our buffer
			return;
		}
	}
	goto draw;
}
