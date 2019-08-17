//
// Created by Over on 01.08.2019.
//

#ifndef VALGEN_SIMCAR_H
#define VALGEN_SIMCAR_H

#include "Sensor.h"
#include <map>

namespace ValueGen {
    class SimCar {
    public:
        SimCar();

        int* getNextSensorValues(int sensor, int count);
        void addSensor(int id, Sensor *sensor);
    private:
        std::map<int, Sensor*> sensorMap;
        std::map<int, int> iterMap;
    };
}


#endif //VALGEN_SIMCAR_H
