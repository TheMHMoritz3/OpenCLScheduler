﻿/*   Berechnet die Board Temperatur
 *      input:      Der Wert des Temperatur Sensors
 *
 *      result:     Das Ergebnis
 *
 *      wLoad:      Wert 1 für voll parallele Ausführung, Wert MAX_ITEM_SIZE_PER_WORKGROUP für vollständig sequenzielle Ausführung.
 */
__kernel void temp(global const unsigned int* input, global float* result, const int wLoad) {
    int gid = get_global_id(0) * wLoad;

    for (int i = 0; i < wLoad; ++i){
        if (input[gid + i] < 500.0f)
            result[gid + i] = input[gid + i] / 4.0f;
        else
            result[gid + i] = (input[gid + i] - 1024.0f) / 4.0f;
	    
    }
}


__kernel void temInformation(global const float* temperature, const float min, const float max, global float* result, const int wLoad){
	int gid = get_global_id(0) * wLoad;
	for (int i = 0; i < wLoad; ++i){
		if(temperature[gid + i] < min){
			result[gid + i] = -1;
		}else if(temperature[gid + i] > max){
			result[gid + i] = 1;
		}else{
			result[gid + i] = 0;
		}
	}
}