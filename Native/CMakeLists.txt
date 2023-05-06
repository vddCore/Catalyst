cmake_minimum_required(VERSION 3.25)
project(Catalyst C)

set(CMAKE_C_STANDARD 23)

if (MSVC)
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
endif()

add_subdirectory(deps/ini)
include_directories(Catalyst deps)

add_library(Catalyst SHARED 
    src/dllmain.def 
    src/dllmain.c
    src/shims/winhttp.c
    src/shims/winhttp.h
    src/shims/shims.h
    src/shim_context.c
    src/shim_context.h
    src/shim_bridge.c
    src/shim_bridge.h
    src/console_alloc.c
    src/mono_loader/mono_loader.c
    src/mono_loader/mono_loader.h
    src/mono_loader/catalyst_bootstrap.c
    src/mono_loader/catalyst_bootstrap.h
)

target_link_libraries(Catalyst shlwapi ini)

set_target_properties(Catalyst PROPERTIES PREFIX "")
set_target_properties(Catalyst PROPERTIES OUTPUT_NAME "winhttp")

