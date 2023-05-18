#include <cstdlib>

#include "SomeGraphics/Application.hpp"

namespace sg {

Application::Application(const std::string& name) :
    m_name(name)
{
    if (!glfwInit()) {
        std::abort();
    }
    m_window = glfwCreateWindow(800, 450, name.c_str(), NULL, NULL);
    if (m_window == nullptr) {
        std::abort();
    }
    glfwMakeContextCurrent(m_window);
}

Application::~Application()
{
    glfwDestroyWindow(m_window);
    glfwTerminate();
}

void Application::run()
{
    while (!glfwWindowShouldClose(m_window))
    {
        glfwPollEvents();
        glfwSwapBuffers(m_window);
    }
}

}
