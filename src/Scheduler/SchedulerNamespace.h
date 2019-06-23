//
// Created by moritz on 17.06.19.
//

#ifndef UI_SCHEDULERNAMESPACE_H
#define UI_SCHEDULERNAMESPACE_H
namespace SCHEDULER {
#ifdef _WINDOWS
	enum __declspec(dllexport) ScheduleType {
#endif
#ifndef _WINDOWS
	enum ScheduleType {
#endif // !_WINDOWS
        LIST,
        ASAPHC,
        STATIC
    };

#ifdef _WINDOWS
	enum __declspec(dllexport) ErrorCodes {
#endif
#ifndef _WINDOWS
		enum ErrorCodes {
#endif // !_WINDOWS
        COMPILATION_NOT_POSSIBLE
    };

#ifdef _WINDOWS
	enum __declspec(dllexport) Type {
#endif
#ifndef _WINDOWS
		enum Type {
#endif // !_WINDOWS
        INT,
        UINT,
        CHAR,
        FLOAT,
        DOUBLE,
        STRING
    };
}

#endif //UI_SCHEDULERNAMESPACE_H
