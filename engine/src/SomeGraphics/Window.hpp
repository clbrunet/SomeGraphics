#pragma once

#include <cstdint>

#include "GLFW/glfw3.h"
#include "glm/ext/vector_float2.hpp"
#include "glm/ext/vector_int2.hpp"

namespace sg {

class Window {
public:
    Window() = delete;
    Window(const char* title, uint16_t width, uint16_t height);
    Window(Window&& other);
    Window(const Window& other) = delete;
    Window& operator=(Window&& other);
    Window& operator=(const Window& other) = delete;
    ~Window();

    bool should_close() const;
    void poll_event() const;
    void begin_frame() const;
    void end_frame() const;
    void swap_buffers() const;

    glm::ivec2 get_dimension() const;
    void reset_time() const;
    float get_time() const;
    bool is_key_pressed(int key) const;
    bool is_mouse_button_pressed(int button) const;
    glm::vec2 get_cursor_position() const;
private:
    GLFWwindow* m_window = nullptr;
    static uint8_t windows_count;

    static void glfw_error_callback(int error, const char* description);
};

}
