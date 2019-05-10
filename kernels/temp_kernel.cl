__kernel void temp(global const unsigned int* A, global float* B, const int WLOAD) {
	int gid = get_global_id(0) * WLOAD;
	int i;

	for (i = 0; i < WLOAD; ++i)
	{

	    if((A[gid + i] / 4.0f)<125.0f)
		    B[gid + i]= A[gid + i] / 4.0f;
		else
		    B[gid + i]= (A[gid + i]-1024.0f) / 4.0f;
	}
}
