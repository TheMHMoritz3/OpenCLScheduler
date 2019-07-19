cmake_minimum_required (VERSION 3.10)

#set (CMAKE_CXX_COMPILER /usr/bin/g++)

project(CAN DESCRIPTION "C3CAN Interface Libary")
# The version number.
set(CMAKE_CXX_STANDARD 11)
set (CAN_VERSION_MAJOR 1)
set (CAN_VERSION_MINOR 0)

include_directories("${PROJECT_BINARY_DIR}")

# add the executable
add_library(CAN SHARED src/CAN/CanAccessor.cpp src/CAN/CanAccessor.h src/CAN/CanNamespace.h src/CAN/CanManager.cpp src/CAN/CanManager.h src/CAN/can_global.h)
if(UNIX)
target_link_libraries(CAN pthread c3can_core c3can_error)
endif(UNIX)
if(WIN32)
#target_link_libraries(CAN )
endif(WIN32)
target_compile_options(CAN PUBLIC)


project(Scheduler DESCRIPTION "Scheduler Interface Libary")
# The version number.
set(CMAKE_CXX_STANDARD 11)
set (Scheduler_VERSION_MAJOR 1)
set (Scheduler_VERSION_MINOR 0)


include_directories("${PROJECT_BINARY_DIR}")


# add the executable
add_library(Scheduler SHARED src/Scheduler/Device.cpp src/Scheduler/Device.h src/Scheduler/ScheduleManager.cpp src/Scheduler/ScheduleManager.h src/Scheduler/Scheduler.cpp src/Scheduler/Scheduler.h src/Scheduler/Task.cpp src/Scheduler/Task.h src/Scheduler/StaticScheduler.cpp src/Scheduler/StaticScheduler.h src/Scheduler/SchedulerNamespace.h src/Scheduler/scheduler_global.h)
if(UNIX)
target_link_libraries(Scheduler libOpenCL.so)
target_include_directories (Scheduler PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
endif(UNIX)
if(WIN32)
target_link_libraries(Scheduler $ENV{OCL_ROOT}/lib/x86_64/opencl.lib)
target_include_directories (Scheduler PUBLIC $ENV{OCL_ROOT}/include ${CMAKE_CURRENT_SOURCE_DIR})
endif(WIN32)

target_compile_options(Scheduler PUBLIC)

#target_link_options(deviceinfo PUBLIC -lOpenCL PUBLIC -lsupc++)

project(UI DESCRIPTION "Scheduler Interface Libary")
# The version number.
set(CMAKE_CXX_STANDARD 11)
set (UI_VERSION_MAJOR 1)
set (UI_VERSION_MINOR 0)
set (CMAKE_PREFIX_PATH  $ENV{QTDIR})
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt5 COMPONENTS REQUIRED Core Gui Widgets)
get_target_property(QtCore_location Qt5::Core LOCATION)

include_directories("${PROJECT_BINARY_DIR}")

qt5_wrap_ui(UI_UICHEADERS src/UI/MainWindow.ui)

# add the executable
add_executable(UI ${UI_UICHEADERS} src/UI/main.cpp src/UI/TUI.cpp src/UI/TUI.h src/UI/MainWindow.h src/UI/MainWindow.cpp src/UI/RandomNumberGenerator.h src/UI/RandomNumberGenerator.cpp)

add_dependencies(UI Scheduler CAN)
target_link_libraries(UI LINK_PUBLIC Scheduler CAN Qt5::Widgets)

target_compile_options(UI PUBLIC)

#target_link_options(deviceinfo PUBLIC -lOpenCL PUBLIC -lsupc++)
