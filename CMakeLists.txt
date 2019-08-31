cmake_minimum_required (VERSION 3.10)

set (CMAKE_CXX_COMPILER /usr/bin/g++)

project(CAN DESCRIPTION "C3CAN Interface Libary")

set(CMAKE_CXX_STANDARD 11)
set (CAN_VERSION_MAJOR 1)
set (CAN_VERSION_MINOR 0)

include_directories("${PROJECT_BINARY_DIR}")

add_library(CAN SHARED src/CAN/RandomNumberGenerator.h
			src/CAN/RandomNumberGenerator.cpp
			src/CAN/CanAccessor.cpp 
			src/CAN/CanAccessor.h
			src/CAN/CanNamespace.h 
			src/CAN/CanManager.cpp
			src/CAN/CanManager.h 
			src/CAN/can_global.h)
	
if(UNIX)
target_link_libraries(CAN pthread c3can_core c3can_error)
endif(UNIX)
if(WIN32)

endif(WIN32)
target_compile_options(CAN PUBLIC)



project(Scheduler DESCRIPTION "Scheduler Interface Libary")

set(CMAKE_CXX_STANDARD 11)
set (Scheduler_VERSION_MAJOR 1)
set (Scheduler_VERSION_MINOR 0)

include_directories("${PROJECT_BINARY_DIR}")

add_library(Scheduler SHARED src/Scheduler/KernelFileParser.cpp
			src/Scheduler/KernelFileParser.h
			src/Scheduler/Device.cpp
			src/Scheduler/Device.h
			src/Scheduler/ScheduleManager.cpp
			src/Scheduler/ScheduleManager.h
			src/Scheduler/Scheduler.cpp
			src/Scheduler/Scheduler.h
			src/Scheduler/Task.cpp
			src/Scheduler/Task.h
			src/Scheduler/StaticScheduler.cpp
			src/Scheduler/StaticScheduler.h
			src/Scheduler/ASAP.cpp
			src/Scheduler/ASAP.h
			src/Scheduler/SchedulerNamespace.h
			src/Scheduler/scheduler_global.h
			src/Scheduler/DevicePropertys.cpp
			src/Scheduler/DevicePropertys.h
			src/Scheduler/ReadyFireScheduler.cpp
			src/Scheduler/ReadyFireScheduler.h)
			
			
if(UNIX)
target_link_libraries(Scheduler libOpenCL.so)
target_include_directories (Scheduler PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
endif(UNIX)
if(WIN32)
target_link_libraries(Scheduler $ENV{OCL_ROOT}/lib/x86_64/opencl.lib)
target_include_directories (Scheduler PUBLIC $ENV{OCL_ROOT}/include ${CMAKE_CURRENT_SOURCE_DIR})
endif(WIN32)

target_compile_options(Scheduler PUBLIC)



project(UI DESCRIPTION "Scheduler Interface Libary")

set(CMAKE_CXX_STANDARD 11)
set (UI_VERSION_MAJOR 1)
set (UI_VERSION_MINOR 0)
set (CMAKE_PREFIX_PATH  $ENV{QTDIR})
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt5 COMPONENTS REQUIRED Core Gui Widgets)

MESSAGE("Searching for QWT")
IF(WIN32)
SET(QWT_INCLUDE_DIR ../qwt/src )

set(QWT_LIBRARY ../qwt/lib/qwt )

ELSE(WIN32)
FIND_PATH(QWT_INCLUDE_DIR NAMES qwt.h PATHS
        /usr/include
        /usr/local/include
        "$ENV{LIB_DIR}/include"
        "$ENV{INCLUDE}"
        PATH_SUFFIXES qwt-qt4 qwt
        )

FIND_LIBRARY(QWT_LIBRARY NAMES qwt qwt-qt4 qwt5-qt4 qwt-qt5 PATHS
        /usr/lib
        /usr/local/lib
        "$ENV{LIB_DIR}/lib"
        "$ENV{LIB}/lib"
        )

IF (QWT_INCLUDE_DIR AND QWT_LIBRARY)
    SET(QWT_FOUND TRUE)
ENDIF (QWT_INCLUDE_DIR AND QWT_LIBRARY)

IF (QWT_FOUND)
    IF (NOT QWT_FIND_QUIETLY)
        MESSAGE(STATUS "Found QWT: ${QWT_LIBRARY}")
    ENDIF (NOT QWT_FIND_QUIETLY)
ELSE (QWT_FOUND)
    IF (QWT_FIND_REQUIRED)
        MESSAGE(FATAL_ERROR "Could not find QWT")
    ENDIF (QWT_FIND_REQUIRED)
ENDIF (QWT_FOUND)
ENDIF(WIN32)

get_target_property(QtCore_location Qt5::Core LOCATION)

include_directories(${QWT_INCLUDE_DIR} ${PROJECT_BINARY_DIR} )

qt5_wrap_ui(UI_UICHEADERS src/UI/MainWindow.ui
			src/UI/ConstantDialog.ui
			src/UI/TaskTabWidget.ui
			src/UI/OpenTaskWizard.ui)

add_executable(UI ${UI_UICHEADERS} src/UI/main.cpp 
			src/UI/ConstantDialog.cpp 
			src/UI/ConstantDialog.h
			src/UI/OpenKernelFileWizard.cpp
			src/UI/OpenKernelFileWizard.h
			src/UI/TUI.cpp src/UI/TUI.h
			src/UI/MainWindow.h
			src/UI/MainWindow.cpp
			src/UI/TaskTabWidget.h
			src/UI/TaskTabWidget.cpp)

file(GLOB_RECURSE SRC_UI "src/UI/*.ui")

source_group("Form Files" FILES ${SRC_UI})

add_dependencies(UI Scheduler CAN)
target_link_libraries(UI LINK_PUBLIC Scheduler CAN Qt5::Widgets ${QWT_LIBRARY})

target_compile_options(UI PUBLIC)
