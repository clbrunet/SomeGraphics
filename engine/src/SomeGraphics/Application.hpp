#pragma once

#include <string>

#include "GLFW/glfw3.h"

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
    GLFWwindow* m_window;
};

}
