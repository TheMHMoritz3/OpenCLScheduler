__kernel void speedCalculation(global const unsigned int* A, global float* B, const int WLOAD) {
	int gid = get_global_id(0) * WLOAD;
	int i;

	for (i = 0; i < WLOAD; ++i)
	{
	    B[gid + i]= (4000000.0f * 360.0f) / (A[gid+i] * 26.0f);
	}
}

__kernel void median(global const float* speed1, global const float* speed2, global float* B, const int WLOAD) {
	int gid = get_global_id(0) * WLOAD;
	int i;

	for (i = 0; i < WLOAD; ++i)
	{
		B[gid + i]= (speed1[gid + i] + speed2[gid+i]) / 2f;
	}
}
