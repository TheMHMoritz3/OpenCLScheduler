__kernel void xAxis(global const unsigned int* A, global float* B, const int WLOAD) {
	int gid = get_global_id(0) * WLOAD;
	int i;

	for (i = 0; i < WLOAD; ++i)
	{
		B[gid + i]= asin(accelCalculation(A[gid + i]);
	}
}

__kernel void yAxis(global const unsigned int* A, global float* B, const int WLOAD) {
	int gid = get_global_id(0) * WLOAD;
	int i;

	for (i = 0; i < WLOAD; ++i)
	{
		B[gid + i]= acos(accelCalculation(A[gid + i]);
	}
}

float accelCalculation(unsigned int value){
    return ((value - 2048.0f)/256.0f);
}