#include <cassert>
#include <cstdlib>
#include <algorithm>
#include <iostream>

#include <GLFW/glfw3.h>
#include <glad/gl.h>
#include <glm/ext/vector_float2.hpp>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include "SomeGraphics/Window.hpp"

namespace sg {

Window::Window(const char* title, uint16_t width, uint16_t height)
{
    assert(!is_instantiated && "Multiple instances are not allowed");
    is_instantiated = true;
    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit()) {
        std::cerr << "Window creation failed\n";
        std::abort();
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#if SG_DEBUG
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);
#endif
    m_window = glfwCreateWindow(width, height, title, nullptr, nullptr);
    if (m_window == nullptr) {
        std::cerr << "Window creation failed\n";
        std::abort();
    }
    glfwMakeContextCurrent(m_window);
    gladLoadGL(glfwGetProcAddress);
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
    ImGui_ImplGlfw_SetCallbacksChainForAllWindows(true);

    if (glfwRawMouseMotionSupported()) {
        glfwSetInputMode(m_window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
    }
}

Window::Window(Window&& other) :
    m_window(other.m_window)
{
    other.m_window = nullptr;
}

Window::~Window()
{
    if (m_window == nullptr) {
        return;
    }
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwDestroyWindow(m_window);
    glfwTerminate();
    is_instantiated = false;
}

bool Window::should_close() const
{
    return glfwWindowShouldClose(m_window);
}

void Window::poll_event()
{
    glfwPollEvents();
    float time = this->time();
    m_delta_time = time - m_last_time;
    m_last_time = time;
    glm::vec2 cursor_position = this->cursor_position();
    m_cursor_delta = cursor_position - m_last_cursor_position;
    m_last_cursor_position = cursor_position;
}

void Window::begin_frame() const
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

void Window::end_frame() const
{
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        GLFWwindow* backup_current_context = glfwGetCurrentContext();
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
        glfwMakeContextCurrent(backup_current_context);
    }
}

void Window::swap_buffers() const
{
    glfwSwapBuffers(m_window);
}

glm::ivec2 Window::dimensions() const
{
    int width, height;
    glfwGetWindowSize(m_window, &width, &height);
    return glm::ivec2(width, height);
}

void Window::reset_time()
{
    m_last_time = 0.0f;
    glfwSetTime(0.0);
}

float Window::time() const
{
    return (float)glfwGetTime();
}

float Window::delta_time() const
{
    return m_delta_time;
}

bool Window::is_key_pressed(int key) const
{
    const ImVector<ImGuiViewport*>& viewports = ImGui::GetPlatformIO().Viewports;
    return std::any_of(viewports.begin(), viewports.end(), [key](ImGuiViewport* viewport) {
        return glfwGetKey(static_cast<GLFWwindow*>(viewport->PlatformHandle), key) == GLFW_PRESS;
    });
}

bool Window::is_mouse_button_pressed(int button) const
{
    const ImVector<ImGuiViewport*>& viewports = ImGui::GetPlatformIO().Viewports;
    return std::any_of(viewports.begin(), viewports.end(), [button](ImGuiViewport* viewport) {
        return glfwGetMouseButton(static_cast<GLFWwindow*>(viewport->PlatformHandle), button)
            == GLFW_PRESS;
    });
}

glm::vec2 Window::cursor_position() const
{
    double xpos, ypos;
    glfwGetCursorPos(m_window, &xpos, &ypos);
    return glm::vec2(xpos, ypos);
}

void Window::set_cursor_visibility(bool state) const
{
    if (state) {
        glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    } else {
        glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    }
}

glm::vec2 Window::cursor_delta() const
{
    return m_cursor_delta;
}

bool Window::is_instantiated = false;

void Window::glfw_error_callback(int error, const char* description)
{
    std::cerr << "GLFW error " << error << ": " << description << '\n';
}

}
