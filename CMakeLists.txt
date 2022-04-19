cmake_minimum_required (VERSION 3.21.4)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)


set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_BUILD_TYPE debug)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++20")
set(CXX_FLAGS "-fconcepts-ts")

project (joystick-service)

find_package(Curses REQUIRED)
include_directories(${CURSES_INCLUDE_DIR})

find_package(yaml-cpp REQUIRED)
include_directories(${YAML_CPP_INCLUDE_DIR})

find_package(Threads REQUIRED)

include(ExternalProject)
set(EXTERNAL_INSTALL_LOCATION ${CMAKE_BINARY_DIR}/external)

ExternalProject_Add( argparse
    GIT_REPOSITORY https://github.com/p-ranav/argparse.git
    CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${EXTERNAL_INSTALL_LOCATION}
  )

include_directories(${EXTERNAL_INSTALL_LOCATION}/include)
set(INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include)
include_directories(${INCLUDE_DIR})

add_executable ( ${CMAKE_PROJECT_NAME}
  src/joystick.cpp)

add_dependencies(${CMAKE_PROJECT_NAME}
  argparse)

target_link_libraries(${CMAKE_PROJECT_NAME}
  ${YAML_CPP_LIBRARIES}
  zmq)

