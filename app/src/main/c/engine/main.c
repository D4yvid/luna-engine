#include <android_native_app_glue.h>
#include "main.h"

struct engine engine = { 0 };

static void android_window_init(struct android_app *app) {
    if (app->window == NULL) {
        LOGW("The app initialized without a window associated with it");
        return;
    }

    if (!gl_init(&engine)) {
        LOGE("Could not initialize OpenGL! exiting...");

        gl_destroy(&engine);

        return;
    }
}

static void android_save_app_state(struct android_app *app) {
    LOGI("Save state");
}

static void android_window_destroy(struct android_app *app) {
    gl_destroy(&engine);
}

static void android_window_focus(struct android_app *app) {
    engine.drawing = true;
}

static void android_window_unfocus(struct android_app *app) {
    engine.drawing = false;
}

static void android_handle_commands(struct android_app *app, i32 command) {
    switch (command) {
        case APP_CMD_INIT_WINDOW: android_window_init(app); break;
        case APP_CMD_SAVE_STATE: android_save_app_state(app); break;
        case APP_CMD_TERM_WINDOW: android_window_destroy(app); break;
        case APP_CMD_GAINED_FOCUS: android_window_focus(app); break;
        case APP_CMD_LOST_FOCUS: android_window_unfocus(app); break;
    }
}

void android_main(struct android_app* state) {
    struct android_poll_source *source;
    int identifier, events;

    engine.app = state;

    LOGI("Hello, world!");

    state->onAppCmd = android_handle_commands;

    engine.running = true;

    while (engine.running) {
        while ((identifier = ALooper_pollAll(
                engine.drawing ? 0 : -1,
                NULL,
                &events,
                (void **) &source)) >= 0) {

            if (source != NULL)
                source->process(state, source);

            if (state->destroyRequested != 0) {
                engine.running = false;
                break;
            }
        }

        if (engine.drawing && engine.initialized) {
            glClearColor(0.0, 0.4, 1, 1);
            glClear(GL_COLOR_BUFFER_BIT);

            eglSwapBuffers(engine.gl_context.display, engine.gl_context.surface);
        }
    }

    LOGI("Exiting...");
}