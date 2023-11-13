#include <android_native_app_glue.h>
#include "gl.h"
#include "luna.h"
#include "util.h"

int attributes[] = {
        EGL_RED_SIZE, 8,
        EGL_GREEN_SIZE, 8,
        EGL_BLUE_SIZE, 8,
        EGL_ALPHA_SIZE, 8,
        EGL_DEPTH_SIZE, EGL_DONT_CARE,
        EGL_NONE
};

bool LunaGLInit(struct Context *ctx) {
    int majorVersion, minorVersion;
    int numConfigs;

    if (ctx->gl.initialized) {
        /** Context already initialized */
        return false;
    }

    ctx->gl.display = eglGetDisplay(EGL_DEFAULT_DISPLAY);

    if (!ctx->gl.display) {
        LOGI("Could not get EGL ctx->gl.display");

        return false;
    }

    eglInitialize(ctx->gl.display, &majorVersion, &minorVersion);

    LOGI("EGL %d.%d initialized.", majorVersion, minorVersion);

    eglChooseConfig(ctx->gl.display, (const int *) &attributes, &ctx->gl.config, 1, &numConfigs);

    if (numConfigs <= 0) {
        LOGE("Cannot find a EGL config that matches the specified attributes:"
             "Red Bits: 8"
             "Green Bits: 8"
             "Blue Bits: 8"
             "Alpha Bits: 8"
             "Display Depth: any");

        return false;
    }

    ctx->gl.surface = eglCreateWindowSurface(
            ctx->gl.display,
            ctx->gl.config,
            ctx->app->window,
            NULL);

    if (!ctx->gl.surface) {
        LOGE("Cannot create EGL ctx->gl.surface");

        return false;
    }

    ctx->gl.context = eglCreateContext(
            ctx->gl.display,
            ctx->gl.config,
            NULL,
            (const int[]) {
                EGL_CONTEXT_CLIENT_VERSION, 3,
                EGL_NONE
            });

    if (!ctx->gl.context) {
        LOGE("Cannot create EGL context");

        return false;
    }
    
    if (!eglMakeCurrent(ctx->gl.display, ctx->gl.surface, ctx->gl.surface, ctx->gl.context)) {
        LOGE("Cannot make context current using ctx->gl.surface %p", ctx->gl.surface);

        return false;
    }

    glEnable(GL_CULL_FACE);
    glDisable(GL_DEPTH_TEST);

    ctx->gl.initialized = true;

    return true;
}

bool LunaGLUpdateContext(struct Context *ctx)
{
    if (!ctx->gl.initialized)
        return false;

    LunaGLDestroy(ctx);
    LunaGLInit(ctx);

    return true;
}

bool LunaGLDestroy(struct Context *ctx) {
    if (!ctx->gl.initialized)
        return false;

    if (ctx->gl.surface)
        eglDestroySurface(ctx->gl.display, ctx->gl.surface);

    if (ctx->gl.context)
        eglDestroyContext(ctx->gl.display, ctx->gl.context);

    if (ctx->gl.display)
        eglTerminate(ctx->gl.display);

    ctx->gl.initialized = false;

    return true;
}
