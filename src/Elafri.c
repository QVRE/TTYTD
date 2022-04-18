#include "Elafri.h"
#include <unistd.h>
#include <termios.h> //for term flags
#include <sys/ioctl.h>
#include "evar.h"
#include "graphics.h"

uvec2 GetResolution() {
	uvec2 res;
	struct winsize r;
	ioctl(0, TIOCGWINSZ, &r);
	res.x = r.ws_col;
	res.y = r.ws_row-1;
	return res;
}

elafriContext ElafriInit() { //Init procedures
	elafriContext current;
	tcgetattr(0, &current.oldTermFlags); //backup flags
	struct termios raw = current.oldTermFlags;
	raw.c_lflag &= ~(ECHO | ICANON | ISIG); //turn off some flags
	tcsetattr(0, TCSAFLUSH, &raw);

	write(0,"\e[?25l",7); //enable mouse tracking and hide term cursor
	current.res = GetResolution();
	GrInit(current.res);
	return current;
}

void Exit(elafriContext to_exit) {
	write(0,"\e[?25h\e[2J\e[H",14);
	tcsetattr(0, TCSAFLUSH, &to_exit.oldTermFlags); //reset terminal flags
	exit(0);
}

u32 Input(u8 kbd[]) { //read stdin and sort kbd input
	struct timeval mtv = {0,1}; //max sleep time
	fd_set fdread;
	FD_ZERO(&fdread); //clear select set
	FD_SET(0,&fdread); //add fd 0 (stdin)
	if (select(1,&fdread,NULL,NULL,&mtv)<=0) return 0; //check if stdin has data
	u8 input[128]; //we read stdin to here
	int kbdSize=0;
	const u32 insz = read(0,input,128); //128 bytes should be enough
	for (u32 i=0; i<insz; i++) {
		if (input[i]==27) { //ESC
			kbd[kbdSize] = 255; //prepare just in case
			if (input[i+1]=='O') {//F1-4 are ordered as 0-3
				kbd[kbdSize+1] = input[i+2]-80;
				kbdSize++;
				i+=2;
			} else if (input[i+1]=='[') {
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
	return kbdSize;
}

void MainLoop(frameHandler callback,elafriContext current) {

	/*Timekeeping-related Inits*/
	InitTimer(ftimer); //timer
	u32 fps = current.max_fps;
	int exec_time, wait_time;
	F32 delta=0; //Δt per frame

	RenderLoop:
		StartTimer(ftimer);

		current.kbdSize = Input(current.kbd); //get user input since last frame
		callback(delta, &current);
		draw(&current.Gr, (uvec2){0}, current.res); //outputs buffer

		StopTimer(ftimer);
		exec_time = mod32(dt_usec(ftimer),1000000); //Compute execution Δt
		wait_time = 1000000/fps-exec_time; //Check if behind/ahead
		fps = max(min(1000000*fps/(1000000-wait_time*fps), current.max_fps), 1); //Compute new fps
		delta = 1. / fps; //sligtly accurate Δt

		ftimer_tend.tv_usec = max(1000000/current.max_fps-exec_time, 0);
		ftimer_tend.tv_sec=0;
		select(1, NULL, NULL, NULL, &ftimer_tend); //wait if over max fps
	goto RenderLoop;
}
