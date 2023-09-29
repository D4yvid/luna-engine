#ifndef LUNA_ENGINE_LUNA_H
#define LUNA_ENGINE_LUNA_H

#include <EGL/egl.h>
#include <stdbool.h>
#include <android_native_app_glue.h>

struct Context
{
    struct android_app *app;
    struct App *lunaApp;

    struct {
        EGLDisplay display;
        EGLConfig config;
        EGLSurface surface;
        EGLContext context;

        bool initialized;
    } gl;
};

struct App
{
    struct Context *ctx;
    bool running, paused;

    bool (*start)(struct App *app);

    bool (*update)(struct App *app, float delta_time);
    bool (*render)(struct App *app, float delta_time);

    bool (*stop)(struct App *app);
};

struct State
{
    struct Context ctx;
    struct App app;
};

bool LunaCreateContext(struct Context *ctx, struct android_app *app);

bool LunaInitApp(struct Context *ctx, struct App *target);

bool LunaRunApp(struct App *app);
bool LunaPauseApp(struct App *app);
bool LunaStopApp(struct App *app);

bool LunaDestroyApp(struct App *app);

int LunaEntryPoint(struct Context *ctx);
void android_main(struct android_app* state);

#endif //LUNA_ENGINE_LUNA_H
