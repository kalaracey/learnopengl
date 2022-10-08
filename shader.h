#ifndef LEARNOPENGL_SHADER_H
#define LEARNOPENGL_SHADER_H

#include <string>

class Shader {
public:
  // The program ID.
  unsigned int ID;

  Shader(const char *vertexPath, const char *fragmentPath);
  ~Shader();

  // Activate the shader.
  void use() const;

  void setBool(const std::string &name, bool value) const;
  void setInt(const std::string &name, int value) const;
  void setFloat(const std::string &name, float value) const;
};

#endif // LEARNOPENGL_SHADER_H
