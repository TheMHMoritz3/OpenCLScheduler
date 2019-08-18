/*   Berechnet die Geschwindigkeit eines Rads
 *      input: Die Sensor Daten der RadDrehzahl
 *
 *      result: Das errechnete WheelSpeed-Ergebnis
 *
 *      wLoad:   Wert 1 für voll parallele Ausführung, Wert MAX_ITEM_SIZE_PER_WORKGROUP für vollständig sequenzielle Ausführung.
 */
__kernel void speedCalc(global const unsigned int* input, global float* result, const int WLOAD) {
    int gid = get_global_id(0) * WLOAD;

    char array[4];

    for (int i = 0; i < WLOAD; i++){
        unsigned int in = input[gid+i];
        array[0] = (in >> (0)) & 0xff;
        array[1] = (in >> (8)) & 0xff;
        array[2] = (in >> (16)) & 0xff;
        array[3] = (in >> (24)) & 0xff;
        result[gid+i] = (2000000*2*M_PI*6)/(100*((array[2])*65536.0f + (array[1])*256.0f + array[0] )*26);
    }
}

/*   Berechnet den Median der Geschwindigkeit zweier Räder
 *      speed[1,2]: Die errechneten WheelSpeeds von zwei Rädern
 *
 *      result: Der Mittelwert der WheelSpeeds
 *
 *      wLoad:   Wert 1 für voll parallele Ausführung, Wert MAX_ITEM_SIZE_PER_WORKGROUP für vollständig sequenzielle Ausführung.
 */
__kernel void median(global const float* speed1, global const float* speed2, global float* result, const int WLOAD) {
    int gid = get_global_id(0) * WLOAD;

    for (int i = 0; i < WLOAD; ++i)
        result[gid+i] = (speed1[gid+i] + speed2[gid+i]) / 2.0f;
}
