cmake_minimum_required(VERSION 3.2)

project(LEDGUI)
set(PROJECT_VERSION 0.0.0.0)
set(PROJECT_BRIEF "\"No description\"")

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")
set(LIBRARY_OUTPUT_PATH "${CMAKE_BINARY_DIR}/lib")
set(EXECUTABLE_OUTPUT_PATH "${CMAKE_BINARY_DIR}/bin")


# Find the QtWidgets library
find_package(Qt5Widgets)

if(Qt5Widgets_FOUND)
	include_directories(${Qt5Widgets_INCLUDE_DIRS})
	MESSAGE(${Qt5Widgets_INCLUDE_DIRS})
endif()

# Needed for QT
set(CMAKE_AUTOMOC ON)

set(Boost_USE_STATIC_LIBS OFF) 
set(Boost_USE_MULTITHREADED ON)  
set(Boost_USE_STATIC_RUNTIME OFF) 
find_package(Boost 1.54.0 COMPONENTS system filesystem regex REQUIRED) 

if(Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS}) 
endif()

# Find the QtWidgets library
find_package(Qt5Widgets)

if(${APPLE})

  MESSAGE( STATUS "Building for Mac OS X, switching on C++11 flags for Mac OS X/clang" )

  SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pedantic -Wall -Wextra -fPIC")

endif(${APPLE})

IF(${CMAKE_SYSTEM_NAME} MATCHES "Linux")

  MESSAGE( STATUS "Building for Linux, switching on C++11 flags for Linux/gcc" )

  SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x -pedantic -Wall -Wextra -fPIC")

ENDIF(${CMAKE_SYSTEM_NAME} MATCHES "Linux")

add_library(LED ../../LEDClass/LED.cpp ../../RS232com/RS232communication.cpp)
add_library(Spectrometer ../Spectrometer.cpp)
add_library(SpecMeas ../SpecMeasurement/SpecMeasurement.cpp)
add_library(SpectrometerApp SpectrometerApp.cpp)

add_executable(SpectrometerGUI SpectrometerAppMain.cpp)
target_link_libraries(SpectrometerGUI SpectrometerApp LED Spectrometer SpecMeas ${Qt5Widgets_LIBRARIES} ${Boost_LIBRARIES})

