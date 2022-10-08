#include "common.h"

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

unsigned int createShader(const char *source, GLenum shaderType, const std::string &name) {
  unsigned int shader = glCreateShader(shaderType);
  glShaderSource(shader, 1, &source, nullptr);
  glCompileShader(shader);
  checkShaderCompilationStatus(shader, name);
  return shader;
}