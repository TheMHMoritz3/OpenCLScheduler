__kernel void batteryCalc(global const unsigned int* A, global float* B, const int WLOAD) {
	int gid = get_global_id(0) * WLOAD;
	int i;
	
	for (i = 0; i < WLOAD; ++i)
	{
		B[gid + i]= (A[gid + i] * 3.7f * 5.0f) / 1024.0f;		
	}
}