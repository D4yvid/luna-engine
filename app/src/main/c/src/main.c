#include <unistd.h>
#include <GLES3/gl3.h>
#include "../engine/luna/luna.h"
#include "../engine/luna/util.h"

int r, g, b;

bool AppStart(struct LunaApplication *app)
{
    LOGI("AppStart");

    app->running = true;

    r = 0;
    g = 0;
    b = 0;

    return true;
}

bool AppStop(struct LunaApplication *app)
{
    LOGI("AppStop");

    return true;
}

bool AppRender(struct LunaApplication *app, float delta_time)
{
    glClearColor(
            (float) r / 255.f,
            (float) g / 255.f,
            (float) b / 255.f,
            1);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    return true;
}

bool AppUpdate(struct LunaApplication *app, float delta_time)
{
    r = (r + 1) % 255;
    g = (g + 1) % 255;
    b = (b + 1) % 255;

    return true;
}

int LunaEntryPoint(struct EngineContext *ctx, struct LunaApplication *application)
{
    application->stop = AppStop;
    application->start = AppStart;
    application->render = AppRender;
    application->update = AppUpdate;
    application->state = NULL;

    return 0;
}
