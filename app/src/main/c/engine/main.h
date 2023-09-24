#ifndef LUNA_ENGINE_MAIN_H
#define LUNA_ENGINE_MAIN_H

#include "gl.h"
#include "util.h"

struct engine
{
    struct android_app *app;

    bool running, drawing, initialized;

    struct {
        EGLDisplay display;
        EGLConfig config;
        EGLSurface surface;
        EGLContext context;
    } gl_context;
};

// See engine.c
extern struct engine engine;

#endif //LUNA_ENGINE_MAIN_H
