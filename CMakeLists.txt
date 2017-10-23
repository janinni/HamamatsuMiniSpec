cmake_minimum_required(VERSION 2.6)

project(SpectrometerClass)
set(PROJECT_VERSION 0.0.0.0)
set(PROJECT_BRIEF "\"No description\"")


set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")
set(LIBRARY_OUTPUT_PATH "${CMAKE_BINARY_DIR}/lib")
set(EXECUTABLE_OUTPUT_PATH "${CMAKE_BINARY_DIR}/bin")

# ROOT, a library for HEP data analysis (contains RooFit)
#find_package(ROOT REQUIRED)
#include_directories(${ROOT_INCLUDE_DIR})
#set(LIBRARIES ${LIBRARIES} ${ROOT_LIBRARIES})
#set(LIBRARIES ${LIBRARIES} ${ROOFIT_LIBRARIES})

set(Boost_USE_STATIC_LIBS OFF) 
set(Boost_USE_MULTITHREADED ON)  
set(Boost_USE_STATIC_RUNTIME OFF) 
find_package(Boost 1.54.0 COMPONENTS system filesystem regex REQUIRED) 

if(Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS}) 
endif()

if(${APPLE})

  MESSAGE( STATUS "Building for Mac OS X, switching on C++11 flags for Mac OS X/clang" )

  SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pedantic -Wall -Wextra")

endif(${APPLE})

IF(${CMAKE_SYSTEM_NAME} MATCHES "Linux")

  MESSAGE( STATUS "Building for Linux, switching on C++11 flags for Linux/gcc" )

  SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x -pedantic -Wall -Wextra")

ENDIF(${CMAKE_SYSTEM_NAME} MATCHES "Linux")

add_executable(SpectrometerMain SpectrometerMain.cpp Spectrometer.cpp SpecMeasurement/SpecMeasurement.cpp /home/xytable/src/LEDClass/LED.cpp /home/xytable/src/RS232com/RS232communication.cpp)
target_link_libraries( SpectrometerMain ${Boost_LIBRARIES})
#target_link_libraries( xyTableMain ${LIBRARIES})

