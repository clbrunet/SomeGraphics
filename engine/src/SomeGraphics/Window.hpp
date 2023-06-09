#pragma once

#include <cstdint>

#include "GLFW/glfw3.h"
#include "glm/ext/vector_float2.hpp"

namespace sg {

class Window {
public:
    Window(const char* title, uint16_t width, uint16_t height);
    ~Window();

    bool should_close() const;
    void poll_event() const;
    void swap_buffers() const;

    void reset_time() const;
    float get_time() const;
    bool is_key_pressed(int key) const;
    bool is_mouse_button_pressed(int button) const;
    glm::vec2 get_cursor_position() const;
private:
    GLFWwindow* m_window = nullptr;
    static uint8_t windows_count;
};

}
