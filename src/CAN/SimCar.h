//
// Created by Over on 01.08.2019.
//

#ifndef VALGEN_SIMCAR_H
#define VALGEN_SIMCAR_H

#include <iostream>
#include <cassert>
#include "Sensor.h"
#include <map>

namespace CAN {
    namespace ValueGen {
        class SimCar {
        public:
            SimCar();

            void addSensor(int id, Sensor *sensor);

            int *getNextSensorValues(int sensor, int count);

            std::map<int, Sensor *> getSensorMap();

            //AccidentKernel
            const int MIN_ACCI_CONST = -38;
            const int MAX_ACCI_CONST = 38;
            const int MIN_ACCI_PEAK = -4;
            const int MAX_ACCI_PEAK = 4;
            //CruiseControl
            const int TARGET_SPEED = 15; //m/s
            //Range
            const int MAX_DUR = 2;
            const float MIN_VOL = 5.5f;
            const float MAX_VOL = 7.5f;
            //TempRange
            const float MIN_TEMP = 0;//min -40
            const float MAX_TEMP = 80;//max 120
            //Traction
            const float TRAC_THRES = 0.4f;
            //TurnRadius
            const float AXLE_SIZE = 0.06f;//from Doc 0.06m
        private:
            std::map<int, Sensor *> sensorMap;
            std::map<int, int> iterMap;
        };
    }
}


#endif //VALGEN_SIMCAR_H
