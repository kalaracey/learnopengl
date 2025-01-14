#include "shader.h"

#include <fstream>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <sstream>

#include "common.h"
#include "opengl.h"

namespace {}

Shader::Shader(const char *vertexPath, const char *fragmentPath) {
  // !. Retrieve the vertex/fragment source code from filePath.
  std::string vertexCode, fragmentCode;
  std::ifstream vShaderFile, fShaderFile;

  // Ensure ifstream objects can throw exceptions.
  vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
  fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

  try {
    vShaderFile.open(vertexPath);
    fShaderFile.open(fragmentPath);
    std::stringstream vShaderStream, fShaderStream;

    vShaderStream << vShaderFile.rdbuf();
    fShaderStream << fShaderFile.rdbuf();

    vShaderFile.close();
    fShaderFile.close();

    vertexCode = vShaderStream.str();
    fragmentCode = fShaderStream.str();
  } catch (const std::ifstream::failure &e) {
    std::cerr << "Error: failed to read shader source. Error code: " << e.code().value() << " ("
              << e.code().message() << ")\n"
              << "Error category: " << e.code().category().name() << std::endl;
    exit(EXIT_FAILURE);
  }

  const char *vShaderCode = vertexCode.c_str();
  const char *fShaderCode = fragmentCode.c_str();

  // 2. Compiler shaders.
  unsigned int vertex = createShader(vShaderCode, GL_VERTEX_SHADER, "vertex shader");
  unsigned int fragment = createShader(fShaderCode, GL_FRAGMENT_SHADER, "fragment shader");

  ID = glCreateProgram();
  glAttachShader(ID, vertex);
  glAttachShader(ID, fragment);
  glLinkProgram(ID);

  int success;
  glGetProgramiv(ID, GL_LINK_STATUS, &success);
  if (!success) {
    char infoLog[512];
    glGetProgramInfoLog(ID, 512, nullptr, infoLog);
    std::cerr << "Error: shader program linking failed\n" << infoLog << std::endl;
  }

  glDeleteShader(vertex);
  glDeleteShader(fragment);
}

void Shader::use() const { glUseProgram(ID); }

void Shader::setBool(const std::string &name, bool value) const {
  glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}
void Shader::setInt(const std::string &name, int value) const {
  glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}
void Shader::setFloat(const std::string &name, float value) const {
  glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::set(const std::string &name, glm::mat4 &m) const {
  glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(m));
}

Shader::~Shader() { glDeleteProgram(ID); }
