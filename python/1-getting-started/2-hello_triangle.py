import sys

import glfw
import glm

try:
    from OpenGL.GL import * 
except ImportError as e:
    # https://stackoverflow.com/questions/63475461/unable-to-import-opengl-gl-in-python-on-macos
    if sys.platform != 'darwin':
        raise e

    from ctypes import util
    orig_util_find_library = util.find_library
    def new_util_find_library( name ):
        res = orig_util_find_library( name )
        if res: return res
        return '/System/Library/Frameworks/'+name+'.framework/'+name
    util.find_library = new_util_find_library
    from OpenGL.GL import *


def framebuffer_size_callback(window, width: int, height: int):
    glViewport(0, 0, width, height)

def process_input(window):
    if glfw.get_key(window, glfw.KEY_ESCAPE) == glfw.PRESS:
        glfw.set_window_should_close(window, True)

VERTEX_SHADER_SOURCE = """
#version 330 core
layout (location = 0) in vec3 aPos;

void main()
{
    gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
}
"""

FRAGMENT_SHADER_SOURCE = """
#version 330 core
out vec4 FragColor;

void main()
{
    FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);
}
"""

def main():
    if not glfw.init():
        print('Error: glfw could not be initialized', file=sys.stderr)
        return
    glfw.window_hint(glfw.CONTEXT_VERSION_MAJOR, 3)
    glfw.window_hint(glfw.CONTEXT_VERSION_MINOR, 3)
    if sys.platform == 'darwin':
        glfw.window_hint(glfw.OPENGL_FORWARD_COMPAT, glfw.TRUE)

    window = glfw.create_window(800, 600, 'LearnOpenGL', None, None)
    if not window:
        print('Error: glfw window could not be created', file=sys.stderr)
        glfw.terminate()
        return
    glfw.make_context_current(window)

    # First two arguments set the location of the lower left corner of the window.
    # glViewport(0,0,800,600)

    glfw.set_framebuffer_size_callback(window, framebuffer_size_callback)

    vertex_shader = glCreateShader(GL_VERTEX_SHADER)
    glShaderSource(vertex_shader, VERTEX_SHADER_SOURCE)
    glCompileShader(vertex_shader)
    if not glGetShaderiv(vertex_shader, GL_COMPILE_STATUS):
        print('Error: vertex shader compilation failed:', file=sys.stderr)
        print(glGetShaderInfoLog(vertex_shader).decode(), file=sys.stderr)
        return

    fragment_shader = glCreateShader(GL_FRAGMENT_SHADER)
    glShaderSource(fragment_shader, FRAGMENT_SHADER_SOURCE)
    glCompileShader(fragment_shader)
    if not glGetShaderiv(fragment_shader, GL_COMPILE_STATUS):
        print('Error: vertex shader compilation failed:', file=sys.stderr)
        print(glGetShaderInfoLog(fragment_shader).decode(), file=sys.stderr)
        return

    shader_program = glCreateProgram()
    glAttachShader(shader_program, vertex_shader)
    glAttachShader(shader_program, fragment_shader)
    glLinkProgram(shader_program)
    if not glGetProgramiv(shader_program, GL_LINK_STATUS):
        print('Error: shader program linking failed:', file=sys.stderr)
        print(glGetProgramInfoLog(shader_program).decode(), file=sys.stderr)
        return

    glDeleteShader(vertex_shader)
    glDeleteShader(fragment_shader)

    vertices = glm.array(glm.float32,
        -0.5, -0.5, 0.0,
         0.5, -0.5, 0.0,
         0.0,  0.5, 0.0
    )
    vbo = glGenBuffers(1)
    vao = glGenVertexArrays(1)

    glBindVertexArray(vao)

    glBindBuffer(GL_ARRAY_BUFFER, vbo)
    glBufferData(GL_ARRAY_BUFFER, vertices.nbytes, vertices.ptr, GL_STATIC_DRAW)

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * glm.sizeof(glm.float32), None)
    glEnableVertexAttribArray(0)

    # Unbind.
    glBindBuffer(GL_ARRAY_BUFFER, 0)
    glBindVertexArray(0)

    while not glfw.window_should_close(window):
        process_input(window)

        glClearColor(0.2, 0.3, 0.3, 1.0)
        glClear(GL_COLOR_BUFFER_BIT)

        glUseProgram(shader_program)
        glBindVertexArray(vao)
        glDrawArrays(GL_TRIANGLES, 0, 3)

        # Swap front and back buffers
        glfw.swap_buffers(window)

        # Poll for and process events
        glfw.poll_events()

    glfw.terminate()


if __name__ == "__main__":
    main()