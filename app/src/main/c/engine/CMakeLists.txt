cmake_minimum_required(VERSION 3.22.1)

project("luna-engine-core")

set(${CMAKE_C_FLAGS}, "${CMAKE_C_FLAGS} -std=c89 -Wall -Werror -Wextra -Wpedantic -Wno-unused-variable -Wno-unused-parameter")
set(CMAKE_SHARED_LINKER_FLAGS
        "${CMAKE_SHARED_LINKER_FLAGS} -u ANativeActivity_onCreate")

set(TARGET android)

add_library(native_app_glue STATIC
        ${CMAKE_ANDROID_NDK}/sources/android/native_app_glue/android_native_app_glue.c)

add_library(LunaCore STATIC
        src/luna.c
        src/platform/${TARGET}/gl.c)

target_include_directories(LunaCore PRIVATE
        ${CMAKE_ANDROID_NDK}/sources/android/native_app_glue)
target_include_directories(LunaCore PRIVATE
        src/platform/${TARGET})

target_link_libraries(LunaCore
        android
        native_app_glue
        EGL
        GLESv1_CM
        log)
