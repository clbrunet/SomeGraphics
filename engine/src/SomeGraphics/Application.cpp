#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "SomeGraphics/Application.hpp"

namespace sg {

Application::Application(const std::string& name) :
    m_name(name),
    m_window(name.c_str(), 800, 450)
{
}

Application::~Application()
{
}

void Application::run()
{
    m_window.reset_time();
    float time = m_window.get_time();
    while (!m_window.should_close())
    {
        m_window.poll_event();
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        float new_time = m_window.get_time();
        float delta_time = new_time - time;
        time = new_time;
        on_update(delta_time);
        on_render();
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            GLFWwindow* backup_current_context = glfwGetCurrentContext();
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
            glfwMakeContextCurrent(backup_current_context);
        }
        m_window.swap_buffers();
    }
}

void Application::on_update(float delta_time)
{
    static_cast<void>(delta_time);
}

void Application::on_render()
{
}

}
