//
// Created by moritz on 17.06.19.
//

#include "CanAccessor.h"

using namespace CAN;

CanAccessor::CanAccessor(CanID id, int elementCount) {

}

CanErrorCode CanAccessor::didErrorOccur() {
    return NoError;
}

std::vector<u_int32_t> CanAccessor::getData() {
    return std::vector<u_int32_t>();
}

void CanAccessor::init() {

}
