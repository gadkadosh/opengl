#pragma once

#include "IndexBuffer.h"
#include "Shader.h"
#include "VertexArray.h"
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

class Renderer {
public:
  void Clear() const;
  void SetClearColor(float color[4]) const;
  void Draw(const VertexArray &va, const IndexBuffer &ib,
            const Shader &shader) const;
};
