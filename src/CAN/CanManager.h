//
// Created by moritz on 17.06.19.
//

#ifndef CAN_CANMANAGER_H
#define CAN_CANMANAGER_H

#include <vector>
#include <map>
#include <sys/types.h>

#include "CanNamespace.h"
#include "CanAccessor.h"

namespace CAN {
    class CanManager {
    public:
        CanManager();
        void create(CanID id, int count);
        void create(std::vector<CanID> ids, int count);
        static double getSamplingRate(CanID id);
        std::vector<u_int32_t> getData(CanID id);

    private:
        std::map<int,CanAccessor*> CanThreadMap;
    };
}

#endif //CAN_CANMANAGER_H