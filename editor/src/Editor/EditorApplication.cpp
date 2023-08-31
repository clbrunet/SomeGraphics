#include <memory>

#include "imgui.h"

#include "Editor/EditorApplication.hpp"
#include "Editor/Viewport.hpp"
#include "Editor/Outliner.hpp"
#include "Editor/Properties.hpp"

std::unique_ptr<sg::Application> create_app()
{
    return std::make_unique<sg::EditorApplication>("SomeGraphics Editor");
}

namespace sg {

EditorApplication::EditorApplication(const std::string& name) :
    Application(name, 1280, 720),
    m_viewport(std::make_unique<Viewport>(*m_renderer))
{
    std::optional<std::shared_ptr<SceneEntity>> scene_entity_opt
        = SceneEntity::load_model("editor/assets/models/rusted_iron_sphere.glb");
    if (!scene_entity_opt.has_value()) {
        abort();
    }
    m_selected_entity = scene_entity_opt.value();
    m_scene->add_entity(std::move(scene_entity_opt.value()));
    ImGuiIO& io = ImGui::GetIO();
    io.IniFilename = "editor/assets/imgui.ini";
    io.LogFilename = "editor/assets/imgui_log.txt";
}

void EditorApplication::on_update(float delta_time)
{
    m_viewport->on_update(*m_window, delta_time);
}

void EditorApplication::on_render()
{
    ImGui::DockSpaceOverViewport();
    m_viewport->on_render(*m_renderer, *m_scene);
    m_outliner->on_render(*m_scene, m_selected_entity);
    if (m_selected_entity.expired()) {
        m_properties->on_render();
    } else {
        m_properties->on_render(*m_selected_entity.lock());
    }
}

}
