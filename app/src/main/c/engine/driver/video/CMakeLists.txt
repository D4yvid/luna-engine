add_library(LunaVideoDriver STATIC
        impl/gles.c)

target_include_directories(LunaVideoDriver PUBLIC ./)

target_link_libraries(LunaVideoDriver
        EGL
        GLESv1_CM)