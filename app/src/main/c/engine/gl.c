#include <android_native_app_glue.h>
#include "gl.h"

i32 attributes[] = {
        EGL_RED_SIZE, 8,
        EGL_GREEN_SIZE, 8,
        EGL_BLUE_SIZE, 8,
        EGL_ALPHA_SIZE, 8,
        EGL_DEPTH_SIZE, EGL_DONT_CARE,
        EGL_NONE
};

static void print_gl_info() {
    static i32 informationList[] = { GL_VENDOR, GL_RENDERER, GL_VERSION, GL_EXTENSIONS };
    static char *informationText[] = {
            [GL_VENDOR] = "OpenGL Vendor",
            [GL_RENDERER] = "OpenGL Renderer",
            [GL_VERSION] = "OpenGL Version",
            [GL_EXTENSIONS] = "OpenGL Extensions"
    };

    LOGI("----- OpenGL Information -----");

    for (i32 i = 0; i < sizeof(informationList) / sizeof(informationList[0]); i++) {
        char *name = informationText[informationList[i]];

        LOGI("\t%s: %s", name, glGetString(informationList[i]));
    }

    LOGI("----- END OpenGL Information -----");
}

bool gl_init(struct engine *engine) {
    i32 majorVersion, minorVersion;
    i32 numConfigs;

    engine->gl_context.display = eglGetDisplay(EGL_DEFAULT_DISPLAY);

    if (!engine->gl_context.display) {
        LOGI("Could not get EGL engine->gl_context.display");

        return false;
    }

    eglInitialize(engine->gl_context.display, &majorVersion, &minorVersion);

    LOGI("EGL %d.%d initialized.", majorVersion, minorVersion);

    eglChooseConfig(engine->gl_context.display, (const i32 *) &attributes, &engine->gl_context.config, 1, &numConfigs);

    if (numConfigs <= 0) {
        LOGE("Cannot find a EGL engine->gl_context.config that matches the specified attributes:"
             "Red Bits: 8"
             "Green Bits: 8"
             "Blue Bits: 8"
             "Alpha Bits: 8"
             "Display Depth: any");

        return false;
    }

    engine->gl_context.surface = eglCreateWindowSurface(
            engine->gl_context.display,
            engine->gl_context.config,
            engine->app->window,
            NULL);

    if (!engine->gl_context.surface) {
        LOGE("Cannot create EGL engine->gl_context.surface");

        return false;
    }

    engine->gl_context.context = eglCreateContext(
            engine->gl_context.display,
            engine->gl_context.config,
            NULL,
            (const i32[]) {
                EGL_CONTEXT_CLIENT_VERSION, 3,
                EGL_NONE
            });

    if (!engine->gl_context.context) {
        LOGE("Cannot create EGL context");

        return false;
    }

    if (!eglMakeCurrent(engine->gl_context.display, engine->gl_context.surface, engine->gl_context.surface, engine->gl_context.context)) {
        LOGE("Cannot make context current using engine->gl_context.surface %p", engine->gl_context.surface);

        return false;
    }

    glEnable(GL_CULL_FACE);
    glDisable(GL_DEPTH_TEST);

    LOGI("GL initialized with success");
    print_gl_info();

    engine->initialized = true;

    return true;
}

bool gl_destroy(struct engine *engine) {
    if (engine->gl_context.surface)
        eglDestroySurface(engine->gl_context.display, engine->gl_context.surface);

    if (engine->gl_context.context)
        eglDestroyContext(engine->gl_context.display, engine->gl_context.context);

    if (engine->gl_context.display)
        eglTerminate(engine->gl_context.display);

    engine->initialized = false;

    return true;
}

