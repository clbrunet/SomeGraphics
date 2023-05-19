#pragma once

#include <string>

#include "SomeGraphics/Window.hpp"
#include "SomeGraphics/Renderer.hpp"

namespace sg {

class Application {
public:
    Application(const std::string& name);
    virtual ~Application();
    void run();
    virtual void on_update();
    virtual void on_render();
protected:
    std::string m_name;
    Window m_window;
    Renderer m_renderer;
};

}
