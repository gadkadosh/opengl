#pragma once

#include "Test.h"

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
  unsigned int m_Indices[3];
  float m_Color[4];
};

} // namespace test
