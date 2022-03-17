cmake_minimum_required(VERSION 3.18)
project(ZumaDecomp)

LIST(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake-modules" )
add_definitions(-DCMAKE_EXPORT_COMPILE_COMMANDS=ON)

IF(MSVC)
    add_definitions(-D_USE_MATH_DEFINES)
    add_definitions(-DNOMINMAX)
ENDIF()

FIND_PACKAGE(DirectX REQUIRED)

add_subdirectory(source/ImageLib)
add_subdirectory(source/PakLib)
add_subdirectory(source/SexyAppFramework)
add_subdirectory(source/CircleShoot)