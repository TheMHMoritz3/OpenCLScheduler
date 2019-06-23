//
// Created by moritz on 17.06.19.
//

#ifndef UI_SCHEDULERNAMESPACE_H
#define UI_SCHEDULERNAMESPACE_H
namespace SCHEDULER {
    enum ScheduleType {
        LIST,
        ASAPHC,
        STATIC
    };

    enum ErrorCodes {
        COMPILATION_NOT_POSSIBLE
    };

    enum Type {
        INT,
        UINT,
        CHAR,
        FLOAT,
        DOUBLE,
        STRING
    };
}

#endif //UI_SCHEDULERNAMESPACE_H