//
// Created by Over on 01.08.2019.
//

#include "Sensor.h"
#include <random>
#include <cassert>
#include <iostream>

using namespace CAN::ValueGen;

Sensor::Sensor(int count, int minL, int maxL, int minA, int maxA, int smooth, int stepS) {
    amount = count;
    minLimit = minL;
    maxLimit = maxL;
    minAccel = minA;
    maxAccel = maxA;
    smoothness = smooth;
    stepSize = stepS;
    data = new int[count];
    regenValues();
}

Sensor::Sensor(int count, int *data) {
    minLimit = 0;
    maxLimit = 0;
    minAccel = 0;
    maxAccel = 0;
    smoothness = 0;
    stepSize = 0;

    amount = count;
    this->data = data;
}

Sensor::Sensor() {
    amount = 0;
    minLimit = 0;
    maxLimit = 0;
    minAccel = 0;
    maxAccel = 0;
    smoothness = 0;
    stepSize = 0;
    data = nullptr;
}

int Sensor::nextRandi(int min, int max) {
    std::uniform_int_distribution<int> range(min, max);
    return range(generator);
}

void Sensor::regenValues() {
    int oldX = 0;
    int newX = nextRandi(minLimit, maxLimit);
    int transLength = 0;
    int oldY = 0;

	for(int i =0; i<amount; i++)
	{
		data[i] = nextRandi(minLimit, maxLimit);
	}

    //for (int i = 0; i < amount + smoothness * 2; i++) {
    //    if (oldY + transLength == i) {
    //        oldX = newX;
    //        int spread = abs(maxLimit - minLimit) / stepSize;
    //        if (spread == 0) spread = 2;
    //        int offset;
    //        if (newX < (spread / 2) + minLimit)
    //            offset = minLimit;
    //        else if (newX > maxLimit - (spread / 2))
    //            offset = maxLimit - (spread);
    //        else
    //            offset = newX - spread / 2;
    //        newX = nextRandi(offset, spread);
    //        oldY = i;
    //        //Aus der möglichen Distanz einen zufälligen Wert suchen
    //        if (oldX == newX) {
    //            transLength = smoothness;
    //        } else if (newX > oldX) {
    //            int minLength = abs((newX - oldX) / maxAccel);
    //            if (minLength == 0) minLength = 1;
    //            transLength = nextRandi(minLength, minLength * 3);
    //        } else {
    //            int minLength = abs((newX - oldX) / minAccel);
    //            if (minLength == 0) minLength = 1;
    //            //System.out.println("["+minLength+"]");
    //            transLength = nextRandi(minLength, minLength * 3);
    //        }
    //    }
    //    data[i] = (int) (oldX * (1.0 - ((double) (i - oldY) / (double) transLength)) +
    //                     newX * ((double) (i - oldY) / (double) transLength));
    //}
    //for (int i = 0; i < amount; i++) {
    //    int sum = 0;
    //    for (int j = 0; j < smoothness * 2; j++)
    //        sum += data[i + j];
    //    data[i] = sum / (smoothness * 2);
    //}
}

int *Sensor::getData() {
    return data;
}

int Sensor::getCount() {
    return amount;
}

//Hilfsfunktion für WheelSpeedumwandlung
uint32_t Sensor::encodePulseTime(int speed) {
    //std::cout<< speed <<";";
    if (speed == 0) {
        //TODO check
        return 0xffffffff;
    }
    long pulsetime = 240000 / (26 * speed);
//    std::cout << pulsetime << "//";
    auto *code = new uint8_t[4];
    code[0] = (uint8_t) (pulsetime / 65536);
    pulsetime -= (pulsetime / 65536) * 65536;
    code[1] = pulsetime / 256;
    pulsetime -= (pulsetime / 256) * 256;
    code[3] = pulsetime;
    code[4] = 0;
    return *reinterpret_cast<uint32_t *>(code);
}

//4er-Array mit Pointern auf die 4-wheelspeed Arrays
Sensor *Sensor::generateWheelspeeds(Sensor main, Sensor curve, Sensor back) {
    assert(main.getCount() == curve.getCount() && curve.getCount() == back.getCount());
    int count = main.getCount();
    int *fl = new int[count];
    int *fr = new int[count];
    int *bl = new int[count];
    int *br = new int[count];
    int *m = main.getData();
    int *c = curve.getData();
    int *b = back.getData();

    for (int i = 0; i < count; i++) {
        fl[i] = encodePulseTime(m[i]);
        fr[i] = encodePulseTime(m[i] + c[i]);
        br[i] = encodePulseTime(m[i] + c[i] + b[i]);
        bl[i] = encodePulseTime(m[i] + b[i]);
    }

    auto *result = new Sensor[4];
    result[0] = Sensor(count, fl);
    result[1] = Sensor(count, fr);
    result[2] = Sensor(count, bl);
    result[3] = Sensor(count, br);
    return result;
}

//SpecialCase für xAccel
void Sensor::modifyForXAccel() {
    for (int i = 0; i < amount; i++)
        data[i] = cos(data[i]) * 256 + 2048;
}

//SpecialCase für yAccel
void Sensor::modifyForYAccel() {
    for (int i = 0; i < amount; i++)
        data[i] = sin(data[i]) * 256 + 2048;
}

//SpecialCase für dem neg Temp Offset
void Sensor::createTempOffset() {
    for (int i = 0; i < amount; i++)
        if (data[i] < 0)
            data[i] += 1024;
}
