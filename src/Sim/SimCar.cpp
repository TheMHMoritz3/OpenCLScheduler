//
// Created by Over on 01.08.2019.
//

#include "SimCar.h"

namespace ValueGen {
    SimCar::SimCar() {
        std::cout << "1.1: Car " << std::endl;

        /*Wheelspeed 8-11 = 0x008-0x00b
        * O*65536 + H*256 + L = pulsetime = (4.000.000cm*PI*0.06)/(26*wheelspeed)
        * rechte Radseite +- Kurvenradius
        * hinteren Räder +- traktionberechnung
        */
        Sensor main = Sensor(1000000, -25, 25, -2, 2, 10, 1); //-30 bis 30
        Sensor curve = Sensor(1000000, -10, 10, -1, 1, 10, 3);
        Sensor back = Sensor(1000000, -15, 15, -2, 2, 2, 1);
        Sensor *sensor = Sensor::generateWheelspeeds(main, curve, back);
        addSensor(8, &sensor[0]);
        addSensor(9, &sensor[1]);
        addSensor(10, &sensor[2]);
        addSensor(11, &sensor[3]);
        std::cout << "1.2: Created WheelSpeeds" << std::endl;

        /*Voltage 0x00d = 13
        *  Raw = (1024*V)/(3,7*5)
        *  Range: Von 15V auf 10V => 553 - 830
        */
        addSensor(13, new Sensor(1000000, 553, 830, -2, 1, 1, 278));
        std::cout << "1.3: Created Voltage" << std::endl;

        /*Accel_X 0x190 = 400
        *  RAW = sin(x)*256+2048
        *  Range: Von ? bis ? => ? - ?
        */
        Sensor *xAccelSensor = new Sensor(1000000, -45, 45, -5, 5, 10, 30);
        xAccelSensor->modifyForXAccel();
        addSensor(400, xAccelSensor);
        std::cout << "1.4: Created xAccel" << std::endl;
        /*Accel_Y 0x191 = 401
        *  RAW = cos(x)*256+2048
        *  Range: Von ? bis ? => ? - ?
        */
        Sensor *yAccelSensor = new Sensor(1000000, -45, 45, -5, 5, 10, 30);
        yAccelSensor->modifyForYAccel();
        addSensor(401, yAccelSensor);
        std::cout << "1.5: Created yAccel" << std::endl;

        /*Temp 0x192 = 402
        *  RAW = temp*4
        *  Range: -40 bis 120
        */
        auto *tempSensor = new Sensor(1000000, -40, 120, -1, 1, 5, 10);
        tempSensor->createTempOffset();
        addSensor(402, tempSensor);
        std::cout << "1.6: Created Temps" << std::endl;
    }

    void SimCar::addSensor(int id, Sensor *sensor) {
        std::cout << "1.x: Added " << id << std::endl;
        sensorMap.insert(std::pair<int, Sensor*>(id, sensor));
    }

    int* SimCar::getNextSensorValues(int sensor, int count) {
        //Prüfen, dass man noch in der ArrayRange ist
        //assert(iterMap.at(sensor)+count <= sensorMap.at(sensor)->getCount());
        return &(sensorMap.at(sensor)->getData()[iterMap.at(sensor)]);
    }

    std::map<int, Sensor *> SimCar::getSensorMap() {
        return sensorMap;
    }
}
