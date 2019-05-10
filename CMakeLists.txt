cmake_minimum_required (VERSION 2.6)
project (EmbeddedSystemsProject)
# The version number.
set (EmbeddedSystemsProject_VERSION_MAJOR 1)
set (EmbeddedSystemsProject_VERSION_MINOR 0)
 
# configure a header file to pass some of the CMake settings
# to the source code
#configure_file ("${PROJECT_SOURCE_DIR}/src")
 
# add the binary tree to the search path for include files
# so that we will find TutorialConfig.h
include_directories("${PROJECT_BINARY_DIR}")
 
# add the executable
add_executable(EmbeddedSystemsProject src/battery_voltage.cpp src/InstanceManager.cpp src/InstanceManager.h src/main.cpp)

#target_compile_options(EmbeddedSystemsProject PUBLIC -Wall)

target_link_options(EmbeddedSystemsProject PUBLIC -lOpenCL PUBLIC -lsupc++ PUBLIC -lc3can_core PUBLIC -lc3can_error)
#SET(GCC_COVERAGE_LINK_FLAGS "-lOpenCL -lsupc++ -lc3can_{core,error}")
#add_definitions(${GCC_COVERAGE_COMPILE_FLAGS})

project (voltage)
# The version number.
set (voltage_VERSION_MAJOR 1)
set (voltage_VERSION_MINOR 0)

# configure a header file to pass some of the CMake settings
# to the source code
#configure_file ("${PROJECT_SOURCE_DIR}/src")

# add the binary tree to the search path for include files
# so that we will find TutorialConfig.h
include_directories("${PROJECT_BINARY_DIR}")

# add the executable
add_executable(voltage src/battery_voltage.cpp)

#target_compile_options(EmbeddedSystemsProject PUBLIC -Wall)

target_link_options(voltage PUBLIC -lOpenCL PUBLIC -lsupc++ PUBLIC -lc3can_core PUBLIC -lc3can_error)

project (wheelspin)
# The version number.
set (wheelspin_VERSION_MAJOR 1)
set (wheelspin_VERSION_MINOR 0)

# configure a header file to pass some of the CMake settings
# to the source code
#configure_file ("${PROJECT_SOURCE_DIR}/src")

# add the binary tree to the search path for include files
# so that we will find TutorialConfig.h
include_directories("${PROJECT_BINARY_DIR}")

# add the executable
add_executable(wheelspin src/wheel_sensors.cpp)

#target_compile_options(EmbeddedSystemsProject PUBLIC -Wall)

target_link_options(wheelspin PUBLIC -lOpenCL PUBLIC -lsupc++ PUBLIC -lc3can_core PUBLIC -lc3can_error)

project (accelaration)
# The version number.
set (accelaration_VERSION_MAJOR 1)
set (accelaration_VERSION_MINOR 0)

# configure a header file to pass some of the CMake settings
# to the source code
#configure_file ("${PROJECT_SOURCE_DIR}/src")

# add the binary tree to the search path for include files
# so that we will find TutorialConfig.h
include_directories("${PROJECT_BINARY_DIR}")

# add the executable
add_executable(accelaration src/accel_sensor.cpp)

#target_compile_options(EmbeddedSystemsProject PUBLIC -Wall)

target_link_options(accelaration PUBLIC -lOpenCL PUBLIC -lsupc++ PUBLIC -lc3can_core PUBLIC -lc3can_error)

project (tempreture)
# The version number.
set (tempreture_VERSION_MAJOR 1)
set (tempreture_VERSION_MINOR 0)

# configure a header file to pass some of the CMake settings
# to the source code
#configure_file ("${PROJECT_SOURCE_DIR}/src")

# add the binary tree to the search path for include files
# so that we will find TutorialConfig.h
include_directories("${PROJECT_BINARY_DIR}")

# add the executable
add_executable(tempreture src/tempreture.cpp)

#target_compile_options(EmbeddedSystemsProject PUBLIC -Wall)

target_link_options(tempreture PUBLIC -lOpenCL PUBLIC -lsupc++ PUBLIC -lc3can_core PUBLIC -lc3can_error)