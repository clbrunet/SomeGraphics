#include <deque>
#include <iostream>
#include <memory>
#include <vector>

#include "glm/ext/vector_int2.hpp"
#include "imgui.h"

#include "Editor/Viewport.hpp"
#include "Editor/EditorCamera.hpp"
#include "SomeGraphics.hpp"

namespace sg {

Viewport::Viewport(const Renderer& renderer)
{
    renderer.set_viewport(glm::ivec2(m_dimension.x, m_dimension.y));
    renderer.set_clear_color(1.0f, 0.0f, 1.0f, 1.0f);
    std::optional<std::unique_ptr<Skybox>> skybox_opt
        = Skybox::create("editor/assets/textures/skybox/right.jpg",
            "editor/assets/textures/skybox/left.jpg", "editor/assets/textures/skybox/top.jpg",
            "editor/assets/textures/skybox/bottom.jpg", "editor/assets/textures/skybox/front.jpg",
            "editor/assets/textures/skybox/back.jpg");
    if (!skybox_opt.has_value()) {
        abort();
    }
    m_skybox = std::move(skybox_opt.value());
    std::optional<std::unique_ptr<Program>> program_opt
        = Program::create("editor/assets/shaders/environment_reflection.vert",
            "editor/assets/shaders/environment_reflection.frag");
    if (!program_opt.has_value()) {
        abort();
    }
    m_program = std::move(program_opt.value());
    std::optional<std::shared_ptr<SceneEntity>> scene_entity_opt
        = SceneEntity::load_model("editor/assets/models/survival_guitar_backpack.glb");
    if (!scene_entity_opt.has_value()) {
        abort();
    }
    m_scene->add_entity(scene_entity_opt.value());
}

Viewport::~Viewport() {
}

void Viewport::on_update(const Window& window, float delta_time)
{
    if (m_is_hovered) {
        m_editor_camera->on_update(window, delta_time);
    }
}

void Viewport::on_render(const Renderer& renderer)
{
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    ImGui::PushStyleVar(ImGuiStyleVar_ChildBorderSize, 0.0f);
    ImGui::Begin("Viewport");
    m_is_hovered = ImGui::IsWindowHovered();
    ImVec2 content_region_available = ImGui::GetContentRegionAvail();
    if (content_region_available.x != m_dimension.x
        || content_region_available.y != m_dimension.y) {
        m_dimension = content_region_available;
        renderer.set_viewport(glm::ivec2(m_dimension.x, m_dimension.y));
        m_frame_buffer = std::make_unique<FrameBuffer>(glm::vec2(m_dimension.x, m_dimension.y));
        m_editor_camera->set_projection(glm::perspective(glm::radians(60.0f),
                m_dimension.x / m_dimension.y, 0.01f, 1000.0f));
    }

    m_frame_buffer->bind();
    renderer.clear();

    m_program->use();
    m_program->set_mat4("u_view_projection", m_editor_camera->view_projection());
    m_program->set_vec3("u_camera_position", m_editor_camera->position());
    m_program->set_int("u_environment", 0);
    m_skybox->texture().bind_to_unit(0);
    std::deque<std::shared_ptr<SceneEntity>> entities = std::deque(m_scene->entities().begin(),
        m_scene->entities().end());
    while (!entities.empty()) {
        const SceneEntity& entity = *entities.front();
        entities.pop_front();
        for (const std::shared_ptr<SceneEntity>& child : entity.children()) {
            entities.push_front(child);
        }
        if (entity.mesh()) {
            m_program->set_mat4("u_model", entity.transform());
            m_program->set_int("u_texture", 1);
            entity.texture()->bind_to_unit(1);
            m_program->set_vec3("u_color", entity.color());
            renderer.draw(*entity.mesh());
        }
    }

    renderer.draw(*m_skybox, *m_editor_camera);

    FrameBuffer::bind_default();

    ImGui::Image((void*)(std::intptr_t)m_frame_buffer->color_texture().renderer_id(),
        m_dimension, ImVec2(0.0f, 1.0f), ImVec2(1.0f, 0.0f));

    ImGui::End();
    ImGui::PopStyleVar();
    ImGui::PopStyleVar();
}

}
