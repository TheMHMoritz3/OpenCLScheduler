//
// Created by moritz on 17.06.19.
//

#ifndef CAN_CANMANAGER_H
#define CAN_CANMANAGER_H

#include <vector>

#include "CanNamespace.h"

namespace CAN {
    class CanManager {
    public:
        CanManager();
        void create(CanID id, int count);
        void create(std::vector<CanID> ids, int count);
        void getSamplingRate(CanID id);
    };
}

#endif //CAN_CANMANAGER_H
