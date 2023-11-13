cmake_minimum_required(VERSION 3.22.1)

project("luna-engine")

set(${CMAKE_C_FLAGS}, "${CMAKE_C_FLAGS} -std=c89 -Wall -Werror -Wextra -Wpedantic -Wno-unused-variable -Wno-unused-parameter")
set(CMAKE_SHARED_LINKER_FLAGS
        "${CMAKE_SHARED_LINKER_FLAGS} -u ANativeActivity_onCreate")

add_subdirectory(engine)

aux_source_directory(src LunaLauncherSources)

add_library(luna SHARED
        ${LunaLauncherSources})

target_include_directories(luna PRIVATE
        ${CMAKE_ANDROID_NDK}/sources/android/native_app_glue)

target_include_directories(luna PRIVATE
        engine)

target_link_libraries(luna
        LunaCore)
