int inRange(float value, float min, float max){
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
 *      threshold: die maximale Differenz zwischen den Radgeschwindigkeiten (default 0.4)
 * 
 *     result:
 *          0: Fahrzeug hat Grip
 *          1: Rechter Hinterreifen dreht sich zu schnell
 *          2: Rechter Hinterreifen blockiert
 *          3: Linker Hinterreifen dreht sich zu schnell
 *          4: Linker Hinterreifen blockiert
 *          5: Rechter & Linker Hinterreifen drehen sich zu schnell
 *          6: Rechter & Linker Hinterreifen blockieren
 *          7: Rechter Hinterreifen dreht sich zu schnell und Linker Blockiert
 *          1: Rechter Hinterreifen blockiert und Linker dreht sich zu schnell
 *
 *      wLoad:   Wert 1 für voll parallele Ausführung, Wert MAX_ITEM_SIZE_PER_WORKGROUP für vollständig sequenzielle Ausführung.
 */
__kernel void tractionControl(global const float* speedF, global const float* speedRL, global const float* speedRR, const float threshold, global int* result, const int wLoad){
    int gid = get_global_id(0) * wLoad;
    float maxdiff = threshold;

    for (int i = 0; i < wLoad; ++i){
        int resultRL = inRange(speedF[gid+i], speedRL[gid+i]*maxdiff, speedRL[gid+i]*(1+maxdiff));
        int resultRR = inRange(speedF[gid+i], speedRR[gid+i]*maxdiff, speedRR[gid+i]*(1+maxdiff));

        if((resultRL == 1)&&(resultRR == 1))
            result[gid+i]=0;

        if((resultRL == 2)&&(resultRR == 1))
            result[gid+i]=1;

        if((resultRL == 3)&&(resultRR == 1))
            result[gid+i]=2;

        if((resultRL == 1)&&(resultRR == 2))
            result[gid+i]=3;

        if((resultRL == 1)&&(resultRR == 3))
            result[gid+i]=4;

        if((resultRL == 2)&&(resultRR == 2))
            result[gid+i]=5;

        if((resultRL == 3)&&(resultRR == 3))
            result[gid+i]=6;

        if((resultRL == 2)&&(resultRR == 3))
            result[gid+i]=7;

        if((resultRL == 3)&&(resultRR == 2))
            result[gid+i]=8;
    }
}
