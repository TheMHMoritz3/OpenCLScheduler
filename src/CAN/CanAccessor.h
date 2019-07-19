//
// Created by moritz on 17.06.19.
//

#ifndef DEVICEINFO_CANACCESSOR_H
#define DEVICEINFO_CANACCESSOR_H

#include <vector>
#ifndef _WIN32
#include <sys/param.h>
#include <c3can/error/extended.h>
#include <c3can/core.h>
#endif
#include <thread>

#include "CanNamespace.h"

namespace CAN {
    class CanAccessor {
    public:
        CanAccessor()= delete;
        CanAccessor(CanID id, int elementCount);
        int didErrorOccur();
        std::vector<uint32_t*> getData();
        void startCollectingData();
    private:
        void collectData();
        void init();
        int ElementCount;
        uint32_t* Data;
#ifndef _WIN32
        c3can_single* Single;
        c3can_message Message;
#endif
        CanID IdCan;
        std::thread Thread;
        int ErrorCode;
    };
}


#endif //DEVICEINFO_CANACCESSOR_H
