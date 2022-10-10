#ifndef LEARNOPENGL_GLFW_COMMON_H
#define LEARNOPENGL_GLFW_COMMON_H

#include <iostream>
#include <unordered_map>

#include "opengl.h"

void checkShaderCompilationStatus(unsigned int shader, const std::string &name);

unsigned int createShader(const char *source, GLenum shaderType, const std::string &name);

class GlfwApplication {
public:
  static std::unique_ptr<GlfwApplication> Create() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    // glfwWindowHint(GLFW_COCOA_MENUBAR, GL_FALSE);

    GLFWwindow *window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
    if (window == NULL) {
      std::cerr << "Failed to Create GLFW window" << std::endl;
      glfwTerminate();
      return nullptr;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, FramebufferSizeCallback);
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

  void Run(const std::function<void()> &draw) {
    while (!glfwWindowShouldClose(window_)) {
      processInput(window_);

      draw();

      glfwSwapBuffers(window_);
      glfwPollEvents();
    }
  }

  void OnKey(int glfw_key, std::function<void()> callback) { key_callbacks_[glfw_key] = callback; }

  void OnMouse(std::function<void(double, double)> callback) {
    mouse_callback_ = callback;
    glfwSetCursorPosCallback(window_, [](GLFWwindow *w, double x, double y) {
      auto app = static_cast<GlfwApplication *>(glfwGetWindowUserPointer(w));
      (*app->mouse_callback_)(x, y);
    });
  }

  void OnScroll(std::function<void(double, double)> callback) {
    scroll_callback_ = callback;
    glfwSetScrollCallback(window_, [](GLFWwindow *w, double x, double y) {
      auto app = static_cast<GlfwApplication *>(glfwGetWindowUserPointer(w));
      (*app->scroll_callback_)(x, y);
    });
  }

  void DisableCursor() { glfwSetInputMode(window_, GLFW_CURSOR, GLFW_CURSOR_DISABLED); }

private:
  explicit GlfwApplication(GLFWwindow *window) : window_(window) {
    glfwSetWindowUserPointer(window_, this);
  }

  static void FramebufferSizeCallback([[maybe_unused]] GLFWwindow *window, int width, int height) {
    glViewport(0, 0, width, height);
  }

  void processInput(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
      glfwSetWindowShouldClose(window, true);
    }
    for (const auto &[key, callback] : key_callbacks_) {
      if (glfwGetKey(window, key) == GLFW_PRESS) {
        callback();
      }
    }
  }

  GLFWwindow *window_;
  std::unordered_map<int, std::function<void()>> key_callbacks_;
  std::optional<std::function<void(double, double)>> mouse_callback_;
  std::optional<std::function<void(double, double)>> scroll_callback_;
};

#endif // LEARNOPENGL_GLFW_COMMON_H
