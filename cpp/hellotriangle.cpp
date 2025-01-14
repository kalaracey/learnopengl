#include <iostream>

// clang-format off
#include <glad/glad.h>
#include <GLFW/glfw3.h>
// clang-format on

void framebuffer_size_callback([[maybe_unused]] GLFWwindow *window, int width, int height) {
  glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, true);
  }
}

// float vertices[] = {
//     0.5f,  0.5f,  0.0f, // top right
//     0.5f,  -0.5f, 0.0f, // bottom right
//     -0.5f, -0.5f, 0.0f, // bottom left
//     -0.5f, 0.5f,  0.0f  // top left
// };

float vertices_a[] = {
    // first triangle
    -0.5f, -0.5f, 0.0f, // bottom left
    0.0f,  -0.5f, 0.0f, // bottom center
    0.0f,  0.5f,  0.0f, // top center
};

float vertices_b[] = {
    // second triangle
    0.5f, -0.5f, 0.0f, // bottom right
    0.0f, -0.5f, 0.0f, // bottom center
    0.0f, 0.5f,  0.0f, // top center
};

// unsigned int indices[] = {
//     // note that we start from 0!
//     0, 1, 3, // first triangle
//     1, 2, 3  // second triangle
// };

const char *vertexShaderSource = "#version 330 core\n"
                                 "layout (location = 0) in vec3 aPos;\n"
                                 "void main()\n"
                                 "{\n"
                                 "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
                                 "}\0";

const char *fragmentShaderSourceOrange = "#version 330 core\n"
                                   "out vec4 FragColor;\n"
                                   "void main()\n"
                                   "{\n"
                                   "  FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
                                   "}\0";

const char *fragmentShaderSourceYellow = "#version 330 core\n"
                                         "out vec4 FragColor;\n"
                                         "void main()\n"
                                         "{\n"
                                         "  FragColor = vec4(255/255.0f, 191/255.0f, 0/255.0f, 1.0f);\n"
                                         "}\0";

void checkShaderCompilationStatus(unsigned int shader, const std::string &name) {
  int success;
  char infoLog[512];
  glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

  if (!success) {
    glGetShaderInfoLog(shader, 512, NULL, infoLog);
    std::cout << "shader compilation failed: " << name << ": " << infoLog << std::endl;
    exit(EXIT_FAILURE);
  }
}

unsigned int setupVertexShader() {
  unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
  glCompileShader(vertexShader);
  checkShaderCompilationStatus(vertexShader, "vertex shader");
  return vertexShader;
}

unsigned int setupFragmentShader(const char *fragmentShaderSource) {
  unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
  glCompileShader(fragmentShader);
  checkShaderCompilationStatus(fragmentShader, "fragment shader");
  return fragmentShader;
}

unsigned int setupShaderProgram(const char *fragmentShaderSource) {
  unsigned int vertexShader = setupVertexShader(), fragmentShader = setupFragmentShader(fragmentShaderSource);

  unsigned int shaderProgram = glCreateProgram();
  glAttachShader(shaderProgram, vertexShader);
  glAttachShader(shaderProgram, fragmentShader);
  glLinkProgram(shaderProgram);
  int success;
  char infoLog[512];
  glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
  if (!success) {
    glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
    std::cout << "shader program link failure: " << infoLog << std::endl;
    exit(EXIT_FAILURE);
  }

  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);

  return shaderProgram;
}

void setupVertexArrayObject(unsigned int &VAO, float *vertices, long vertices_size) {
  unsigned int VBO;
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);

  // 1. Bind Vertex Array Object.
  glBindVertexArray(VAO);

  // 2. Copy our vertices array into a buffer for OpenGL to use.
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, vertices_size, vertices, GL_STATIC_DRAW);

  // 3. Then set our vertex attribute pointers.
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);

  // Unbind the VBO. Note that this is allowed, the call to glVertexAttribPointer registered VBO as
  // the vertex attribute's bound vertex buffer object so afterwards we can safely unbind.
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  // Unbind the VAO. You can unbind the VAO afterwards so other VAO calls won't accidentally modify
  // this VAO, but this rarely happens. Modifying other VAOs requires a call to glBindVertexArray
  // anyway, so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
  glBindVertexArray(0);
}

// void setupVertexArrayObject(unsigned int &VAO, unsigned int &VBO, unsigned int &EBO) {
// void setupVertexArrayObject(unsigned int &VAO, unsigned int &VBO) {
void setupVertexArrayObjects(unsigned int &VAO1, unsigned int &VAO2) {
  setupVertexArrayObject(VAO1, vertices_a, sizeof(vertices_a));
  setupVertexArrayObject(VAO2, vertices_b, sizeof(vertices_b));
}

int main() {
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

  GLFWwindow *window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
  if (window == NULL) {
    std::cout << "Failed to Create GLFW window" << std::endl;
    glfwTerminate();
    return -1;
  }
  glfwMakeContextCurrent(window);
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cout << "Failed to initialize GLAD" << std::endl;
    return -1;
  }

  //    glViewport(0, 0, 800, 600);

  unsigned int orangeShaderProgram = setupShaderProgram(fragmentShaderSourceOrange);
  unsigned int yellowShaderProgram = setupShaderProgram(fragmentShaderSourceYellow);

  unsigned int VAO1, VAO2; //, VBO1; //, EBO;
  setupVertexArrayObjects(VAO1, VAO2 /*, EBO*/);

  while (!glfwWindowShouldClose(window)) {
    processInput(window);

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    glUseProgram(orangeShaderProgram);

    glBindVertexArray(VAO1);
    glDrawArrays(GL_TRIANGLES, 0, 3);

    glUseProgram(yellowShaderProgram);
    glBindVertexArray(VAO2);
    glDrawArrays(GL_TRIANGLES, 0, 3);

    glBindVertexArray(0);

    // glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glfwTerminate();
  return 0;
}
