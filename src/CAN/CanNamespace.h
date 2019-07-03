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

#ifndef CAN_CANNAMESPACE_H
#define CAN_CANNAMESPACE_H

#include "can_global.h"

namespace CAN{
    enum CAN_EXPORT CanID{
        WheelFrontRight = 0x008,
        WheelFrontLeft = 0x009,
        WheelRearLeft = 0x00a,
        WheelRearRight = 0x00b,
        BatteryVoltage = 0x00d,
        AccelerationLongitudinal = 0x190,
        AccelerationLateral = 0x191,
        Temperature = 0x192
    };

    enum CAN_EXPORT CanErrorCode{
        NoError = 0,
        KernelError,
        ArgError,
        AccessError,
        AddressError,
        NetworkError,
        MemoryError,
        BufferError,
        ReadError = 10,
        SendError,
        Connection = 16,
        UnkownError = 19,
        TimeOut
    };
}

#endif //CAN_CANNAMESPACE_H
