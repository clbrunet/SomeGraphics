#pragma once

#include <cstdint>

#include <GLFW/glfw3.h>
#include <glm/ext/vector_float2.hpp>
#include <glm/ext/vector_int2.hpp>

namespace sg {

class Window {
public:
    Window() = delete;
    Window(const char* title, uint16_t width, uint16_t height);
    Window(Window&& other);
    Window(const Window& other) = delete;
    Window& operator=(Window&& other) = delete;
    Window& operator=(const Window& other) = delete;
    ~Window();

    bool should_close() const;
    void poll_event();
    void begin_frame() const;
    void end_frame() const;
    void swap_buffers() const;

    glm::ivec2 dimensions() const;

    void reset_time();
    float time() const;
    float delta_time() const;

    bool is_key_pressed(int key) const;

    bool is_mouse_button_pressed(int button) const;

    glm::vec2 cursor_position() const;
    void set_cursor_visibility(bool state) const;
    glm::vec2 cursor_delta() const;
private:
    GLFWwindow* m_window = nullptr;
    float m_last_time = 0.0f;
    float m_delta_time = 0.0f;
    glm::vec2 m_last_cursor_position = glm::vec2(0.0f);
    glm::vec2 m_cursor_delta = glm::vec2(0.0f);
    static bool is_instantiated;

    static void glfw_error_callback(int error, const char* description);
};

}
