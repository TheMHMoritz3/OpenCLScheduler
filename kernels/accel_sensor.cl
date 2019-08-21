float alpha(int input) {
    return (input - 2048.0f)/256.0;
}

/*   Berechnet die Beschleunigung auf der X-Achse
 *      input: Die Sensor Daten der x-Achse
 *
 *      result: Das errechnete Ergebnis
 *
 *      wLoad:   Wert 1 für voll parallele Ausführung, Wert MAX_ITEM_SIZE_PER_WORKGROUP für vollständig sequenzielle Ausführung.
 */
__kernel void xAxis(global const unsigned int* input, global float* result, const int WLOAD) {
	int gid = get_global_id(0) * WLOAD;

	for (int i = 0; i < WLOAD; ++i)
       result[gid + i]= asin(alpha(input[gid+i]));
}

/*   Berechnet die Beschleunigung auf der Y-Achse
 *      input: Die Sensor Daten der y-Achse
 *
 *      result: Das errechnete Ergebnis
 *
 *      wLoad:   Wert 1 für voll parallele Ausführung, Wert MAX_ITEM_SIZE_PER_WORKGROUP für vollständig sequenzielle Ausführung.
 */
__kernel void yAxis(global const unsigned int* A, global float* result, const int WLOAD) {
	int gid = get_global_id(0) * WLOAD;

	for (int i = 0; i < WLOAD; ++i)
        result[gid + i]= acos(alpha(A[gid+i]));
}

/*   Berechnet die Beschleunigung auf der Dual-Achse
 *      x_axis: Die Sensor Daten der x-Achse
 *      y_axis: Die Sensor Daten der y-Achse
 *
 *      result: Das errechnete Ergebnis
 *
 *      wLoad:   Wert 1 für voll parallele Ausführung, Wert MAX_ITEM_SIZE_PER_WORKGROUP für vollständig sequenzielle Ausführung.
 */
__kernel void dualAxis(global const unsigned int* x_axis, global const unsigned int* y_axis, global float* result, const int WLOAD) {
	int gid = get_global_id(0) * WLOAD;

	for (int i = 0; i < WLOAD; ++i)
        result[gid + i]= atan(alpha(x_axis[gid+i])/alpha(y_axis[gid+i]));
}
