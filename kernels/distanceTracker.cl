__kernel void distanceTracker(global const unsigned int* wSpeedMedian, global float* output, float* distance, float* time, const int WLOAD) {
	int gid = get_global_id(0) * WLOAD;
	for(int i=0; i<WLOAD; i++)
		result[gid+i] = distance[gid+i] + wSpeedMedian[gid+i]*time[gid+i] / 10000;
}
