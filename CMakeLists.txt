cmake_minimum_required(VERSION 3.18)
project(ZumaDecomp)

if(CMAKE_SIZEOF_VOID_P EQUAL 8)
    set(BITNESS "64")
elseif(CMAKE_SIZEOF_VOID_P EQUAL 4)
    set(BITNESS "32")
endif()

LIST(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake-modules" )
add_definitions(-DCMAKE_EXPORT_COMPILE_COMMANDS=ON)
set(CMAKE_CXX_STANDARD 98)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

IF(MSVC)
    add_definitions(-D_USE_MATH_DEFINES)
    add_definitions(-DNOMINMAX)

    IF (CMAKE_CXX_FLAGS MATCHES "sanitize=address")
        message(STATUS "ASAN detected, switching to non-debug runtime library.")
        set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded")
    ENDIF()
ENDIF()

# Use BASS 2.0 on 64-bit platform
IF(NOT WIN32 OR BITNESS STREQUAL "64")
    message(STATUS "Using BASS 2.x")
    add_definitions(-DBASS2)
ENDIF()

FIND_PACKAGE(DirectX REQUIRED)

set(SEXYAPP_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/source)

add_subdirectory(source/ImageLib)
add_subdirectory(source/PakLib)
add_subdirectory(source/SexyAppFramework)
add_subdirectory(source/CircleShoot)
