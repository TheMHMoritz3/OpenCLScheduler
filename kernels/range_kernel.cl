/*   Errechnet die verbleibende fahrbare Distanz anhand der aktuellen Verbrauchswerte
 *      input: Der Batteriestatus
 *
 *      result: Das errechnete mögliche Distanz
 *
 *      wLoad:   Wert 1 für voll parallele Ausführung, Wert MAX_ITEM_SIZE_PER_WORKGROUP für vollständig sequenzielle Ausführung.
 *
 *      Assumptions:
 *          Maximum Battery Voltage = 7,5v
 *          Minimum Battery Voltage = 5,5v
 *          Charge of Battery lasts 2h
 */
__kernel void distance(global const float* inputVoltage, global const float* speed, global float* result, const int WLOAD) {
    int gid = get_global_id(0) * WLOAD;
    int const maxDuration = 2;
    float const minVoltage = 5.5;
    float const maxVoltage = 7.5;
    float const maxVoltageDelta = maxVoltage - minVoltage;

    for (int i = 0; i < WLOAD; ++i){
        float voltageDelta = maxVoltage - inputVoltage[gid+i];
        float percentage = maxVoltageDelta/voltageDelta;
        result[gid+i] = speed[gid+i]*percentage*maxDuration;
    }
}