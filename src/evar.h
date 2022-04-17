#ifndef _EVAR //file made to keep Elafri addons more modular
#define _EVAR //make sure to compile with -flto to remove unnecessary garbage
#include <sys/select.h>
#include <sys/time.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>

#define u8 uint8_t
#define u16 uint16_t
#define u32 uint32_t
#define u64 uint64_t
#define F32 float_t //not always 32 (most likely 64) but uh works

#define max(x,y) ((x) > (y) ? (x) : (y))
#define min(x,y) ((x) < (y) ? (x) : (y))
#define pi 3.1415926535897932384626433832795

#define InitTimer(t) struct timeval t ## _tstart, t ## _tend
#define StartTimer(t) gettimeofday(&t ## _tstart, NULL)
#define StopTimer(t) gettimeofday(&t ## _tend, NULL)
#define dt_sec(t) (t ## _tend.tv_sec-t ## _tstart.tv_sec)
#define dt_usec(t) (t ## _tend.tv_usec-t ## _tstart.tv_usec)
#define Sleep(time) select(1, NULL, NULL, NULL, &time)

typedef struct UVector2 {u32 x,y;} uvec2;
typedef struct IVector2 {int x,y;} ivec2;

static inline int mod32(int x, int m) { //actual modulo for negative numbers
	return (x%m + m)%m;
}
#endif
