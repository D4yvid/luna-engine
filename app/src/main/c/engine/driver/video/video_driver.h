#ifndef LUNA_ENGINE_VIDEO_DRIVER_H
#define LUNA_ENGINE_VIDEO_DRIVER_H

#include <EGL/egl.h>

#ifdef GLES_IMPL
struct VideoDriverContext {
    EGLDisplay display;
    EGLConfig config;
    EGLSurface surface;
    EGLContext eglContext;

    EGLint eglMajor, eglMinor;

    bool initialized, current, windowAttached;
};
#else
#error No OpenGL implementation is in use
#endif

char *LunaVideoGetError();

bool LunaVideoInit(struct VideoDriverContext *context);

bool LunaVideoMakeCurrent(struct VideoDriverContext *context);

bool LunaVideoSwapBuffers(struct VideoDriverContext *context);

bool LunaVideoCreateWindow(struct VideoDriverContext *context, void *nativeWindow);
bool LunaVideoDestroyWindow(struct VideoDriverContext *context);

bool LunaVideoDestroy(struct VideoDriverContext *context);

#endif
