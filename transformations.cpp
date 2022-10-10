#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>

#include "common.h"
#include "shader.h"
#include "third_party/stb_image.h"

// clang-format off
float vertices[] = {
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
};

glm::vec3 cubePositions[] = {
    glm::vec3( 0.0f,  0.0f,  0.0f),
    glm::vec3( 2.0f,  5.0f, -15.0f),
    glm::vec3(-1.5f, -2.2f, -2.5f),
    glm::vec3(-3.8f, -2.0f, -12.3f),
    glm::vec3( 2.4f, -0.4f, -3.5f),
    glm::vec3(-1.7f,  3.0f, -7.5f),
    glm::vec3( 1.3f, -2.0f, -2.5f),
    glm::vec3( 1.5f,  2.0f, -2.5f),
    glm::vec3( 1.5f,  0.2f, -1.5f),
    glm::vec3(-1.3f,  1.0f, -1.5f)
};

// unsigned int indices[] = {
//     // note that we start from 0!
//     0, 1, 3, // first triangle
//     1, 2, 3  // second triangle
// };
// clang-format on

void setupVertexArrayObject(unsigned int &VAO, unsigned int &VBO /*, unsigned int &EBO*/) {
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
  // glGenBuffers(1, &EBO);

  // 1. Bind Vertex Array Object.
  glBindVertexArray(VAO);

  // 2. Copy our vertices array into a buffer for OpenGL to use.
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  // Copy our index array into an element buffer for OpenGL to use.
  // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  // glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

  // 3. Then set our vertex attribute pointers.
  // Position attribute:
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), nullptr);
  glEnableVertexAttribArray(0);
  // // Color attribute:
  // glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(3 *
  // sizeof(float))); glEnableVertexAttribArray(1); Texture attribute:
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);

  // Unbind the VBO. Note that this is allowed, the call to glVertexAttribPointer registered VBO
  // as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind.
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  // Unbind the VAO. You can unbind the VAO afterwards so other VAO calls won't accidentally
  // modify this VAO, but this rarely happens. Modifying other VAOs requires a call to
  // glBindVertexArray anyway, so we generally don't unbind VAOs (nor VBOs) when it's not
  // directly necessary.
  glBindVertexArray(0);

  // Unbind the EBO. This must come after unbinding the VAO since a bound VAO stores binds and
  // unbinds of GL_ELEMENT_ARRAY_BUFFER.
  // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

int main() {
  auto app = GlfwApplication::Create();

  Shader shader("/Users/kal/Code/learnopengl/vertex_shader.glsl",
                "/Users/kal/Code/learnopengl/fragment_shader.glsl");

  unsigned int texture1;
  glGenTextures(1, &texture1);
  glBindTexture(GL_TEXTURE_2D, texture1);
  // set the texture wrapping/filtering options (on the currently bound texture object)
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  // load and generate the texture
  int width, height, nrChannels;
  unsigned char *data =
      stbi_load("/Users/kal/Code/learnopengl/container.jpeg", &width, &height, &nrChannels, 0);
  if (data) {
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
  } else {
    std::cout << "Failed to load container texture" << std::endl;
  }
  stbi_image_free(data);

  unsigned int texture2;
  glGenTextures(1, &texture2);
  glBindTexture(GL_TEXTURE_2D, texture2);
  // set the texture wrapping/filtering options (on the currently bound texture object)
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  stbi_set_flip_vertically_on_load(true);
  data = stbi_load("/Users/kal/Code/learnopengl/awesomeface.png", &width, &height, &nrChannels, 0);
  if (data) {
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
  } else {
    std::cout << "Failed to load awesomeface texture" << std::endl;
  }
  stbi_image_free(data);

  unsigned int VAO, VBO; //, EBO;
  setupVertexArrayObject(VAO, VBO /*, EBO*/);

  glEnable(GL_DEPTH_TEST);

  float deltaTime = 0.0f; // Time between current frame and last frame.
  float lastFrame = 0.0f; // Time of last frame.

  glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
  // After the first mouse event, cameraFront is maintained to be a function of yaw and pitch.
  // It is initialized to -z to match the initial yaw of 90 (see below).
  glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
  glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
  const float baseCameraSpeed = 2.5f;
  auto cameraSpeed = [&]() { return baseCameraSpeed * deltaTime; };

  app->OnKey(GLFW_KEY_W, [&]() { cameraPos += cameraSpeed() * cameraFront; });
  app->OnKey(GLFW_KEY_S, [&]() { cameraPos -= cameraSpeed() * cameraFront; });
  app->OnKey(GLFW_KEY_A, [&]() {
    cameraPos -= cameraSpeed() * glm::normalize(glm::cross(cameraFront, cameraUp));
  });
  app->OnKey(GLFW_KEY_D, [&]() {
    cameraPos += cameraSpeed() * glm::normalize(glm::cross(cameraFront, cameraUp));
  });

  // https://stackoverflow.com/questions/10569659/camera-pitch-yaw-to-direction-vector
  // Pitch and yaw of 0 correspond to (1,0,0), i.e. the default direction is +x.
  // Default to looking in the -z direction.
  float pitch = 0.0f, yaw = 90.0f;

  app->DisableCursor();
  // Initialize mouse position to the center of the 800x600 screen.
  float lastX = 400, lastY = 300;
  bool first_mouse = true;
  app->OnMouse([&](double x, double y) {
    if (first_mouse) {
      lastX = x;
      lastY = y;
      first_mouse = false;
    }
    const float sensitivity = 0.1f;
    float x_offset = sensitivity * (x - lastX);
    float y_offset = sensitivity * (y - lastY);
    lastX = x;
    lastY = y;

    yaw -= x_offset;
    pitch = std::clamp(pitch - y_offset, -90.0f, 90.0f);

    // First consider the cartesian point (x,y,z) in cylindrical coordinates:
    //   x = r*cos(yaw)
    //   y = y
    //   z = -r*sin(yaw)
    // where r is the radius from the origin to (x,0,z). Note that r < 1 unless y = 0, where r = 1.
    //
    // Let rho be the radius from the origin to (x,y,z). Consider the triangle formed by r and rho
    // with the angle between them being the pitch. Then
    //   r = rho*cos(pitch)
    // and so
    //  x = rho*cos(pitch)*cos(yaw)
    //  y = rho*sin(pitch)
    //  z = -rho*cos(pitch)*sin(yaw)
    //
    // This is not equivalent to rotating the initial direction vector (1,0,0) about the y axis
    // by yaw and then about the (new) z axis by pitch.
    //   R_z(pitch) * R_y(yaw) * (1,0,0)^T
    //
    cameraFront = glm::vec3(std::cos(glm::radians(pitch)) * std::cos(glm::radians(yaw)),
                            std::sin(glm::radians(pitch)),
                            std::cos(glm::radians(pitch)) * -std::sin(glm::radians(yaw)));
  });

  float fov = 45.0f;
  app->OnScroll([&]([[maybe_unused]] double x, double y) {
    fov -= (float)y;
    fov = std::clamp(fov - (float)y, 0.0f, 45.0f);
  });

  app->Run([&]() {
    float currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture1);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texture2);

    // Activate the shader before setting the uniform.
    shader.use();
    shader.setInt("texture1", 0);
    shader.setInt("texture2", 1);

    // glm::mat4 model = glm::mat4(1.0f);
    // model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(0.5f, 1.0f, 1.0f));
    // shader.set("model", model);

    // glm::mat4 view = glm::mat4(1.0f);
    // view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
    // shader.set("view", view);

    glm::mat4 projection = glm::perspective(glm::radians(fov), 800.0f / 600.0f, 0.1f, 100.0f);
    shader.set("projection", projection);

    glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
    shader.set("view", view);

    glBindVertexArray(VAO);
    // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    // glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
    for (unsigned int i = 0; i < 10; i++) {
      glm::mat4 model = glm::mat4(1.0f);
      model = glm::translate(model, cubePositions[i]);
      float angle = 20.f * i;
      model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
      shader.set("model", model);

      glDrawArrays(GL_TRIANGLES, 0, 36);
    }

    //////////////////////////////////////////////////
    glBindVertexArray(0);
  });

  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);
}