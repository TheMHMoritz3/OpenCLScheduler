int inRange(float value, float min, float max){
    if(value>max) return 1;
    if(value<min) return -1;
    return 0;
}

/*   Berechnet den Temperatur Trend
 *      input:      Die Temperatur aus dem temp_kernel
 *      minTemp:    die minimale Temperatur als einzel Wert
 *      maxTemp:    die maximale Temperatur als einzel Wert
 *
 *      result:     Das Ergebnis, bzw ob man über unter oder in der gewünschten Temperatur Zone liegt:
 *                  -1: unter dem unteren Schwellwert
 *                  0: in Range
 *                  1: über dem oberen Schwellwert
 *
 *      wLoad:      Wert 1 für voll parallele Ausführung, Wert MAX_ITEM_SIZE_PER_WORKGROUP für vollständig sequenzielle Ausführung.
 */
__kernel void tempRange(global const unsigned float* input, global const float min, global const float max, global int* result, const int wLoad) {
    int gid = get_global_id(0) * wLoad;

    for (int i = 0; i < wLoad; ++i){
        unsigned int in = input[gid + i];

        result[gid+i] = inRange(input[gid+i], min, max);
    }
} 

//Mit Array Min,Max
__kernel void temp1(global const unsigned float* input, global const float* min, global const float*max, global int* result, const int wLoad) {
    int gid = get_global_id(0) * wLoad;

    for (int i = 0; i < wLoad; ++i){
        unsigned int in = input[gid + i];

        result[gid+i] = inRange(input[gid+i], min[gid+i], max[gid+i]);
    }
}
