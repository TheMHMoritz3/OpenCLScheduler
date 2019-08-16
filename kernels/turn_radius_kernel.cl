/*   Berechnet den WendeRadius abhängig vom WheelSpeed
 *      speedL:     Der Speed-Wert des Rads vorne links
 *      speedR:     Der Speed-Wert des Rads vorne Rechts
 *
 *      resuult:    Das Ergebnis
 *
 *      wLoad: Wert 1 für voll parallele Ausführung, Wert MAX_ITEM_SIZE_PER_WORKGROUP für vollständig sequenzielle Ausführung.
 */
__kernel void radius(global const float* speedL, global const float* speedR, global float* result, const int WLOAD){
    int gid = get_global_id(0) * WLOAD;
    const float axleSize = 0.05f;

    for (int i = 0; i < WLOAD; ++i){
        float res = (axleSize/2.0f) * ((speedL[gid + i] + speedR[gid+i])/(speedL[gid + i] - speedR[gid+i]));

        if(res<0)
            result[gid + i] = 0.0f - res;
        else
            result[gid + i] = res;
    }
}