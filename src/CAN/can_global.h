//
// Created by root on 28.06.19.
//

#ifndef EMBEDDEDSYSTEMSPROJECT_CAN_GLOBAL_H
#define EMBEDDEDSYSTEMSPROJECT_CAN_GLOBAL_H



#ifdef _WIN32
#ifdef CAN_EXPORTS
#define CAN_EXPORT __declspec(dllexport)
#else
#define CAN_EXPORT __declspec(dllimport)
#endif
#else
#define CAN_EXPORT
#endif
#endif //EMBEDDEDSYSTEMSPROJECT_CAN_GLOBAL_H
