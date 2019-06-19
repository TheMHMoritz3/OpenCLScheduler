//
// Created by moritz on 17.06.19.
//

#ifndef DEVICEINFO_CANACCESSOR_H
#define DEVICEINFO_CANACCESSOR_H

#include <vector>
#include <sys/param.h>
#include <c3can/error/extended.h>
#include <c3can/core.h>
#include "CanNamespace.h"

namespace CAN {
    class CanAccessor {
    public:
        CanAccessor()= delete;
        CanAccessor(CanID id, int elementCount);
        CanErrorCode didErrorOccur();
        std::vector<u_int32_t> getData();
        void startCollectingData();
    private:
        void init();
        int ElementCount;
        u_int32_t* Data;
        c3can_single* Single;
        c3can_message Message;
        CanID IdCan;
    };
}


#endif //DEVICEINFO_CANACCESSOR_H
