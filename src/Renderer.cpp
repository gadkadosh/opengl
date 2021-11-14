#include "Renderer.h"

#include <iostream>

void GLClearErrors() {
  while (glGetError() != GL_NO_ERROR) {
  }
}

bool GLLogCall(const char *function, const char *file, int line) {
  while (GLenum error = glGetError()) {
    std::cout << "OpenGL Error " << error << ": " << function << " " << file
              << ":" << line << std::endl;
    return false;
  }
  return true;
}

void Renderer::Clear() const { GLCall(glClear(GL_COLOR_BUFFER_BIT)); }

void Renderer::SetClearColor(float color[4]) const {
  GLCall(glClearColor(color[0], color[1], color[2], color[3]));
}

void Renderer::Draw(const VertexArray &va, const IndexBuffer &ib,
                    const Shader &shader) const {
  shader.Bind();
  va.Bind();
  ib.Bind();
  GLCall(glDrawElements(GL_TRIANGLES, ib.GetCount(), GL_UNSIGNED_INT, nullptr));
}
