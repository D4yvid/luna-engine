#include <stdbool.h>
#include "video_driver.h"
#include "../../../luna/util.h"
#include <EGL/egl.h>

internal char *currentError;
internal int EGL_CONFIG_ATTRIBUTES[] = {
        EGL_RED_SIZE, 8,
        EGL_GREEN_SIZE, 8,
        EGL_BLUE_SIZE, 8,
        EGL_ALPHA_SIZE, 8,
        EGL_NONE
};

char *LunaVideoGetError()
{
    return currentError;
}

bool LunaVideoInit(struct VideoDriverContext *context)
{
    EGLint numConfigs;

    if (context->initialized) {
        currentError = "Video driver context already initialized";

        return false;
    }

    context->display = eglGetDisplay(EGL_DEFAULT_DISPLAY);

    if (!context->display) {
        currentError = "Could not get default display";

        return false;
    }

    if (!eglInitialize(context->display, &context->eglMajor, &context->eglMinor)) {
        currentError = "Could not initialize EGL";
    }

    eglChooseConfig(context->display,
                    (const EGLint *) EGL_CONFIG_ATTRIBUTES,
                    &context->config,
                    1,
                    &numConfigs);

    if (numConfigs <= 0 || context->config == NULL) {
        currentError = "Could not find a suitable EGL configuration";

        return false;
    }

    context->initialized = true;

    return true;
}

bool LunaVideoCreateWindow(struct VideoDriverContext *context, void *nativeWindow)
{
    if (!nativeWindow) {
        currentError = "Invalid native window given";

        return false;
    }

    if (!context->initialized) {
        currentError = "Video driver context not initialized";

        return false;
    }

    context->surface = eglCreateWindowSurface(
            context->display,
            context->config,
            nativeWindow,
            NULL);

    if (!context->surface) {
        currentError = "Could not create EGL surface from native window";

        return false;
    }

    context->eglContext = eglCreateContext(
            context->display,
            context->config,
            NULL,
            NULL);

    if (!context->eglContext) {
        currentError = "Could not create EGL context";

        return false;
    }

    context->windowAttached = true;
    context->current = false;

    return true;
}

bool LunaVideoDestroyWindow(struct VideoDriverContext *context)
{
    if (!context->initialized) {
        currentError = "Video context not initialized";

        return false;
    }

    if (context->surface) {
        eglDestroySurface(context->display, context->surface);

        context->surface = NULL;
    }

    if (context->eglContext) {
        eglDestroyContext(context->display, context->eglContext);

        context->eglContext = NULL;
    }

    context->windowAttached = false;
    context->current = false;

    return true;
}

bool LunaVideoDestroy(struct VideoDriverContext *context)
{
    if (!context->initialized) {
        currentError = "Video driver context not initialized";

        return false;
    }

    if (context->surface) {
        eglDestroySurface(context->display, context->surface);

        context->surface = NULL;
    }

    if (context->eglContext) {
        eglDestroyContext(context->display, context->eglContext);

        context->eglContext = NULL;
    }

    if (context->display) {
        eglTerminate(context->display);

        context->display = NULL;
    }

    context->initialized = false;
    context->eglMajor = context->eglMinor = 0;
    context->config = NULL;

    return true;
}

bool LunaVideoSwapBuffers(struct VideoDriverContext *context)
{
    return eglSwapBuffers(context->display, context->surface);
}

bool LunaVideoMakeCurrent(struct VideoDriverContext *context)
{
    if (!context->initialized) {
        currentError = "Video driver context not initialized";

        return false;
    }

    if (!context->windowAttached) {
        currentError = "There is no window attached to the video context";

        return false;
    }

    if (!context->surface) {
        currentError = "Invalid video context driver surface";

        return false;
    }

    if (!eglMakeCurrent(context->display,context->surface,
                        context->surface,context->eglContext)) {
        currentError = "Could not make context current";

        return false;
    }

    context->current = true;

    return true;
}
