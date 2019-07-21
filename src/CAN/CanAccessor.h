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
