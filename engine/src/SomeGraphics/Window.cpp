#include <cstdlib>

#include "GLFW/glfw3.h"
#include "SomeGraphics/Window.hpp"

namespace sg {

Window::Window(const char* title, uint16_t width, uint16_t height)
{
    if (windows_count == 0) {
        if (!glfwInit()) {
            std::abort();
        }
    }
    m_window = glfwCreateWindow(width, height, title, NULL, NULL);
    if (m_window == nullptr) {
        std::abort();
    }
    windows_count++;
    glfwMakeContextCurrent(m_window);
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
