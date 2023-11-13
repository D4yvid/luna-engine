#ifndef LUNA_ENGINE_LUNA_H
#define LUNA_ENGINE_LUNA_H

#include <EGL/egl.h>
#include <stdbool.h>
#include <android_native_app_glue.h>
#include "driver/video/video_driver.h"

struct LunaApplication
{
    void *state;

    bool paused, running;

    bool (*start)(struct LunaApplication *app);

    bool (*update)(struct LunaApplication *app, float delta_time);
    bool (*render)(struct LunaApplication *app, float delta_time);

    bool (*stop)(struct LunaApplication *app);
};

struct EngineContext
{
    struct android_app *nativeApp;
    struct LunaApplication application;

    struct VideoDriverContext videoDriver;
};

int LunaEntryPoint(struct EngineContext *ctx, struct LunaApplication *application);
void android_main(struct android_app* state);

#endif //LUNA_ENGINE_LUNA_H
