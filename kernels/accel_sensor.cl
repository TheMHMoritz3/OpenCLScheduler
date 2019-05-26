float alpha(int input) {
    return (input - 2048.0f)/256.0;
}

__kernel void xAxis(global const unsigned int* A, global float* B, const int WLOAD) {
	int gid = get_global_id(0) * WLOAD;
	int i;

	for (i = 0; i < WLOAD; ++i)
	{
		B[gid + i]= asin(alpha(A[gid+i]));
	}
}

__kernel void yAxis(global const unsigned int* A, global float* B, const int WLOAD) {
	int gid = get_global_id(0) * WLOAD;
	int i;

	for (i = 0; i < WLOAD; ++i)
	{
		B[gid + i]= acos(alpha(A[gid+i]));
	}
}

__kernel void dualAxis(global const unsigned int* x_axis, global const unsigned int* y_axis, global float* B, const int WLOAD) {
	int gid = get_global_id(0) * WLOAD;
	int i;

	for (i = 0; i < WLOAD; ++i)
	{
		B[gid + i]= atan(alpha(x_axis[gid+i])/alpha(y_axis[gid+i]));
	}
}
