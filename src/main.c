#include "Elafri.h"
#include "graphics.h"
#include "evar.h"

#define FPS 60
#define MPS 1000000 // s/μs

int main()
{
	ElafriInit(); //calls all the init functions

	gr Gr = GrBuffer(res.x,res.y); //allocates drawing buffer

	/*Timekeeping-related Inits*/
	InitTimer(ftimer); //timer
	u32 fps = FPS;
	int exec_time, wt;
	F32 delta=0; //Δt per frame

	RenderLoop:
		StartTimer(ftimer);

		Input(); //get user input since last frame
		
		draw(&Gr, (uvec2){0}, res); //outputs buffer

		StopTimer(ftimer);
		exec_time = mod32(dt_usec(ftimer),MPS); //Compute execution Δt
		wt = MPS/fps-exec_time; //Check if behind/ahead
		fps = max(min(MPS*fps/(MPS-wt*fps), FPS), 1); //Compute new fps
		delta = 1. / fps; //sligtly accurate Δt

		ftimer_tend.tv_usec = max(MPS/FPS-exec_time, 0);
		ftimer_tend.tv_sec=0;
		select(1, NULL, NULL, NULL, &ftimer_tend); //wait if over max fps
	goto RenderLoop;
}
