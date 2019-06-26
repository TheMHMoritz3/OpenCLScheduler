int inRange(int value, const int min, const int max){
    if(value>max) return 2;
    if(value<min) return 3;
    return 1;
}

/*   Prüft ob die Hinterreifen Grip haben.
 *      speed[F,RL,RR]: Der Speed-Wert der Hinterräder und der Mittelwert der beiden vorderen Räder.
 *
 *      resultRR: Falls der rechte Hinterreifen
 *          1: Grip hat
 *          2: sich zu schnell dreht
 *          3: blockiert
 *      resultRL: Falls der linke Hinterreifen
 *          1: Grip hat
 *          2: sich zu schnell dreht
 *          3: blockiert
 *
 *      wLoad:   Wert 1 für voll parallele Ausführung, Wert MAX_ITEM_SIZE_PER_WORKGROUP für vollständig sequenzielle Ausführung.
 */
__kernel void tractionControl(global const float* speedF, global const float* speedRL, global const float* speedRR, global int* resultRR, global int* resultRL, const int wLoad){
    int gid = get_global_id(0) * wLoad;
    float maxdiff = 0.4f;

    for (int i = 0; i < wLoad; ++i){
        resultRL[git+i] = inRange(speedF, speedRL*maxdiff, speedRL*(1+maxdiff));
        resultRR[git+i] = inRange(speedF, speedRR*maxdiff, speedRR*(1+maxdiff));
    }
}