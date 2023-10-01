#include <cstdint>
#include <iostream>

#include "SomeGraphics/Application.hpp"

namespace sg {

Application::Application(std::string name, uint16_t width, uint16_t height) :
    m_name(std::move(name)),
    m_window(std::make_unique<Window>(m_name.c_str(), width, height))
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
    while (!m_window->should_close())
    {
        m_window->poll_event();
        m_window->begin_frame();
        on_update();
        on_render();
        m_window->end_frame();
        m_window->swap_buffers();
    }
}

void Application::on_update()
{
}

void Application::on_render()
{
}

bool Application::is_instantiated = false;

}
