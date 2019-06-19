//
// Created by moritz on 17.06.19.
//

#ifndef CAN_CANNAMESPACE_H
#define CAN_CANNAMESPACE_H

namespace CAN{
    enum CanID{
        WheelFrontRight = 0x008,
        WheelFrontLeft = 0x009,
        WheelRearLeft = 0x00a,
        WheelRearRight = 0x00b,
        BatteryVoltage = 0x00d,
        AccelerationLongitudinal = 0x190,
        AccelerationLateral = 0x191,
        Tempreture = 0x192
    };

    enum CanErrorCode{
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
