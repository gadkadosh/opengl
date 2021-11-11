#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <fstream>
#include <iostream>
#include <sstream>

#define ASSERT(x)                                                              \
  if (!(x))                                                                    \
    __builtin_debugtrap();

#define GLCall(x)                                                              \
  GLClearErrors();                                                             \
  x;                                                                           \
  ASSERT(GLLogCall(#x, __FILE__, __LINE__))

static void GLClearErrors() {
  while (glGetError() != GL_NO_ERROR) {
  }
}

static bool GLLogCall(const char *function, const char *file, int line) {
  while (GLenum error = glGetError()) {
    std::cout << "OpenGL Error " << error << ": " << function << " " << file
              << ":" << line << std::endl;
    return false;
  }
  return true;
}

struct ShaderProgramSource {
  std::string VertexSource;
  std::string FragmentSource;
};

static ShaderProgramSource ParseShader(const std::string &filepath) {
  std::ifstream stream(filepath);

  enum ShaderType { NONE = -1, VERTEX = 0, FRAGMENT = 1 };

  std::string line;
  std::stringstream ss[2];
  ShaderType type = ShaderType::NONE;

  while (getline(stream, line)) {
    if (line.find("#shader") != std::string::npos) {
      if (line.find("vertex") != std::string::npos)
        type = ShaderType::VERTEX;
      else if (line.find("fragment") != std::string::npos)
        type = ShaderType::FRAGMENT;
    } else
      ss[(int)type] << line << '\n';
  }

  return {ss[0].str(), ss[1].str()};
}

static unsigned int CompileShader(unsigned int type,
                                  const std::string &source) {
  unsigned int id = glCreateShader(type);
  const char *src = source.c_str();
  glShaderSource(id, 1, &src, nullptr);
  glCompileShader(id);

  int result;
  glGetShaderiv(id, GL_COMPILE_STATUS, &result);
  if (result == GL_FALSE) {
    int length;
    glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
    char *message = (char *)alloca(length * sizeof(char));
    glGetShaderInfoLog(id, length, &length, message);
    std::cout << "Failed to compile "
              << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader"
              << std::endl;
    std::cout << message << std::endl;
    glDeleteShader(id);
  }

  return id;
}

static unsigned int CreateShader(const std::string &vertexShader,
                                 const std::string &fragmentShader) {
  GLCall(unsigned int program = glCreateProgram());
  GLCall(unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader));
  GLCall(unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader));

  GLCall(glAttachShader(program, vs));
  GLCall(glAttachShader(program, fs));
  GLCall(glLinkProgram(program));
  GLCall(glValidateProgram(program));

  GLCall(glDeleteShader(vs));
  GLCall(glDeleteShader(fs));

  return program;
}

int main(void) {
  GLFWwindow *window;

  /* Initialize the library */
  if (!glfwInit())
    return -1;

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);

  /* Create a windowed mode window and its OpenGL context */
  window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
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

  unsigned int vao;
  GLCall(glGenVertexArrays(1, &vao));
  GLCall(glBindVertexArray(vao));

  float positions[] = {
      -0.5f,  -0.5f,  // 0
      0.5f,   -0.5f,  // 1
      0.5f,   0.5f,   // 2
      -0.5f,  0.5f,   // 3
      -0.75f, -0.75f, // 4
  };

  unsigned int indices[] = {
      0, 1, 2, // 0
      2, 3, 0, // 1
      4, 2, 3  // 2
  };

  unsigned int buffer;
  GLCall(glGenBuffers(1, &buffer));
  GLCall(glBindBuffer(GL_ARRAY_BUFFER, buffer));
  GLCall(glBufferData(GL_ARRAY_BUFFER, 6 * 2 * sizeof(float), positions,
                      GL_STATIC_DRAW));

  GLCall(glEnableVertexAttribArray(0));
  GLCall(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0));

  unsigned int ibo;
  GLCall(glGenBuffers(1, &ibo));
  GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo));
  GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, 2 * 3 * sizeof(unsigned int),
                      indices, GL_STATIC_DRAW));

  ShaderProgramSource source = ParseShader("res/shaders/Basic.shader");
  unsigned int shader =
      CreateShader(source.VertexSource, source.FragmentSource);
  GLCall(glUseProgram(shader));

  GLCall(int location = glGetUniformLocation(shader, "u_Color"));
  ASSERT(location != -1);

  float r = 0.0f;
  float inc = 0.02f;

  /* Loop until the user closes the window */
  while (!glfwWindowShouldClose(window)) {
    /* Render here */
    GLCall(glClear(GL_COLOR_BUFFER_BIT));

    GLCall(glUniform4f(location, r, 0.8f, 0.3f, 1.0f));
    GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));

    if (r > 1.0f)
      inc = -0.02f;
    else if (r < 0.0f)
      inc = 0.02f;

    r += inc;

    /* Swap front and back buffers */
    glfwSwapBuffers(window);

    /* Poll for and process events */
    glfwPollEvents();
  }

  GLCall(glDeleteProgram(shader));

  glfwTerminate();
  return 0;
}
