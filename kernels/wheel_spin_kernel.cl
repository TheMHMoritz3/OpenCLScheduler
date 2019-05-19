__kernel void speedCalcul(global const unsigned int* A, global float* B, const int WLOAD) {

	int gid = get_global_id(0) * WLOAD;
	int i;

	int size = 4;
  char array[4];
  float value[10];

  for (i = 0; i < WLOAD; i++)
  {
    for (int j = 0; j < size; j++)
    {
      array[j] = (A[gid + i] >> (8 * j)) & 0xff;
    }
    value[i]=((array[2])*65536.0f + (array[1])*256.0f + array[0] );
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
	    printf("Speed 1: %f Speed 2: %f\n",speed1[gid + i],speed2[gid+i]);
		B[gid + i] = (speed1[gid + i] + speed2[gid+i]) / 2.0f;
	}
}
