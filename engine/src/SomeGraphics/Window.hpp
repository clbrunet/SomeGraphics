#pragma once

#include <cstdint>

#include <GLFW/glfw3.h>

namespace sg {

class Window {
public:
    Window(const char* title, uint16_t width, uint16_t height);
    ~Window();
    bool should_close() const;
    void poll_event() const;
    void swap_buffers() const;
private:
    GLFWwindow* m_window = nullptr;
    static uint8_t windows_count;
};

}
