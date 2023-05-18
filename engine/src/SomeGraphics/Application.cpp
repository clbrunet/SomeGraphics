#include "SomeGraphics/Application.hpp"

namespace sg {

Application::Application(const std::string& name) :
    m_name(name),
    m_window(name.c_str(), 800, 450)
{
}

Application::~Application()
{
}

void Application::run()
{
    while (!m_window.should_close())
    {
        m_window.poll_event();
        on_update();
        on_render();
        m_window.swap_buffers();
    }
}

void Application::on_update()
{
}

void Application::on_render()
{
}

}
