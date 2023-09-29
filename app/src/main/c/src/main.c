#include <unistd.h>
#include <GLES3/gl3.h>
#include "luna.h"
#include "util.h"

int r, g, b;

bool AppStart(struct App *app)
{
    LOGI("AppStart");

    r = 0;
    g = 0;
    b = 0;

    return true;
}

bool AppStop(struct App *app)
{
    LOGI("AppStop");

    return true;
}

bool AppRender(struct App *app, float delta_time)
{
    glClearColor(
            (float) r / 255.f,
            (float) g / 255.f,
            (float) b / 255.f,
            1);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    return true;
}

bool AppUpdate(struct App *app, float delta_time)
{
    r = (r + 1) % 255;
    g = (g + 1) % 255;
    b = (b + 1) % 255;

    return true;
}

int LunaEntryPoint(struct Context *ctx)
{
    struct App app;

    app.start = AppStart;
    app.stop = AppStop;
    app.render = AppRender;
    app.update = AppUpdate;

    LunaInitApp(ctx, &app);

    LunaRunApp(&app);

    LunaDestroyApp(&app);

    return 0x00;
}