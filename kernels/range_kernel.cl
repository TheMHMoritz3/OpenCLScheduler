/*   Errechnet die verbleibende fahrbare Distanz anhand der aktuellen Verbrauchswerte
 *      input: Der Batteriestatus
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
__kernel void dist(global const float* inputVoltage, global const float* speed, global float* result, const int WLOAD) {
    int gid = get_global_id(0) * WLOAD;

    int maxDuration = 2;
    float minVoltage = 5.5;
    float maxVoltage = 7.5;
    float maxVoltageDelta = maxVoltage - minVoltage;
    //TODO prüfen ob Formel bei kürzlichen Geschwindigkeitsänderungen nach längerer gleicher Geschwindigkeit Sinn macht
    for (int i = 0; i < WLOAD; ++i){
        float voltageDelta = maxVoltage - inputVoltage[gid+i];
        float percentage = voltageDelta/maxVoltageDelta;
        if(speed[gid+i]>1)
        {
            result[gid+i] = speed[gid+i]*percentage*maxDuration;
        }
        else
        {
            result[gid+i] = percentage*maxDuration;
        }
    }
}