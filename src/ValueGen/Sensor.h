//
// Created by Over on 01.08.2019.
//

#include <cstdlib>
#include <random>

#ifndef VALGEN_SENSOR_H
#define VALGEN_SENSOR_H

namespace ValueGen {
    class Sensor {
    public:
        Sensor(int amount, int minL, int maxL, int minA, int maxA, int smooth, int stepS);
        void regenValues();
        int* getData();
        int getCount();
        static Sensor* generateWheelspeeds(Sensor main, Sensor curve, Sensor back);

    private:
        int nextRandi(int min, int max);
        std::default_random_engine generator;

        Sensor(int count, int* data);

        int amount;     //  Anzahl der Datenpunkte
        int maxLimit;   //	Obergrenze	differenz muss größer 2
        int minLimit;   //	Untergrenze
        int maxAccel;   //	muss positiv
        int minAccel;   //	muss negativ
        int smoothness; //	Mittelwert von smoothness Punkten nehmen um Spitzen zu glätten
        int stepSize;   // 	1/stepSize max dist von vorherigem Datenpunkt
        int* data;      //  results

    protected:
        Sensor();
    };
}


#endif //VALGEN_SENSOR_H
