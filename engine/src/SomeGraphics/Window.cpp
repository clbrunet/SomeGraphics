#include <cstdlib>

#include <glad/gl.h>

#include "SomeGraphics/Window.hpp"

namespace sg {

Window::Window(const char* title, uint16_t width, uint16_t height)
{
    if (windows_count == 0) {
        if (!glfwInit()) {
            std::abort();
        }
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);
    m_window = glfwCreateWindow(width, height, title, NULL, NULL);
    if (m_window == nullptr) {
        std::abort();
    }
    glfwMakeContextCurrent(m_window);
    if (windows_count == 0) {
        gladLoadGL(glfwGetProcAddress);
    }
    windows_count++;
}

Window::~Window()
{
    glfwDestroyWindow(m_window);
    windows_count--;
    if (windows_count == 0) {
        glfwTerminate();
    }
}

bool Window::should_close() const
{
    return glfwWindowShouldClose(m_window);
}

void Window::poll_event() const
{
    glfwPollEvents();
}

void Window::swap_buffers() const
{
    glfwSwapBuffers(m_window);
}

uint8_t Window::windows_count = 0;

}
