#include <stdbool.h>
#include "driver.h"
#include "../../../luna/util.h"
#include <EGL/egl.h>

internal char *currentError;

char *LunaVideoGetError()
{
    return currentError;
}

bool LunaVideoTryInit()
{
    EGLDisplay dpy;
    EGLint major, minor;

    currentError = NULL;

    dpy = eglGetDisplay(EGL_DEFAULT_DISPLAY);

    if (dpy == NULL) {
        currentError = "Could not get default EGL display";

        return false;
    }

    eglInitialize(dpy, &major, &minor);

    if (major != 3 && minor != 3) {
        // Not OpenGL ES 3.3
        currentError = "No OpenGL 3.3 supported.";

        eglTerminate(dpy);
        return false;
    }

    eglTerminate(dpy);

    return true;
}

bool LunaVideoInit()
{

}

bool LunaVideoDestroy()
{

}
