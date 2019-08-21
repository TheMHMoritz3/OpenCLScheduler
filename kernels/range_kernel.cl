/*   Errechnet die verbleibende fahrbare Distanz anhand der aktuellen Verbrauchswerte
 *      input: Der Batteriestatus
 *      maxDur: maximale Duration als einzelner Wert
 *      minVol: minimale Voltage als einzelner Wert
 *      maxVol: maximale Voltage als einzelner Wert
 *
 *      result: Die errechnete mögliche Distanz
 *
 *      wLoad:  Wert 1 für voll parallele Ausführung, Wert MAX_ITEM_SIZE_PER_WORKGROUP für vollständig sequenzielle Ausführung.
 *
 *      Assumptions:
 *          Maximum Battery Voltage = 7,5v
 *          Minimum Battery Voltage = 5,5v
 *          Charge of Battery lasts 2h
 */
__kernel void distance(global const float* inputVoltage, global const float* speed, const int duration, const float minVoltage, const float maxVoltage, global float* result, const int WLOAD) {
    int gid = get_global_id(0) * WLOAD;
	
    float maxVoltageDelta = maxVoltage - minVoltage;
    
    for (int i = 0; i < WLOAD; ++i){
        float voltageDelta = maxVoltage - inputVoltage[gid+i];
        float percentage = voltageDelta/maxVoltageDelta;
        if(speed[gid+i]>1)
            result[gid+i] = speed[gid+i]*percentage*maxDuration;
        else
            result[gid+i] = percentage*maxDuration;
    }
}

//Als Array
__kernel void dist1(global const float* inputVoltage, global const float* speed, global const int* maxDur, global const float* minVol, global const float* maxVol, float* result, const int WLOAD) {
    int gid = get_global_id(0) * WLOAD;

    for (int i = 0; i < WLOAD; ++i){
        float voltageDelta = maxVol[gid+i] - inputVoltage[gid+i];
        float percentage = voltageDelta/(maxVol[gid+i]-minVol[gid+i]);
        if(speed[gid+i]>1)
            result[gid+i] = speed[gid+i]*percentage*maxDur[gid+i];
        else
            result[gid+i] = percentage*maxDur[gid+i];
    }
}
