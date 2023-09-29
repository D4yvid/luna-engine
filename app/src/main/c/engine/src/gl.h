#ifndef LUNA_ENGINE_GL_H
#define LUNA_ENGINE_GL_H

#include <EGL/egl.h>
#include <GLES3/gl3.h>
#include "luna.h"

bool LunaGLInit(struct Context *ctx);

bool LunaGLUpdateContext(struct Context *ctx);

bool LunaGLDestroy(struct Context *ctx);

#endif //LUNA_ENGINE_GL_H
