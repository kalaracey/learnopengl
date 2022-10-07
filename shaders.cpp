#include "common.h"
#include <cmath>

float vertices[] = {-0.5f, -0.5f, 0.0f, //
                    0.5f,  -0.5f, 0.0f, //
                    0.0f,  0.5f,  0.0f};

const char *vertexShaderSource = R"(
#version 330 core
layout (location = 0) in vec3 aPos; // The position variable has attribute position 0.

out vec4 vertexColor; // Specify a color output to the fragment shader.

void main() {
  gl_Position = vec4(aPos, 1.0);
  vertexColor = vec4(0.5, 0.0, 0.0, 1.0); // Dark red.
}
)";

const char *fragmentShaderSource = R"(
#version 330 core
out vec4 FragColor;

uniform vec4 ourColor; // we set this variable in the OpenGL code.

void main() {
  FragColor = ourColor;
}
)";

unsigned int setupVertexShader() {
  unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
  glCompileShader(vertexShader);
  checkShaderCompilationStatus(vertexShader, "vertex shader");
  return vertexShader;
}

unsigned int setupFragmentShader() {
  unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
  glCompileShader(fragmentShader);
  checkShaderCompilationStatus(fragmentShader, "fragment shader");
  return fragmentShader;
}

unsigned int setupShaderProgram() {
  unsigned int vertexShader = setupVertexShader(), fragmentShader = setupFragmentShader();

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

void setupVertexArrayObject(unsigned int &VAO, unsigned int &VBO) {
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);

  glBindVertexArray(VAO);

  // 2. Copy our vertices array into a buffer for OpenGL to use.
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  // 3. Then set our vertex attribute pointers.
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);

  // Unbind the VBO. Note that this is allowed, the call to glVertexAttribPointer registered VBO
  // as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind.
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  // Unbind the VAO. You can unbind the VAO afterwards so other VAO calls won't accidentally
  // modify this VAO, but this rarely happens. Modifying other VAOs requires a call to
  // glBindVertexArray anyway, so we generally don't unbind VAOs (nor VBOs) when it's not
  // directly necessary.
  glBindVertexArray(0);
}

int main() {
  auto app = GlfwApplication::create();

  unsigned int shaderProgram = setupShaderProgram();

  unsigned int VAO, VBO;
  setupVertexArrayObject(VAO, VBO);

  app->run([&]() {
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // Activate the shader before setting the uniform.
    glUseProgram(shaderProgram);

    float timeValue = glfwGetTime();
    float greenValue = (sin(timeValue) / 2.0f) + 0.5f;
    int vertexColorLocation = glGetUniformLocation(shaderProgram, "ourColor");
    glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);

    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 3);
  });

  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);
  glDeleteProgram(shaderProgram);
}