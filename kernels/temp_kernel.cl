/*   Berechnet die Board Temperatur
 *      input:      Der Wert des Temperatur Sensors
 *
 *      resuult:    Das Ergebnis
 *
 *      wLoad:      Wert 1 für voll parallele Ausführung, Wert MAX_ITEM_SIZE_PER_WORKGROUP für vollständig sequenzielle Ausführung.
 */
__kernel void temp(global const unsigned int* input, global float* resultTemps, global int* resultRange, const int wLoad) {
    int gid = get_global_id(0) * wLoad;

    for (int i = 0; i < wLoad; ++i){
        unsigned int in = input[gid + i];

        if (in < 500.0f)
            resultTemps[gid + i] = in / 4.0f;
        else
            resultTemps[gid + i] = (in - 1024.0f) / 4.0f;
	
	if(resultTemps[gid + i]] < min){
	    resultRange[gid + i] = -1;
	}else if(resultTemps[gid + i] > max){
	    resultRange[gid + i] = 1;
	}else{
	    resultRange[gid + i] = 0;
	}    
     }
}
