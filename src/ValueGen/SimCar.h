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

        void addSensor(int id, Sensor* sensor);
        void SimCar::addAttribute(int id, float* value);

        float* SimCar::addAttribute(int id);
        int* getNextSensorValues(int sensor, int count);
    private:
        std::map<int, Sensor*> sensorMap;
        std::map<int, int*> iterMap;
        std::map<int, float*> attributeMap;
    };
}


#endif //VALGEN_SIMCAR_H
