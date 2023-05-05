cmake_minimum_required(VERSION 3.25)
project(ini C)

set(CMAKE_C_STANDARD 23)

if (MSVC)
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
endif()

add_library(ini STATIC ini.c ini.h)
set_target_properties(ini PROPERTIES PREFIX "")
    