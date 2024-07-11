cmake_minimum_required(VERSION 3.18)
project(ZumaDecomp)

if(CMAKE_SIZEOF_VOID_P EQUAL 8)
    set(BITNESS "64")
elseif(CMAKE_SIZEOF_VOID_P EQUAL 4)
    set(BITNESS "32")
endif()

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake-modules" )
add_definitions(-DCMAKE_EXPORT_COMPILE_COMMANDS=ON)
set(CMAKE_CXX_STANDARD 98)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(MSVC)
    message(STATUS "MSVC Version: ${MSVC_VERSION}")
    if (MSVC_VERSION EQUAL 1300)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /GF /EHsc /GS /Gy /GR /W3 /c /Wp64 /Z7 /TP")
        set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /Og /Ob1 /Oi /Ot /Oy-")
        set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded")
    else()
        add_definitions(-DNOMINMAX)

        if(CMAKE_CXX_FLAGS MATCHES "sanitize=address")
            message(STATUS "ASAN detected, switching to non-debug runtime library.")
            set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded")
        endif()
    endif()

    add_definitions(-D_USE_MATH_DEFINES)
endif()

option(USE_BASS2 "Use BASS 2.x instead of BASS 1.x" OFF)

# Use BASS 2.0 on 64-bit platform
if(NOT WIN32 OR BITNESS STREQUAL "64")
    message(STATUS "64-bit platform detected, forcing BASS 2.x")
    set(USE_BASS2 ON)
endif()

if (USE_BASS2)
    add_definitions(-DBASS2)
endif()

# add_subdirectory(sexy-windows)
add_subdirectory(source/ImageLib)
add_subdirectory(source/PakLib)
add_subdirectory(source/SexyAppFramework)
set(SEXYAPP_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/source)
add_subdirectory(source/CircleShoot)