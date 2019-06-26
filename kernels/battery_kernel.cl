/*   Berechnet den Batterie Status
 *      input:      Der Wert des Batterie Sensors
 *
 *      resuult:    Das Ergebnis
 *
 *      wLoad:      Wert 1 für voll parallele Ausführung, Wert MAX_ITEM_SIZE_PER_WORKGROUP für vollständig sequenzielle Ausführung.
 */
__kernel void batteryCalc(global const unsigned int* input, global float* result, const int wLoad) {
	int gid = get_global_id(0) * wLoad;

	for (int i = 0; i < wLoad; ++i)
		result[gid+i]= (input[gid+i] * 3.7f * 5.0f) / 1024.0f;
}
