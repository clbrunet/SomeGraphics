#include "SomeGraphics/Application.hpp"

namespace sg {

Application::Application(const std::string& name) :
    m_name(name),
    m_window(std::make_unique<Window>(name.c_str(), 800, 450))
{
    assert(applications_count == 0 && "Multiple applications are not supported");
    applications_count++;
}

Application::~Application()
{
    applications_count--;
}

void Application::run()
{
    m_window->reset_time();
    float time = m_window->get_time();
    while (!m_window->should_close())
    {
        m_window->poll_event();
        m_window->begin_frame();
        float new_time = m_window->get_time();
        float delta_time = new_time - time;
        time = new_time;
        on_update(delta_time);
        on_render();
        m_window->end_frame();
        m_window->swap_buffers();
    }
}

void Application::on_update(float delta_time)
{
    static_cast<void>(delta_time);
}

void Application::on_render()
{
}

uint8_t Application::applications_count = 0;

}
