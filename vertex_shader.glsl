#version 330 core
layout (location = 0) in vec3 aPos;   // The position variable has attribute position 0.
layout (location = 1) in vec3 aColor;

out vec3 ourColor; // Specify a color output to the fragment shader.
out vec3 ourPosition;

uniform float offset;

void main() {
  gl_Position = vec4(aPos.x + offset, aPos.y, aPos.z, 1.0);
  ourColor = aColor;
  ourPosition = gl_Position.xyz;
}