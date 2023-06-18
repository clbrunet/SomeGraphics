#include <memory>

#include "Editor/EditorApplication.hpp"
#include "Editor/Viewport.hpp"
#include "imgui.h"

std::unique_ptr<sg::Application> create_app()
{
    return std::make_unique<sg::EditorApplication>("SomeGraphics Editor");
}

namespace sg {

EditorApplication::EditorApplication(const std::string& name) :
    Application(name),
    m_viewport(std::make_unique<Viewport>(*m_renderer))
{
}

EditorApplication::~EditorApplication()
{
}

void EditorApplication::on_update(float delta_time)
{
    m_viewport->on_update(*m_window, delta_time);
}

void EditorApplication::on_render()
{
    ImGui::DockSpaceOverViewport();
    m_viewport->on_render(*m_renderer);
}

}
