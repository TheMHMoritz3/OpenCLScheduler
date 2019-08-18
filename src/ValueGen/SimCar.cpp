//
// Created by Over on 01.08.2019.
//

#include <cassert>
#include "SimCar.h"

int main(){
    auto* simCar = new ValueGen::SimCar();
    int* nr8 = simCar->getNextSensorValues(8, 1000000);
    int* nr9 = simCar->getNextSensorValues(9, 1000000);
    int* nr10 = simCar->getNextSensorValues(10, 1000000);
    int* nr11 = simCar->getNextSensorValues(11, 1000000);
    int* nr13 = simCar->getNextSensorValues(13, 1000000);
    int* nr400 = simCar->getNextSensorValues(400, 1000000);
    int* nr402 = simCar->getNextSensorValues(402, 1000000);
}
namespace ValueGen {
    SimCar::SimCar() {
        //Wheelspeed 8-11 = 0x008-0x00b
        // O*65536 + H*256 + L = pulsetime = (24.000.000cm*PI)/wheelspeed cm/50ms?
        // rechte Radseite +- Kurvenradius
        // hinteren Räder +- traktionberechnung
        Sensor main = Sensor(1000000, 0, 0, 0, 0, 10, 0);
        Sensor curve= Sensor(1000000, 0, 0, 0, 0, 7, 0);
        Sensor back = Sensor(1000000, 0, 0, 0, 0, 2, 0);
        Sensor *sensor = Sensor::generateWheelspeeds(main, curve, back);
        addSensor(8, &sensor[0]);
        addSensor(9, &sensor[1]);
        addSensor(10, &sensor[2]);
        addSensor(11, &sensor[3]);

        //Voltage 0x00d = 13
        //  Raw = (1024*V)/(3,7*5)
        //  Range: Von 15V auf 10V => 553 - 830
        addSensor(13, new Sensor(1000000, 553, 830, -2, 1, 0, 278));

        //Accel_X 0x190 = 400 TODO +AccelY mit cos statt sin
        //  RAW = sin(x)*256+2048
        //  Range: Von ? bis ? => ? - ?
        addSensor(400, new Sensor(1000000, 0, 0, 0, 0, 3, 0));

        //Temp 0x192 = 402
        //  RAW = temp*4
        //  !!Range nur 0-120, 40-1 entfällt!!
        addSensor(402, new Sensor(1000000, 0, 120, -1, 1, 5, 10));
    }

    int* SimCar::getNextSensorValues(int sensor, int count) {
        //Prüfen, dass man noch in der ArrayRange ist
        assert(iterMap.at(sensor)+count < sensorMap.at(sensor)->getCount());
        return &(sensorMap.at(sensor)->getData()[iterMap.at(sensor)]);
    }

    void SimCar::addSensor(int id, Sensor *sensor) {
        sensorMap.insert(std::pair<int, Sensor*>(id, sensor));
    }

}
