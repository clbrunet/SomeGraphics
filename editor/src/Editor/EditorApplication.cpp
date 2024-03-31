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
    Node* sphere = m_scene->load_model("editor/assets/models/rusted_iron_sphere.glb",
            m_scene->root().entity());
    sphere->set_local_position(glm::vec3(-5.0f, 3.0f, -5.0f));
    m_selection = sphere->handle();
    m_scene->load_model("editor/assets/models/axes.glb", m_scene->root().entity());
    m_scene->load_model("editor/assets/models/cube.glb", m_scene->root().entity());
    Node* cerberus = m_scene->load_model("editor/assets/models/cerberus.glb", m_scene->root().entity());
    cerberus->set_local_position(glm::vec3(-5.0f, -3.0f, 1.0f));
    cerberus->set_local_scale(glm::vec3(0.03f));
    Node* animated = m_scene->load_model("editor/assets/models/animated.glb", m_scene->root().entity());
    animated->set_local_position(glm::vec3(5.0f, -7.0f, -4.0f));
    animated->set_local_scale(glm::vec3(3.0f));
    Node& light = m_scene->create_node("Light", m_scene->root().entity());
    light.handle().emplace<Light>(glm::vec3(1.0f), 200.0f);

    std::error_code ec;
    std::filesystem::copy_file("editor/assets/default_imgui.ini", "editor/assets/imgui.ini",
        std::filesystem::copy_options::skip_existing, ec);
    ImGuiIO& io = ImGui::GetIO();
    io.IniFilename = "editor/assets/imgui.ini";
    io.LogFilename = "editor/assets/imgui_log.txt";
}

void EditorApplication::update()
{
    m_scene->update(*m_window);
    m_viewport->update(*m_window);
}

void EditorApplication::render()
{
    ImGui::DockSpaceOverViewport();
    m_viewport->render(*m_renderer, *m_scene);
    m_outliner->render(*m_scene, m_selection);
    m_properties->render(*m_renderer, m_selection);
}

}
