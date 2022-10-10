#ifndef LEARNOPENGL__CAMERA_H
#define LEARNOPENGL__CAMERA_H

#include <algorithm>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

/**
 * Possible options for camera movement. Used as abstraction to stay away from window-system
 * specific input methods.
 */
enum CameraMovement { FORWARD, BACKWARD, LEFT, RIGHT };

// An abstract camera class that processes input and calculates the corresponding Euler Angles,
// Vectors and Matrices for use in OpenGL
class Camera {
public:
  Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f)) : position_(position) {
    UpdateCameraVectors();
  }

  glm::mat4 ViewMatrix() { return glm::lookAt(position_, position_ + front_, up_); }

  /** Process input received from a keyboard-like input system. */
  void ProcessKeyboard(CameraMovement direction, float delta_time_s_) {
    float distance = movement_speed_ * delta_time_s_;
    if (direction == FORWARD)
      position_ += front_ * distance;
    if (direction == BACKWARD)
      position_ -= front_ * distance;
    if (direction == LEFT)
      position_ -= right_ * distance;
    if (direction == RIGHT)
      position_ += right_ * distance;
  }

  /**
   * Processes input received from a mouse input system. Expects the offset value in both x and y
   * directions.
   */
  void ProcessMouseMovement(float x_delta, float y_delta) {
    yaw_ -= mouse_sensitivity_ * x_delta;
    pitch_ = std::clamp(pitch_ - mouse_sensitivity_ * y_delta, -90.0f, 90.0f);
    UpdateCameraVectors();
  }

  /**
   * Processes input received from a mouse scroll-wheel event. Only requires input on the vertical
   * wheel axis.
   */
  void ProcessMouseScroll(double y) { zoom_ = std::clamp(zoom_ - (float)y, 0.0f, 45.0f); }

  /** Returns the field of view angle. */
  float Zoom() { return zoom_; }

private:
  void UpdateCameraVectors() {
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
    front_ = glm::vec3(std::cos(glm::radians(pitch_)) * std::cos(glm::radians(yaw_)),
                       std::sin(glm::radians(pitch_)),
                       std::cos(glm::radians(pitch_)) * -std::sin(glm::radians(yaw_)));
    // front_ should already be a unit vector since we are translating from polar coordinates with
    // radius one, and hence there is no need to call glm::normalize.

    // Normalize the vectors, because their length gets closer to 0 the more you look up or down
    // which results in slower movement.
    right_ = glm::normalize(glm::cross(front_, world_up_));
    up_ = glm::normalize(glm::cross(right_, front_));
  }

  // Camera attributes.

  glm::vec3 position_;
  // front_, up_, and right_ are functions of yaw and pitch and are maintained as such.
  glm::vec3 front_;
  glm::vec3 up_;
  glm::vec3 right_;
  const glm::vec3 world_up_ = glm::vec3(0.0f, 1.0f, 0.0f);

  // Euler angles.
  // https://stackoverflow.com/questions/10569659/camera-pitch-yaw-to-direction-vector
  // Pitch and yaw of 0 correspond to (1,0,0), i.e. the default direction is +x.
  // Default to looking in the -z direction (and hence front_ will be initialized to (0,0,-1).
  float yaw_ = 90.0f;
  float pitch_ = 0.0f;

  // Camera options.
  const float movement_speed_ = 2.5f;
  const float mouse_sensitivity_ = 0.1f;
  float zoom_ = 45.0f; // Field-of-view angle.
};

#endif // LEARNOPENGL__CAMERA_H
