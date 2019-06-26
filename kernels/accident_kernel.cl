int inRange(int value, const int min, const int max){
    if(value>max) return -1;
    if(value<min) return -2;
    return 0;
}

/*   Prüft ob die Accel-Werte zwischen zwei Werten liegt
 *       input:  Ein Array mit den letzten n Datenpunkten aus dem accel_kernel.cl, auf der x-Achse
 *
 *       result: Das Array mit den entsprechenden Ergebniseen:
 *           0 = In Range
 *          -1 = Over Max
 *          -2 = Below Min
 *
 *       min:  Eine minimale Beschleunigung die unter normalen Bedingungen nicht unterschritten werden kann
 *       max:  Eine maximale Beschleunigung die unter normalen Bedingungen nicht überschritten werden kann
 *
 *       wLoad: Wert 1 für voll parallele Ausführung, Wert MAX_ITEM_SIZE_PER_WORKGROUP für vollständig sequenzielle Ausführung.
 */
__kernel void accidentConst(global const float* input, global int* result, global const int min, global const int max, const int wLoad) {
    int gid = get_global_id(0) * wLoad;

    for (int i = 0; i < wLoad; ++i)
        result[gid+i] = inRange( input[gid+i], min, max);
}

//TODO: Insert peak detection