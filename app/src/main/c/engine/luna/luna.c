#include <stdbool.h>
#include <string.h>
#include <android_native_app_glue.h>
#include "luna.h"
#include "util.h"
#include "driver/video/video_driver.h"

internal void LunaHandleAndroidCommand(struct android_app *app, int32_t command)
{
    struct EngineContext *context = app->userData;

    switch (command)
    {
        case APP_CMD_INIT_WINDOW:
            if (!LunaVideoCreateWindow(&context->videoDriver, app->window)) {
                LOGE("Could not create window: %s", LunaVideoGetError());

                return;
            }

            if (!LunaVideoMakeCurrent(&context->videoDriver)) {
                LOGE("Could not make context current: %s", LunaVideoGetError());
            }
            break;
        case APP_CMD_TERM_WINDOW:
            if (!LunaVideoDestroyWindow(&context->videoDriver)) {
                LOGE("Could not destroy window: %s", LunaVideoGetError());

                return;
            }

            context->application.paused = true;

            break;
        case APP_CMD_GAINED_FOCUS:
            context->application.paused = false;

            break;
        case APP_CMD_LOST_FOCUS:
            context->application.paused = true;

            break;

        default:
            break;
    }
}

void android_main(struct android_app* app)
{
    struct EngineContext context;
    int code;

    memset(&context, 0, sizeof(context));

    app->userData = &context;

    // TODO: Handle input events; app->onInputEvent = LunaHandleAndroidInputEvent;
    app->onAppCmd = LunaHandleAndroidCommand;

    if ((code = LunaEntryPoint(&context, &context.application)) != 0x00) {
        LOGE("LunaEntryPoint returned %d, destroying application...", code);

        goto _destroy;
    }

    if (!LunaVideoInit(&context.videoDriver)) {
        LOGE("Could not initialize video driver: %s", LunaVideoGetError());

        goto _destroy;
    }

    // TODO: Start application
    struct android_poll_source *source;
    int identifier, events;

    context.application.paused = false;

    context.application.start(&context.application);

    while (context.application.running)
    {
        while ((identifier = ALooper_pollAll(
                !context.application.paused ? 0 : -1,
                NULL,
                &events,
                (void **) &source)) >= 0) {

            if (source != NULL)
                source->process(app, source);

            if (app->destroyRequested != 0) {
                context.application.running = false;

                goto _finish;
            }
        }

        if (context.application.paused)
            continue;

        context.application.render(&context.application, 0);
        context.application.update(&context.application, 0);

        LunaVideoSwapBuffers(&context.videoDriver);
    }

_finish:
    context.application.stop(&context.application);
    LunaVideoDestroy(&context.videoDriver);

_destroy:
    ANativeActivity_finish(app->activity);
}
