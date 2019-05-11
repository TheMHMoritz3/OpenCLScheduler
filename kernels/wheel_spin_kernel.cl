__kernel void speedCalculation(global const unsigned int* A, global float* B, const int WLOAD) {

	int gid = get_global_id(0) * WLOAD;
	int i;

	int size = 4;
    char array[4];

    int value[10];

    for (i = 0; i < WLOAD; i++)
    {
        for (int j = 0; i < size; i++)
        {
            array[i] = (A[gid + i] >> (8 * i)) & 0xff;
        }
        value[i]=((array[2])*65536 + (array[1])*256 + array[0] );
    }

	for (i = 0; i < WLOAD; ++i)
	{
		B[gid + i]= (value[i] * 3.7f * 5.0f) / 1024.0f;
	}
}

__kernel void median(global const float* speed1, global const float* speed2, global float* B, const int WLOAD) {
	int gid = get_global_id(0) * WLOAD;
	int i;

	for (i = 0; i < WLOAD; ++i)
	{
	    printf("Value: %f \n",speed1[gid + i]);
		B[gid + i] = (speed1[gid + i] + speed2[gid+i]) / 2.0f;
	}
}
