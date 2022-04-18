#include "Elafri.h"
#include "graphics.h"
#include "evar.h"

void frame(F32 delta, elafriContext* curr);

int main() {
	elafriContext cont = ElafriInit(); //calls all the init functions
	cont.Gr = GrBuffer(cont.res.x, cont.res.y);
	cont.max_fps = 20;
	MainLoop(&frame, cont);
}

void frame(F32 delta, elafriContext* current) {
	if(current->kbdSize && current->kbd[0] == 3)
		Exit(*current);
}
