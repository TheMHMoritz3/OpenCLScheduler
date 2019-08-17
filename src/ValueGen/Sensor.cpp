//
// Created by Over on 01.08.2019.
//

#include "Sensor.h"
//#include <stdlib.h>
#include <random>
#include <cassert>

namespace ValueGen {
    Sensor::Sensor(int count, int minL, int maxL, int minA, int maxA, int smooth, int stepS) {
        amount = count;
        minLimit = minL;
        maxLimit = maxL;
        minAccel = minA;
        maxAccel = maxA;
        smoothness = smooth;
        stepSize = stepS;
        data = new int[count+smoothness*2];
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
        std::uniform_int_distribution<int>  range(min, max);
        return range(generator);
    }
    void Sensor::regenValues() {
        int oldX = 0;
        int newX = nextRandi(minLimit, maxLimit);
        int transLength = 0;
        int oldY = 0;

        for(int i=0; i<amount+smoothness*2; i++){
            if(oldY+transLength == i){
                oldX = newX;
                int spread = abs(maxLimit-minLimit)/stepSize;
                if(spread == 0) spread = 2;
                int offset;
                if(newX < (spread/2)+minLimit)
                    offset = minLimit;
                else if(newX > maxLimit-(spread/2))
                    offset = maxLimit-(spread);
                else
                    offset = newX-spread/2;
                newX = nextRandi(offset, spread);
                oldY = i;
                //Aus der möglichen Distanz einen zufälligen Wert suchen
                if(oldX == newX){
                    transLength = smoothness;
                }else if(newX > oldX){
                    int minLength = abs((newX-oldX)/maxAccel);
                    if(minLength==0) minLength = 1;
                    transLength = nextRandi(minLength, minLength*3);
                }else{
                    int minLength = abs((newX-oldX)/minAccel);
                    if(minLength==0) minLength = 1;
                    //System.out.println("["+minLength+"]");
                    transLength = nextRandi(minLength, minLength*3);
                }
            }
            data[i] = (int)(oldX*(1.0-((double)(i-oldY)/(double)transLength)) + newX*((double)(i-oldY)/(double)transLength));
        }
        for(int i=0; i<amount; i++){
            int sum = 0;
            for(int j=0; j<smoothness*2; j++)
                sum += data[i+j];
            data[i] = sum/(smoothness*2);
        }
    }

    //4er-Array mit Pointern auf die 4-wheelspeed Arrays
    Sensor* Sensor::generateWheelspeeds(Sensor main, Sensor curve, Sensor back) {
        assert(main.getCount()==curve.getCount() && curve.getCount()==back.getCount());
        int count = main.getCount();
        int* fl = new int[count];
        int* fr = new int[count];
        int* bl = new int[count];
        int* br = new int[count];

        for(int i=0; i<count; i++){
            fl[i] = main[i];
            fr[i] = main[i] + curve[i];
            br[i] = main[i] + curve[i] + back[i];
            bl[i] = main[i] + back[i];
        }
        auto *result = new Sensor[4];
        result[0] = Sensor(count, fl);
        result[1] = Sensor(count, fr);
        result[2] = Sensor(count, bl);
        result[3] = Sensor(count, br);
        return result;
    }

    int *Sensor::getData() {
        return data;
    }

    int Sensor::getCount() {
        return amount;
    }
}
