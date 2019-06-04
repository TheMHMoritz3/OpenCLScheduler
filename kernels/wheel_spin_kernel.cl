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
		B[gid + i]= (2000000*2*M_PI*6)/(100*value[i]*26);
	}
}

__kernel void median(global const float* speed1, global const float* speed2, global float* B, const int WLOAD) {
	int gid = get_global_id(0) * WLOAD;
	int i;

	for (i = 0; i < WLOAD; ++i)
	{
		B[gid + i] = (speed1[gid + i] + speed2[gid+i]) / 2.0f;
	}
}

__kernel void tractionControl(global const float* speedFL, global const float* speedFR, global const float* speedRL, global const float* speedRR, global int* Output, const int WLOAD){
    int gid = get_global_id(0) * WLOAD;
    int i;

    for (i = 0; i < WLOAD; ++i)
    {
        if(!(((speedRL[gid + i]-speedRR[gid + i])<0.01f)&&((speedRR[gid + i]-speedRL[gid + i])<0.01f)||(((speedRL[gid + i]-speedFL[gid + i])<0.01f)&&((speedRR[gid + i]-speedFR[gid + i])<0.01f)))){
            if((speedRL[gid + i]-speedRR[gid + i])>=0.01f)
            {
                Output[gid+i]=1;
            }
            if((speedRR[gid + i]-speedRL[gid + i])>=0.01f)
            {
                Output[gid+i]=0;
            }
            if(((speedRL[gid + i]-speedFL[gid + i])>=0.01f)&&((speedRR[gid + i]-speedFR[gid + i])>=0.01f))
            {
                Output[gid+i]=2;
            }
        }else{
            Output[gid+i]=3;
        }
    }
}
