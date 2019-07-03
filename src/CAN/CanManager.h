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
#include "can_global.h"

namespace CAN {
    class CAN_EXPORT CanManager {
    public:
        CanManager();
        void create(CanID id, int count);
        void create(std::vector<CanID> ids, int count);
        static double getSamplingRate(CanID id);
        std::vector<uint32_t*> getData(CanID id);

    private:
        std::map<int,CanAccessor*> CanThreadMap;
    };
}

#endif //CAN_CANMANAGER_H
