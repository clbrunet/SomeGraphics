#include <iostream>
#include <memory>
#include <stack>

#include "SomeGraphics/SceneEntity.hpp"
#include "glm/ext/matrix_float4x4.hpp"
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
}

void Viewport::on_update(const Window& window, float delta_time)
{
    if (m_is_hovered) {
        m_editor_camera->on_update(window, delta_time);
    }
}

void Viewport::on_render(const Renderer& renderer, const Scene& scene)
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
    render_scene(renderer, scene);
    renderer.draw(*m_skybox, *m_editor_camera);
    FrameBuffer::bind_default();

    ImGui::Image((void*)(std::intptr_t)m_frame_buffer->color_texture().renderer_id(),
        m_dimension, ImVec2(0.0f, 1.0f), ImVec2(1.0f, 0.0f));

    ImGui::End();
    ImGui::PopStyleVar();
    ImGui::PopStyleVar();
}

void Viewport::render_scene(const Renderer& renderer, const Scene& scene) const
{
    m_program->use();
    m_program->set_mat4("u_view_projection", m_editor_camera->view_projection());
    m_program->set_vec3("u_camera_position", m_editor_camera->position());
    m_program->set_int("u_environment", 0);
    m_skybox->texture().bind_to_unit(0);
    std::stack<std::vector<std::shared_ptr<SceneEntity>>::const_iterator>
        iterators_stack({ scene.root_entity()->children().cbegin() });
    std::stack<std::vector<std::shared_ptr<SceneEntity>>::const_iterator>
        ends_stack({ scene.root_entity()->children().cend() });
    std::stack<glm::mat4> model_matrices_stack({ scene.root_entity()->transform().local() });
    while (!iterators_stack.empty()) {
        const std::shared_ptr<SceneEntity>& entity = *iterators_stack.top();
        ++iterators_stack.top();
        glm::mat4 model_matrix = model_matrices_stack.top() * entity->transform().local();
        if (entity->mesh()) {
            m_program->set_mat4("u_model", model_matrix);
            m_program->set_int("u_texture", 1);
            entity->texture()->bind_to_unit(1);
            m_program->set_vec3("u_color", entity->color());
            renderer.draw(*entity->mesh());
        }
        if (entity->children().size() > 0) {
            iterators_stack.push(entity->children().cbegin());
            ends_stack.push(entity->children().cend());
            model_matrices_stack.push(model_matrix);
        }
        while (!iterators_stack.empty() && iterators_stack.top() == ends_stack.top()) {
            iterators_stack.pop();
            ends_stack.pop();
            model_matrices_stack.pop();
        }
    }
}

}
