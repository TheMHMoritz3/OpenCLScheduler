/*
Embedded Systems Project 2019
Copyright (C) 2019  Moritz Herzog, Philip Lersch

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef CAN_CANMANAGER_H
#define CAN_CANMANAGER_H

#include <vector>
#include <map>
#include <sys/types.h>

#include "CanNamespace.h"
#include "CanAccessor.h"
#include "SimCar.h"
#include "can_global.h"

namespace CAN {
    class CAN_EXPORT CanManager {
    public:
        CanManager();
        void create(CanID id, int count);
        void create(std::vector<CanID> ids, int count);
        static double getSamplingRate(CanID id);
        std::vector<uint32_t*> getData(CanID id);
        //int* getValuesFromSimulation(CAN::CanID id, int count);

    private:
        std::map<int,CanAccessor*> CanThreadMap;
        ValueGen::SimCar* SimCar;
        bool SimulationTrigger;
        int Count;
    };
}

#endif //CAN_CANMANAGER_H
