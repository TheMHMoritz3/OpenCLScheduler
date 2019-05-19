#include <math.h>

__kernel void accelCalc(global const unsigned int* A, global float* B, const int WLOAD) {
	int gid = get_global_id(0) * WLOAD;
	int i;

	for (i = 0; i < WLOAD; ++i)
	{
		B[gid + i]= (A[gid + i] - 2048.0f) / 256.0f;
	}
}
