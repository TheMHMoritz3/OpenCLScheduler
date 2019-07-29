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

#ifndef EMBEDDEDSYSTEMSPROJECT_CAN_GLOBAL_H
#define EMBEDDEDSYSTEMSPROJECT_CAN_GLOBAL_H



#ifdef _WINDOWS
#ifdef CAN_EXPORTS
#define CAN_EXPORT __declspec(dllexport)
#else
#define CAN_EXPORT __declspec(dllimport)
#endif
#else
#define CAN_EXPORT
#endif
#endif //EMBEDDEDSYSTEMSPROJECT_CAN_GLOBAL_H
