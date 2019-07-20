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

#include "CanAccessor.h"
#include <iostream>
#include <exception>

#define U32_DATA(p) (*(uint32_t*)(p))

using namespace CAN;

CanAccessor::CanAccessor(CanID id, int elementCount) {
    IdCan = id;
    ElementCount = elementCount;
    init();
    Data = new uint32_t[ElementCount];
}

int CanAccessor::didErrorOccur() {
    return ErrorCode;
}

std::vector<uint32_t *> CanAccessor::getData() {
//    Thread.join();
    std::vector<uint32_t *> *data = new std::vector<uint32_t *>();
#ifndef _WIN32
    for (int i = 0; i < ElementCount; i++) {
        data->push_back(&Data[i]);
    }
#endif
    return *data;
}

void CanAccessor::init() {
#ifndef _WIN32
    Single = c3can_single_init("can0");
    ErrorCode = c3can_single_get_error(Single)->code;
    c3can_single_filter_add(Single, IdCan, (C3CAN_SINGLE_FILTER_OPTS) 0);
    ErrorCode = c3can_single_get_error(Single)->code;
#endif
}

void CanAccessor::collectData() {
#ifndef _WIN32
    struct timeval timestamp;
#endif
    for (int i = 0; i < ElementCount; i++) {
#ifndef _WIN32
        c3can_message *canMessage = new c3can_message();
        c3can_single_recv(Single, canMessage, &timestamp);
        ErrorCode = c3can_single_get_error(Single)->code;
        Data[i] = U32_DATA(c3can_message_get_payload(canMessage));
#endif
    }
}

void CanAccessor::startCollectingData() {

    //Thread=std::thread([=] {collectData();});
    collectData();
}
