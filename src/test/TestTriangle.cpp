#include "TestTriangle.h"

#include "imgui/imgui.h"
#include <iostream>

namespace test {

TestTriangle::TestTriangle()
    : m_Positions{-1.0f, -1.0f, 1.0f, -1.0f, -0.5f, 1.0f}, m_Color{1.0f, 1.0f,
                                                                   1.0f, 1.0f} {
  unsigned int indices[] = {0, 1, 2};

  m_IndexBuffer = std::make_unique<IndexBuffer>(indices, 3);

  m_VertexBufferLayout = std::make_unique<VertexBufferLayout>();
  m_VertexBufferLayout->Push<float>(2);

  m_Shader = std::make_unique<Shader>("res/shaders/Basic.shader");
}

TestTriangle::~TestTriangle() {}

void TestTriangle::OnUpdate(float deltaTime) {}

void TestTriangle::OnRender() {
  VertexBuffer vb(m_Positions, 3 * 2 * sizeof(float));
  VertexArray va;
  va.AddBuffer(vb, *m_VertexBufferLayout);

  m_Shader->Bind();
  m_Shader->SetUniform4f("u_Color", m_Color[0], m_Color[1], m_Color[2],
                         m_Color[3]);

  Renderer renderer;
  renderer.Draw(va, *m_IndexBuffer, *m_Shader);
}

void TestTriangle::OnImGuiRender() {
  ImGui::SliderFloat2("Triangle Vertex 1", m_Positions, -1.0f, 1.0f);
  ImGui::SliderFloat2("Triangle Vertex 2", &m_Positions[2], -1.0f, 1.0f);
  ImGui::SliderFloat2("Triangle Vertex 3", &m_Positions[4], -1.0f, 1.0f);
  ImGui::ColorEdit4("Triangle Color", m_Color);
}

} // namespace test
