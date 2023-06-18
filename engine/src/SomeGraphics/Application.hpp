#pragma once

#include <string>

#include "SomeGraphics/Window.hpp"
#include "SomeGraphics/Rendering/Renderer.hpp"

namespace sg {

class Application {
public:
    Application() = delete;
    Application(const std::string& name);
    Application(Application&& other) = default;
    Application(const Application& other) = delete;
    Application& operator=(Application&& other) = default;
    Application& operator=(const Application& other) = delete;
    virtual ~Application();
    void run();
    virtual void on_update(float delta_time);
    virtual void on_render();
protected:
    std::string m_name;
    Window m_window;
    Renderer m_renderer;
};

}
