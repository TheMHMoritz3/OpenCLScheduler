//Currently No Constant Values are supported

__kernel void distanceTracker(global const unsigned int* wSpeedMedian, global float* output, const int WLOAD) {
	int gid = get_global_id(0) * WLOAD;
	float distance = 10;
	float time = 10;

	for(int i=0; i<WLOAD; i++)
		output[gid+i] = distance + wSpeedMedian[gid+i] * time/ 10000;
}
