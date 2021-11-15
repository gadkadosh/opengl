#include "TestTriangle.h"

#include "Renderer.h"
#include "VertexBufferLayout.h"
#include "imgui/imgui.h"
#include <iostream>

namespace test {

TestTriangle::TestTriangle()
    : m_Positions{-1.0f, -1.0f, 1.0f, -1.0f, -0.5f, 1.0f}, m_Indices{0, 1, 2},
      m_Color{1.0f, 1.0f, 1.0f, 1.0f} {}

TestTriangle::~TestTriangle() {}

void TestTriangle::OnUpdate(float deltaTime) {}

void TestTriangle::OnRender() {
  VertexBuffer vb(m_Positions, 3 * 2 * sizeof(float));
  VertexBufferLayout layout;
  layout.Push<float>(2);
  IndexBuffer ib(m_Indices, 3);
  VertexArray va;
  va.AddBuffer(vb, layout);

  Shader shader("res/shaders/Basic.shader");
  shader.Bind();
  shader.SetUniform4f("u_Color", m_Color[0], m_Color[1], m_Color[2],
                      m_Color[3]);

  Renderer renderer;
  renderer.Draw(va, ib, shader);
}

void TestTriangle::OnImGuiRender() {
  ImGui::SliderFloat2("Triangle Vertex 1", m_Positions, -1.0f, 1.0f);
  ImGui::SliderFloat2("Triangle Vertex 2", &m_Positions[2], -1.0f, 1.0f);
  ImGui::SliderFloat2("Triangle Vertex 3", &m_Positions[4], -1.0f, 1.0f);
  ImGui::ColorEdit4("Triangle Color", m_Color);
}

} // namespace test
