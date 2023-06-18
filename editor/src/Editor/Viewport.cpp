#include "glm/ext/vector_int2.hpp"
#include "imgui.h"

#include "Editor/Viewport.hpp"
#include "Editor/EditorCamera.hpp"
#include "SomeGraphics.hpp"

namespace sg {

Viewport::Viewport(const Renderer& renderer)
{
    renderer.set_viewport(glm::ivec2(m_dimension.x, m_dimension.y));
    renderer.set_clear_color(0.0f, 0.5f, 0.0f, 1.0f);
    m_frame_buffer = std::make_unique<FrameBuffer>(glm::vec2(m_dimension.x, m_dimension.y));
    m_camera = std::make_unique<EditorCamera>(glm::vec3(2.0f, 2.0f, 2.0f), glm::vec2(-35.0f, 45.0f),
        glm::perspective(glm::radians(60.0f), m_dimension.x / m_dimension.y, 0.01f, 1000.0f));
    std::optional<std::unique_ptr<Program>> program_optional
        = Program::create("editor/assets/shaders/flat.vert", "editor/assets/shaders/flat.frag");
    if (!program_optional.has_value()) {
        abort();
    }
    m_program = std::move(program_optional.value());
    std::optional<std::unique_ptr<Model>> model_optional
        = Model::load("editor/assets/models/teapot.obj");
    if (!model_optional.has_value()) {
        abort();
    }
    m_model = std::move(model_optional.value());
}

Viewport::~Viewport() {
}

void Viewport::on_update(const Window& window, float delta_time)
{
    if (m_is_hovered) {
        m_camera->on_update(window, delta_time);
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
        m_camera->set_projection(glm::perspective(glm::radians(60.0f),
                m_dimension.x / m_dimension.y, 0.01f, 1000.0f));
    }

    m_frame_buffer->bind();
    renderer.clear();
    m_program->use();
    m_program->set_mat4("u_view_projection", m_camera->view_projection());
    m_program->set_mat4("u_model", glm::translate(glm::mat4(1.0f), glm::vec3(0.0f)));
    renderer.draw(*m_model);
    FrameBuffer::bind_default();

    ImGui::Image((void*)(std::intptr_t)m_frame_buffer->color_texture().renderer_id(),
        m_dimension, ImVec2(0.0f, 1.0f), ImVec2(1.0f, 0.0f));

    ImGui::End();
    ImGui::PopStyleVar();
    ImGui::PopStyleVar();
}

}
