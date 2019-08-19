__kernel void distanceTracker(global const unsigned int* wSpeedMedian, global const float* distance, global const float* time, global float* output, const int WLOAD) {
	int gid = get_global_id(0) * WLOAD;

	for(int i=0; i<WLOAD; i++)
		output[gid+i] = distance[gid+i] + wSpeedMedian[gid+i] * time[gid+i]/ 10000;
}
