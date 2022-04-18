#include "Elafri.h"
#include <unistd.h>
#include <termios.h> //for term flags
#include <sys/ioctl.h>
#include "evar.h"
#include "graphics.h"

u8 kbd[64]; //parsed keyboard input
u32 kbdSize; //size of input buffer
uvec2 res; //term resolution

struct termios oldTermFlags;

void GetResolution() {
	struct winsize r;
	ioctl(0, TIOCGWINSZ, &r);
	res.x = r.ws_col;
	res.y = r.ws_row-1;
}

void ElafriInit() { //Init procedures
	tcgetattr(0, &oldTermFlags); //backup flags
	struct termios raw = oldTermFlags;
	raw.c_lflag &= ~(ECHO | ICANON | ISIG); //turn off some flags
	tcsetattr(0, TCSAFLUSH, &raw);

	write(0,"\e[?25l",7); //enable mouse tracking and hide term cursor
	GetResolution();
	GrInit(res);
}

void Exit() {
	write(0,"\e[?25h\e[2J\e[H",14);
	tcsetattr(0, TCSAFLUSH, &oldTermFlags); //reset terminal flags
	exit(0);
}

void Input(struct timeval *mtv) { //read stdin and sort kbd and mouse input
	u8 input[128]; //we read stdin to here
	kbdSize=0;
	fd_set fdread;
	FD_ZERO(&fdread); //clear select set
	FD_SET(0,&fdread); //add fd 0 (stdin)
	if (select(1,&fdread,NULL,NULL,mtv)>0) { //check if stdin has data
		const u32 insz = read(0,input,128); //128 bytes should be enough
		for (u32 i=0; i<insz; i++) {
			printf("%d ", input[i]);
		}
		if (insz) printf("\n");
		for (u32 i=0; i<insz; i++) {
			if (input[i]==3) Exit(); //ctrl + C
			if (input[i]==27) { //ESC
				kbd[kbdSize] = 255; //prepare just in case
				if (input[i+1]==79) {//F1-4 are ordered as 0-3
					kbd[kbdSize+1] = input[i+2]-80;
					kbdSize++;
					i+=2;
				} else
					if (input[i+1]==91) {
						kbdSize++;
						switch (input[i+2]) {
							case 49: //F5-8 are ordered as 4-7
								kbd[kbdSize] = input[i+3]-49-(input[i+3]>53);
								i+=4;
								break;
							case 50:
								if (input[i+3]!=126) {
									if (input[i+4]==126) //F9-12 are ordered as 8-11
										kbd[kbdSize] = input[i+3]-40-(input[i+3]>50);
									i+=4;
									break;
								}
							default:
								kbd[kbdSize] = input[i+2];
								i+=3-(input[i+2]>64);
						}
					} else kbd[kbdSize] = input[i]; //1 byte
			} else kbd[kbdSize] = input[i]; //1 byte
			kbdSize++;
		}
	}
}
