#pragma once

#include <memory>
#include <string>

#include "SomeGraphics/Window.hpp"
#include "SomeGraphics/Rendering/Renderer.hpp"

namespace sg {

class Application {
public:
    Application() = delete;
    Application(std::string name, uint16_t width, uint16_t height);
    Application(Application&& other) = default;
    Application(const Application& other) = delete;
    Application& operator=(Application&& other) = delete;
    Application& operator=(const Application& other) = delete;
    virtual ~Application();

    void run();
    virtual void update();
    virtual void render();
protected:
    std::string m_name;
    std::unique_ptr<Window> m_window;
    std::unique_ptr<Renderer> m_renderer = std::make_unique<Renderer>();
    static bool is_instantiated;
};

}
