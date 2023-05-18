#pragma once

#include <string>

#include "GLFW/glfw3.h"

namespace sg {

class Application {
public:
    Application(const std::string& name);
    virtual ~Application();
    void run();
protected:
    std::string m_name;
    GLFWwindow* m_window;
};

}
