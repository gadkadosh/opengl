#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include "IndexBuffer.h"
#include "Renderer.h"
#include "Shader.h"
#include "Texture.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "test/Test.h"
#include "test/TestClearColor.h"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include "test/TestTriangle.h"

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>

int main(void) {
  GLFWwindow *window;

  /* Initialize the library */
  if (!glfwInit())
    return -1;

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  /* Create a windowed mode window and its OpenGL context */
  window = glfwCreateWindow(960, 540, "Hello World", NULL, NULL);
  if (!window) {
    glfwTerminate();
    return -1;
  }

  /* Make the window's context current */
  glfwMakeContextCurrent(window);

  glfwSwapInterval(1);

  if (glewInit() != GLEW_OK)
    std::cout << "Error!" << std::endl;

  std::cout << glGetString(GL_VERSION) << std::endl;

  GLCall(glEnable(GL_BLEND));
  GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

  Renderer renderer;

  ImGui::CreateContext();
  ImGui::StyleColorsDark();
  ImGui_ImplGlfw_InitForOpenGL(window, true);
  const char *glsl_version = "#version 330";
  ImGui_ImplOpenGL3_Init(glsl_version);

  test::Test *currentTest = nullptr;
test:
  test::TestMenu *testMenu = new test::TestMenu(currentTest);
  currentTest = testMenu;

  testMenu->RegisterTest<test::TestClearColor>("Clear Color");
  testMenu->RegisterTest<test::TestTriangle>("Triangle");

  while (!glfwWindowShouldClose(window)) {
    renderer.Clear();
    float color[4] = {0.0f, 0.0f, 0.0f, 1.0f};
    renderer.SetClearColor(color);

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    if (currentTest) {
      currentTest->OnUpdate(0.0f);
      currentTest->OnRender();
      ImGui::Begin("Test");
      if (currentTest != testMenu && ImGui::Button("<-"))
        currentTest = testMenu;

      currentTest->OnImGuiRender();
      ImGui::End();
    }

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    glfwSwapBuffers(window);

    glfwPollEvents();
  }

  delete currentTest;
  if (currentTest != testMenu)
    delete testMenu;

  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();

  glfwTerminate();
  return 0;
}
