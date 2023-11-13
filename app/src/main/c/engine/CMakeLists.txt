cmake_minimum_required(VERSION 3.22.1)

set(${CMAKE_C_FLAGS}, "${CMAKE_C_FLAGS} -std=c89 -Wall -Werror -Wextra -Wpedantic -Wno-unused-variable -Wno-unused-parameter")
set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -u ANativeActivity_onCreate")

project("luna-engine-core")

add_subdirectory(driver/video)

target_compile_definitions(LunaVideoDriver PUBLIC GLES_IMPL)

add_library(native_app_glue STATIC
        ${CMAKE_ANDROID_NDK}/sources/android/native_app_glue/android_native_app_glue.c)

add_library(LunaCore STATIC
        luna/luna.c)

target_include_directories(LunaCore
        PRIVATE
            ${CMAKE_ANDROID_NDK}/sources/android/native_app_glue
        PUBLIC
            .)

target_link_libraries(LunaCore
        LunaVideoDriver
        android
        native_app_glue
        log)
