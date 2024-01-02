#include <cassert>
#include <cstdlib>
#include <iostream>
#include <memory>
#include <filesystem>

#include <imgui.h>

#include "Editor/EditorApplication.hpp"
#include "Editor/Viewport.hpp"
#include "Editor/Outliner.hpp"
#include "Editor/Properties.hpp"

std::unique_ptr<sg::Application> create_app()
{
    return std::make_unique<sg::EditorApplication>("SomeGraphics Editor");
}

namespace sg {

EditorApplication::EditorApplication(std::string name) :
    Application(std::move(name), 1280, 720),
    m_viewport(std::make_unique<Viewport>(*m_renderer))
{
    std::shared_ptr<Entity> entity
        = Entity::load_model("editor/assets/models/rusted_iron_sphere.glb",
            m_scene->root()).value();
    entity->set_local_position(glm::vec3(5.0f, 5.0f, -5.0f));
    m_selection = std::move(entity);
    Entity::load_model("editor/assets/models/axes.glb", m_scene->root());
    Entity::load_model("editor/assets/models/cube.glb", m_scene->root());
    entity = Entity::load_model("editor/assets/models/cerberus.glb", m_scene->root()).value();
    entity->set_local_position(glm::vec3(-5.0f, -3.0f, 1.0f));
    entity->set_local_scale(glm::vec3(0.03f));
    entity = Entity::load_model("editor/assets/models/animated.glb", m_scene->root()).value();
    entity->set_local_position(glm::vec3(-4.0f, -7.0f, -4.0f));
    entity->set_local_rotation(glm::radians(glm::vec3(0.0f, 45.0f, 0.0f)));
    entity->set_local_scale(glm::vec3(3.0f));
    m_scene->add_light("Light1", m_scene->root())->light()->intensity = 200.0f;
    std::error_code ec;
    std::filesystem::copy_file("editor/assets/default_imgui.ini", "editor/assets/imgui.ini",
        std::filesystem::copy_options::skip_existing, ec);
    ImGuiIO& io = ImGui::GetIO();
    io.IniFilename = "editor/assets/imgui.ini";
    io.LogFilename = "editor/assets/imgui_log.txt";
}

void EditorApplication::on_update()
{
    m_viewport->on_update(*m_window);
}

void EditorApplication::on_render()
{
    ImGui::DockSpaceOverViewport();
    m_viewport->on_render(*m_renderer, *m_scene);
    m_outliner->on_render(*m_scene, m_selection);
    m_properties->on_render(*m_renderer, m_selection);
}

}
