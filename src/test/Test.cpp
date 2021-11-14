#include "Test.h"

#include <imgui/imgui.h>
#include <iostream>

namespace test {

TestMenu::TestMenu(Test *&currentTestPointer)
    : m_CurrentTest(currentTestPointer) {}

void TestMenu::OnImGuiRender() {
  for (auto test : m_Tests) {
    if (ImGui::Button(test.first.c_str()))
      m_CurrentTest = test.second();
  }
}

/* template <typename T> void TestMenu::RegisterTest(const std::string &name) {
 */
/*   std::cout << "Registering test " << name << std::endl; */
/*   m_Tests.push_back(std::make_pair(name, []() { return new T; })); */
/* } */

} // namespace test
