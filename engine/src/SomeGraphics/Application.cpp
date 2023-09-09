#include <cstdint>

#include "SomeGraphics/Application.hpp"

namespace sg {

Application::Application(const std::string& name, uint16_t width, uint16_t height) :
    m_name(name),
    m_window(std::make_unique<Window>(name.c_str(), width, height))
{
    assert(!is_instantiated && "Multiple instances are not allowed");
    is_instantiated = true;
}

Application::~Application()
{
    is_instantiated = false;
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

void Application::on_update([[maybe_unused]] float delta_time)
{
}

void Application::on_render()
{
}

bool Application::is_instantiated = false;

}
