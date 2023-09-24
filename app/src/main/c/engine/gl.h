#ifndef LUNA_ENGINE_GL_H
#define LUNA_ENGINE_GL_H

#include <EGL/egl.h>
#include <GLES3/gl3.h>
#include "main.h"

bool gl_init(struct engine *engine);

bool gl_destroy(struct engine *engine);

#endif //LUNA_ENGINE_GL_H
