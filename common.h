#ifndef LEARNOPENGL_GLFW_COMMON_H
#define LEARNOPENGL_GLFW_COMMON_H

#include <iostream>

#include "opengl.h"

void checkShaderCompilationStatus(unsigned int shader, const std::string &name);

unsigned int createShader(const char *source, GLenum shaderType, const std::string &name);

class GlfwApplication {
public:
  static std::unique_ptr<GlfwApplication> create() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    GLFWwindow *window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
    if (window == NULL) {
      std::cerr << "Failed to create GLFW window" << std::endl;
      glfwTerminate();
      return nullptr;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
      std::cerr << "Failed to initialize GLAD" << std::endl;
      return nullptr;
    }

    int nAttributes;
    glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nAttributes);
    std::cout << "Maximum vertex attributes supported: " << nAttributes << std::endl;

    return std::unique_ptr<GlfwApplication>(new GlfwApplication(window));
  }

  ~GlfwApplication() { glfwTerminate(); }

  void run(const std::function<void()> &draw) {
    while (!glfwWindowShouldClose(window_)) {
      processInput(window_);

      draw();

      glfwSwapBuffers(window_);
      glfwPollEvents();
    }
  }

private:
  explicit GlfwApplication(GLFWwindow *window) : window_(window) {}

  static void framebuffer_size_callback([[maybe_unused]] GLFWwindow *window, int width,
                                        int height) {
    glViewport(0, 0, width, height);
  }

  static void processInput(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
      glfwSetWindowShouldClose(window, true);
    }
  }

  GLFWwindow *window_;
};

#endif // LEARNOPENGL_GLFW_COMMON_H
