//
// Created by moritz on 17.06.19.
//

#ifndef CAN_CANNAMESPACE_H
#define CAN_CANNAMESPACE_H

namespace CAN{
    enum CanID{
        WheelRearRight,
        WheelRearFront,
        WheelFrontLeft,
        WheelFrontRight,
        BatteryVoltage,
        AccelerationX,
        AccelerationY
    };

    enum CanErrorCode{
        NoError = 0

    };
}

#endif //CAN_CANNAMESPACE_H
