cmake_minimum_required(VERSION 3.25)

project(orbis VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(CTest)

if(BUILD_TESTING)
    find_package(Catch2 3 REQUIRED)
endif()

add_subdirectory(engine)