/*   Errechnet die verbleibende fahrbare Distanz anhand der aktuellen Verbrauchswerte
 *      input: Der Batteriestatus
 *
 *      result: Das errechnete mögliche Distanz
 *
 *      wLoad:   Wert 1 für voll parallele Ausführung, Wert MAX_ITEM_SIZE_PER_WORKGROUP für vollständig sequenzielle Ausführung.
 */
__kernel void distance(global const float* input, global float* result, const int WLOAD) {
    int gid = get_global_id(0) * WLOAD;
        //TODO insert Formula
    for (int i = 0; i < WLOAD; ++i)
        result[gid+i] = input[gid+i];
}