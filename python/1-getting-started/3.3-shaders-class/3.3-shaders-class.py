import ctypes
import os
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

class Shader:
    id: int
    """ID of the shader program."""

    def __init__(self, vertex_path: str, fragment_path: str):
        current_dir = os.path.dirname(os.path.abspath(__file__))
        with open(os.path.join(current_dir, vertex_path)) as f:
            vertex_code = f.read()
        with open(os.path.join(current_dir, fragment_path)) as f:
            fragment_code = f.read()    

        vertex_shader = glCreateShader(GL_VERTEX_SHADER)
        glShaderSource(vertex_shader, vertex_code)
        glCompileShader(vertex_shader)
        if not glGetShaderiv(vertex_shader, GL_COMPILE_STATUS):
            print('Error: vertex shader compilation failed:', file=sys.stderr)
            print(glGetShaderInfoLog(vertex_shader).decode(), file=sys.stderr)
            return

        fragment_shader = glCreateShader(GL_FRAGMENT_SHADER)
        glShaderSource(fragment_shader, fragment_code)
        glCompileShader(fragment_shader)
        if not glGetShaderiv(fragment_shader, GL_COMPILE_STATUS):
            print('Error: vertex shader compilation failed:', file=sys.stderr)
            print(glGetShaderInfoLog(fragment_shader).decode(), file=sys.stderr)
            return

        self.id = glCreateProgram()
        glAttachShader(self.id, vertex_shader)
        glAttachShader(self.id, fragment_shader)
        glLinkProgram(self.id)
        if not glGetProgramiv(self.id, GL_LINK_STATUS):
            print('Error: shader program linking failed:', file=sys.stderr)
            print(glGetProgramInfoLog(self.id).decode(), file=sys.stderr)
            return

        glDeleteShader(vertex_shader)
        glDeleteShader(fragment_shader)
        

    def use(self):
        glUseProgram(self.id)

    def set_bool(self, name: str, value: bool):
        glUniform1i(glGetUniformLocation(self.id, name), int(value))

    def set_int(self, name: str, value: int):
        glUniform1i(glGetUniformLocation(self.id, name), value)

    def set_float(self, name: str, value: float):
        glUniform1f(glGetUniformLocation(self.id, name), value)


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

    shader = Shader('shader.vs', 'shader.fs')

    vertices = glm.array(glm.float32,
        # positions       # colors
         0.5, -0.5, 0.0,  1.0, 0.0, 0.0,   # bottom right
        -0.5, -0.5, 0.0,  0.0, 1.0, 0.0,   # bottom left
         0.0,  0.5, 0.0,  0.0, 0.0, 1.0    # top
    )
    vbo = glGenBuffers(1)
    vao = glGenVertexArrays(1)

    glBindVertexArray(vao)

    glBindBuffer(GL_ARRAY_BUFFER, vbo)
    glBufferData(GL_ARRAY_BUFFER, vertices.nbytes, vertices.ptr, GL_STATIC_DRAW)

    # Position.
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * glm.sizeof(glm.float32), None)
    glEnableVertexAttribArray(0)
    # Color.
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * glm.sizeof(glm.float32), ctypes.c_void_p(3 * glm.sizeof(glm.float32)))
    glEnableVertexAttribArray(1)

    # Unbind.
    glBindBuffer(GL_ARRAY_BUFFER, 0)
    glBindVertexArray(0)

    while not glfw.window_should_close(window):
        process_input(window)

        glClearColor(0.2, 0.3, 0.3, 1.0)
        glClear(GL_COLOR_BUFFER_BIT)

        shader.use()

        # time_value = glfw.get_time()
        # green_value = (glm.sin(time_value) / 2.0) + 0.5
        # vertex_color_location = glGetUniformLocation(shader_program, 'ourColor')
        # glUniform4f(vertex_color_location, 0, green_value, 0, 1)

        glBindVertexArray(vao)
        glDrawArrays(GL_TRIANGLES, 0, 3)

        # Swap front and back buffers
        glfw.swap_buffers(window)

        # Poll for and process events
        glfw.poll_events()

    glfw.terminate()


if __name__ == "__main__":
    main()