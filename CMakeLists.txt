cmake_minimum_required (VERSION 2.6)

project(CAN DESCRIPTION "C3CAN Interface Libary")
# The version number.
set (CAN_VERSION_MAJOR 1)
set (CAN_VERSION_MINOR 0)
#set (CMAKE_CXX_COMPILER g++)

include_directories("${PROJECT_BINARY_DIR}")

# add the executable
add_library(CAN SHARED src/CAN/CanAccessor.cpp src/CAN/CanAccessor.h src/CAN/CanNamespace.h src/CAN/CanManager.cpp src/CAN/CanManager.h)
target_link_libraries(CAN libsupc++.a c3can_core c3can_error)

target_compile_options(CAN PUBLIC -Wall)

#target_link_options(deviceinfo PUBLIC -lOpenCL PUBLIC -lsupc++)

project(Scheduler DESCRIPTION "Scheduler Interface Libary")
# The version number.
set (Scheduler_VERSION_MAJOR 1)
set (Scheduler_VERSION_MINOR 0)
#set (CMAKE_CXX_COMPILER g++)

include_directories("${PROJECT_BINARY_DIR}")

# add the executable
add_library(Scheduler SHARED src/Scheduler/Device.cpp src/Scheduler/Device.h src/Scheduler/ScheduleManager.cpp src/Scheduler/ScheduleManager.h src/Scheduler/Scheduler.cpp src/Scheduler/Scheduler.h src/Scheduler/Task.cpp src/Scheduler/Task.h src/Scheduler/StaticScheduler.cpp src/Scheduler/StaticScheduler.h src/Scheduler/SchedulerNamespace.h)
target_link_libraries(Scheduler libOpenCL.so libsupc++.a)

target_compile_options(Scheduler PUBLIC -Wall)

#target_link_options(deviceinfo PUBLIC -lOpenCL PUBLIC -lsupc++)


project(UI DESCRIPTION "Scheduler Interface Libary")
# The version number.
set (UI_VERSION_MAJOR 1)
set (UI_VERSION_MINOR 0)
#set (CMAKE_CXX_COMPILER g++)

include_directories("${PROJECT_BINARY_DIR}")

# add the executable
add_executable(UI src/UI/main.cpp)
add_dependencies(UI Scheduler CAN)
target_link_libraries(UI libsupc++.a)

target_compile_options(UI PUBLIC -Wall)

#target_link_options(deviceinfo PUBLIC -lOpenCL PUBLIC -lsupc++)