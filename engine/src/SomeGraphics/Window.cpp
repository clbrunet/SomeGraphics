#include <cstdlib>

#include "GLFW/glfw3.h"
#include "glad/gl.h"
#include "glm/ext/vector_float2.hpp"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "SomeGraphics/Window.hpp"

namespace sg {

Window::Window(const char* title, uint16_t width, uint16_t height)
{
    if (windows_count == 0) {
        if (!glfwInit()) {
            std::abort();
        }
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#if SG_DEBUG
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);
#endif
    m_window = glfwCreateWindow(width, height, title, NULL, NULL);
    if (m_window == nullptr) {
        std::abort();
    }
    glfwMakeContextCurrent(m_window);
    if (windows_count == 0) {
        gladLoadGL(glfwGetProcAddress);
    }
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
    io.ConfigViewportsNoAutoMerge = true;
    ImGui::StyleColorsDark();
    ImGui::GetStyle().Colors[ImGuiCol_WindowBg].w = 1.0f;
    ImGui_ImplGlfw_InitForOpenGL(m_window, true);
    ImGui_ImplOpenGL3_Init("#version 450");
    windows_count++;
}

Window::~Window()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwDestroyWindow(m_window);
    if (windows_count == 1) {
        glfwTerminate();
    }
    windows_count--;
}

bool Window::should_close() const
{
    return glfwWindowShouldClose(m_window);
}

void Window::poll_event() const
{
    glfwPollEvents();
}

void Window::swap_buffers() const
{
    glfwSwapBuffers(m_window);
}

glm::ivec2 Window::get_dimension() const
{
    int width, height;
    glfwGetWindowSize(m_window, &width, &height);
    return glm::ivec2(width, height);
}

void Window::reset_time() const
{
    return glfwSetTime(0.0);
}

float Window::get_time() const
{
    return glfwGetTime();
}

bool Window::is_key_pressed(int key) const
{
    return glfwGetKey(m_window, key) == GLFW_PRESS;
}

bool Window::is_mouse_button_pressed(int button) const
{
    return glfwGetMouseButton(m_window, button) == GLFW_PRESS;
}

glm::vec2 Window::get_cursor_position() const
{
    double xpos, ypos;
    glfwGetCursorPos(m_window, &xpos, &ypos);
    return glm::vec2(xpos, ypos);
}

uint8_t Window::windows_count = 0;

}
