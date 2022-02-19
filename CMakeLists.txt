cmake_minimum_required (VERSION 3.21.4)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)


set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_BUILD_TYPE debug)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++20")

project (joystick-service)

find_package(Curses REQUIRED)
include_directories(${CURSES_INCLUDE_DIR})

find_package(yaml-cpp REQUIRED)
include_directories(${YAML_CPP_INCLUDE_DIR})

find_package(Threads REQUIRED)

set(CXX_FLAGS "-fconcepts-ts")

add_executable ( ${CMAKE_PROJECT_NAME}
  joystick.cpp 
  argparse.hpp 
  controller.hpp)

target_link_libraries(${CMAKE_PROJECT_NAME}
  ${CURSES_LIBRARY} 
  ${YAML_CPP_LIBRARIES}
  ${CMAKE_THREAD_LIBS_INIT})

