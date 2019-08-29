//Currently No Constant Values are supported

__kernel void distanceTracker(global const float* time, global const float* wSpeedMedian, global float* output, const int WLOAD) {
	int gid = get_global_id(0) * WLOAD;

	for(int i=0; i<WLOAD; i++){
		if(i==0){
			output[gid+i] = wSpeedMedian[gid+i] * time[gid+i]/ 10000;
		}else {
			output[gid+i] = output[gid+i-1] + wSpeedMedian[gid+i] * time[gid+i]/ 10000;
		}
	}
}
