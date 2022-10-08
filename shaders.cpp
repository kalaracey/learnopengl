#include "common.h"
#include "shader.h"
#include <cmath>

// clang-format off
float vertices[] = {
    // positions         // colors
    -0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,  // bottom right
     0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,  // bottom left
     0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f   // top
};
// clang-format on

void setupVertexArrayObject(unsigned int &VAO, unsigned int &VBO) {
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);

  glBindVertexArray(VAO);

  // 2. Copy our vertices array into a buffer for OpenGL to use.
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  // 3. Then set our vertex attribute pointers.
  // Position attribute:
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), nullptr);
  glEnableVertexAttribArray(0);
  // Color attribute:
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);

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

  Shader shader("/Users/kal/Code/learnopengl/vertex_shader.glsl",
                "/Users/kal/Code/learnopengl/fragment_shader.glsl");
  // unsigned int shaderProgram = setupShaderProgram();

  unsigned int VAO, VBO;
  setupVertexArrayObject(VAO, VBO);

  app->run([&]() {
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // Activate the shader before setting the uniform.
    // glUseProgram(shaderProgram);
    shader.use();

    // float timeValue = glfwGetTime();
    // float greenValue = (sin(timeValue) / 2.0f) + 0.5f;
    // int vertexColorLocation = glGetUniformLocation(shaderProgram, "ourColor");
    // glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);

    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 3);
  });

  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);
}