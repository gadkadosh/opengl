#pragma once

#include "Renderer.h"
#include "Test.h"
#include "VertexBufferLayout.h"

namespace test {

class TestTriangle : public Test {
public:
  TestTriangle();
  ~TestTriangle();

  void OnUpdate(float deltaTime);
  void OnRender();
  void OnImGuiRender();

private:
  float m_Positions[6];
  float m_Color[4];

  std::unique_ptr<IndexBuffer> m_IndexBuffer;
  std::unique_ptr<VertexBufferLayout> m_VertexBufferLayout;
  std::unique_ptr<Shader> m_Shader;
};

} // namespace test
