import sys

import glfw

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

def main():
    if not glfw.init():
        print('Error: glfw could not be initialized', sys.stderr)
        return
    glfw.window_hint(glfw.CONTEXT_VERSION_MAJOR, 3)
    glfw.window_hint(glfw.CONTEXT_VERSION_MINOR, 3)
    if sys.platform == 'darwin':
        glfw.window_hint(glfw.OPENGL_FORWARD_COMPAT, glfw.TRUE)

    window = glfw.create_window(800, 600, 'LearnOpenGL', None, None)
    if not window:
        print('Error: glfw window could not be created', sys.stderr)
        glfw.terminate()
        return
    glfw.make_context_current(window)

    # First two arguments set the location of the lower left corner of the window.
    glViewport(0,0,800,600)

    glfw.set_framebuffer_size_callback(window, framebuffer_size_callback)

    while not glfw.window_should_close(window):
        process_input(window)

        glClearColor(0.2, 0.3, 0.3, 1.0)
        glClear(GL_COLOR_BUFFER_BIT)

        # Render here, e.g. using pyOpenGL

        # Swap front and back buffers
        glfw.swap_buffers(window)

        # Poll for and process events
        glfw.poll_events()

    glfw.terminate()


if __name__ == "__main__":
    main()