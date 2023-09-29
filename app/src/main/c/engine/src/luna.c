#include <stdbool.h>
#include <string.h>
#include <android_native_app_glue.h>
#include <string.h>
#include <stdlib.h>
#include <dlfcn.h>
#include "luna.h"
#include "util.h"
#include "gl.h"

bool LunaCreateContext(struct Context *ctx, struct android_app *app)
{
    if (!ctx) return false;

    memset(ctx, 0, sizeof(*ctx));

    ctx->app = app;
    ctx->gl.initialized = false;

    return true;
}

bool LunaInitApp(struct Context *ctx, struct App *target)
{
    target->ctx = ctx;
    target->running = false;
    target->paused = false;

    ctx->lunaApp = target;

    return true;
}

bool LunaRunApp(struct App *app)
{
    struct android_poll_source *source;
    int identifier, events;

    if (app->running)
        return false;

    app->running = true;
    app->paused = false;

    app->start(app);

    while (app->running)
    {
        while ((identifier = ALooper_pollAll(
                !app->paused ? 0 : -1,
                NULL,
                &events,
                (void **) &source)) >= 0) {

            if (source != NULL)
                source->process(app->ctx->app, source);

            if (app->ctx->app->destroyRequested != 0) {
                app->running = false;

                goto _finish;
            }
        }

        if (app->paused || !app->ctx->gl.initialized) {
            continue;
        }

        app->render(app, 0);
        app->update(app, 0);

        eglSwapBuffers(app->ctx->gl.display, app->ctx->gl.surface);
    }

_finish:
    return app->stop(app);
}

bool LunaPauseApp(struct App *app)
{
    app->paused = !app->paused;

    return true;
}

bool LunaStopApp(struct App *app)
{
    app->running = false;

    return true;
}

bool LunaDestroyApp(struct App *app)
{
    LunaGLDestroy(app->ctx);

    app->ctx->lunaApp = NULL;
    memset(app, 0, sizeof(*app));

    return true;
}

internal void LunaHandleAndroidCommand(struct android_app *app, int32_t command)
{
    struct Context *ctx = app->userData;
    LOGI("Command %d received", command);

    switch (command)
    {
        case APP_CMD_INIT_WINDOW:
            if (!app->window) break;

            if (ctx->gl.initialized) {
                LOGI("OpenGL Context already initialized, skipping");
                break;
            }

            LunaGLInit(ctx);
            break;
        case APP_CMD_TERM_WINDOW:
            LunaGLDestroy(ctx);

            break;
        case APP_CMD_GAINED_FOCUS:
            ctx->lunaApp->paused = false;

            break;
        case APP_CMD_LOST_FOCUS:
            ctx->lunaApp->paused = true;

            break;

        case APP_CMD_WINDOW_REDRAW_NEEDED:
            LunaGLDestroy(ctx);
            LunaGLInit(ctx);
            break;

        case APP_CMD_SAVE_STATE:
            if (!app->savedState)
                app->savedState = malloc(sizeof(struct Context));

            memcpy(app->savedState, ctx, sizeof(struct Context));

            LOGE("State saved");
            break;
        case APP_CMD_RESUME:
            if (!app->savedState)
                break;

            memcpy(ctx, app->savedState, sizeof(struct Context));

            LOGE("State recovered");
            break;
    }
}

internal int LunaHandleAndroidInputEvent(struct android_app *app, AInputEvent *event)
{
    int32_t type = AInputEvent_getType(event);

    switch (type)
    {
        case AINPUT_EVENT_TYPE_MOTION:
            LOGI("Motion inside app");
            break;
        case AINPUT_EVENT_TYPE_TOUCH_MODE:
            LOGI("Touch Mode???");
            break;
    }

    return 0;
}

void android_main(struct android_app* app)
{
    struct Context ctx;
    int code;

    memset(&ctx, 0, sizeof(ctx));

    app->userData = &ctx;

    app->onInputEvent = LunaHandleAndroidInputEvent;
    app->onAppCmd = LunaHandleAndroidCommand;

    if (!LunaCreateContext(&ctx, app)) {
        LOGE("Cannot create application context, exiting...");

        goto _destroy;
    }

    if ((code = LunaEntryPoint(&ctx)) != 0x00) {
        LOGE("LunaEntryPoint returned %d, destroying application...", code);

        goto _quit;
    }

_quit:
    LunaGLDestroy(&ctx);

_destroy:
    ANativeActivity_finish(app->activity);
}
