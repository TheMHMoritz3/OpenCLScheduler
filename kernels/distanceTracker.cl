__kernel void distanceTracker(global float* distance, global float* time, global const unsigned int* wSpeedMedian, global float* output, const int WLOAD) {
	int gid = get_global_id(0) * WLOAD;
	for(int i=0; i<WLOAD; i++)
		output[gid+i] = distance[gid+i] + wSpeedMedian[gid+i] * time[gid+i] / 10000;
}
