__kernel void xAxis(global const unsigned int* A, global float* B, const int WLOAD) {
	int gid = get_global_id(0) * WLOAD;
	int i;

	for (i = 0; i < WLOAD; ++i)
	{
		B[gid + i]= asin((A[gid + i]- 2048.0f)/256.0f);
	}
}

__kernel void yAxis(global const unsigned int* A, global float* B, const int WLOAD) {
	int gid = get_global_id(0) * WLOAD;
	int i;

	for (i = 0; i < WLOAD; ++i)
	{
		B[gid + i]= acos((A[gid + i]- 2048.0f)/256.0f);
	}
}
