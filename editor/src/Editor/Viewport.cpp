#include <iostream>
#include <memory>
#include <stack>

#include "SomeGraphics/Rendering/Material.hpp"
#include "SomeGraphics/SceneEntity.hpp"
#include "glm/ext/matrix_float4x4.hpp"
#include "glm/ext/vector_float2.hpp"
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
        = Program::create("editor/assets/shaders/post_processing.vert",
            "editor/assets/shaders/post_processing.frag");
    if (!program_opt.has_value()) {
        abort();
    }
    m_post_processing_program = std::move(program_opt.value());
    m_quad = std::make_unique<Mesh>(std::vector<QuadVertex>({
        QuadVertex(glm::vec2(-1.0f, 1.0f), glm::vec2(0.0f, 1.0f)),
        QuadVertex(glm::vec2(-1.0f, -1.0f), glm::vec2(0.0f, 0.0f)),
        QuadVertex(glm::vec2(1.0f, -1.0f), glm::vec2(1.0f, 0.0f)),
        QuadVertex(glm::vec2(1.0f, 1.0f), glm::vec2(1.0f, 1.0f)),
    }), std::initializer_list<VertexAttribute>({
        VertexAttributeType::Vec2,
        VertexAttributeType::Vec2,
    }), std::vector<uint>({
        0, 1, 2, 0, 2, 3,
    }));
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
    renderer.draw(scene, *m_editor_camera);
    renderer.draw(*m_skybox, *m_editor_camera);
    post_processing(renderer);
    FrameBuffer::bind_default();

    ImGui::Image((void*)(std::intptr_t)m_frame_buffer->color_texture().renderer_id(),
        m_dimension, ImVec2(0.0f, 1.0f), ImVec2(1.0f, 0.0f));

    ImGui::End();
    ImGui::PopStyleVar();
    ImGui::PopStyleVar();
}

void Viewport::post_processing(const Renderer& renderer) const
{
    m_post_processing_program->use();
    m_post_processing_program->set_int("u_texture", 0);
    m_frame_buffer->color_texture().bind_to_unit(0);
    renderer.draw(*m_quad);
}

}
