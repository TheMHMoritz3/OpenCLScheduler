__kernel void cruiseControl(global int* limit, global const unsigned int* accelSensor, global const float* wSpeedmedian, global float* result, const int WLOAD) {
	int gid = get_global_id(0) * WLOAD;
	float epsilon = 1e-3;

	for(int i=0; i<WLOAD; i++) {
        float angle = accelSensor[gid+i];
		float velocity = wSpeedmedian[gid+i];

        if ((angle < epsilon || angle > -epsilon) && (velocity > limit[gid+i] - epsilon && velocity < limit[gid+i] + epsilon)) result[gid+i] = 0;
        if (angle < epsilon && angle > -epsilon && velocity < limit[gid+i]) result[gid+i] = 1;
        if (angle < epsilon && angle > -epsilon && velocity > limit[gid+i]) result[gid+i] = -1;
        if (angle > epsilon && velocity < limit[gid+i]) result[gid+i] = 2;
        if (angle > epsilon && velocity > limit[gid+i]) result[gid+i] = 0;
        if (angle < -epsilon && velocity < limit[gid+i]) result[gid+i] = 1;
        if (angle < -epsilon && velocity > limit[gid+i]) result[gid+i] = -2;
    }
}
