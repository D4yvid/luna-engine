cmake_minimum_required(VERSION 3.22.1)

project("luna-engine")

set(${CMAKE_C_FLAGS}, "${CMAKE_C_FLAGS} -std=c89 -Wall -Werror -Wextra -Wpedantic -Wno-unused-variable -Wno-unused-parameter")
set(CMAKE_SHARED_LINKER_FLAGS
        "${CMAKE_SHARED_LINKER_FLAGS} -u ANativeActivity_onCreate")

file(GLOB_RECURSE LunaSources "engine/*.c")

add_library(native_app_glue STATIC
        ${CMAKE_ANDROID_NDK}/sources/android/native_app_glue/android_native_app_glue.c)

add_library(luna SHARED
        ${LunaSources})

target_include_directories(luna PRIVATE
        ${CMAKE_ANDROID_NDK}/sources/android/native_app_glue)

target_link_libraries(luna
        android
        native_app_glue
        EGL
        GLESv1_CM
        log)
