cmake_minimum_required(VERSION 3.14)
project(backtester LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

file(GLOB_RECURSE BACKTESTER_SOURCES CONFIGURE_DEPENDS "src/*.cpp")

add_executable(backtester ${BACKTESTER_SOURCES})

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

target_include_directories(backtester PRIVATE include)

