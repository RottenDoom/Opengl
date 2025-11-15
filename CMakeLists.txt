cmake_minimum_required(VERSION 3.17)

set(CMAKE_C_STANDARD 99)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Choose the type of build." FORCE)
endif()

list(APPEND CMAKE_MODULE_PATH
    ${PROJECT_SOURCE_DIR}/cmake
    ${PROJECT_SOURCE_DIR}/cmake/cmake-compiler
)

include(cmake/cmake-compiler/gcc.cmake)
include(cmake/macros.cmake)

project(opengl VERSION 1.0 LANGUAGES C CXX)

include(cmake/options.cmake)

include(cmake/dependencies.cmake)

add_subdirectory(src)