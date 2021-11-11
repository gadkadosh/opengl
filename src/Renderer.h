#pragma once

#include <GL/glew.h>

#define ASSERT(x)                                                              \
  if (!(x))                                                                    \
    __builtin_debugtrap();

#define GLCall(x)                                                              \
  GLClearErrors();                                                             \
  x;                                                                           \
  ASSERT(GLLogCall(#x, __FILE__, __LINE__))

void GLClearErrors();

bool GLLogCall(const char *function, const char *file, int line);
