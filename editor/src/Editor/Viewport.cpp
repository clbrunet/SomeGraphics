#include <cassert>
#include <iostream>
#include <memory>

#include <glm/ext/matrix_float4x4.hpp>
#include <glm/ext/vector_float2.hpp>
#include <glm/ext/vector_int2.hpp>
#include <imgui.h>
#include <SomeGraphics.hpp>

#include "Editor/Viewport.hpp"
#include "Editor/EditorCamera.hpp"

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
        assert(false);
    }
    m_skybox = std::move(skybox_opt.value());
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
        m_frame_buffer_a
            = std::make_unique<FrameBuffer>(glm::ivec2(m_dimension.x, m_dimension.y), true);
        m_frame_buffer_b
            = std::make_unique<FrameBuffer>(glm::ivec2(m_dimension.x, m_dimension.y), true);
        m_editor_camera->set_projection(glm::perspective(glm::radians(60.0f),
                m_dimension.x / m_dimension.y, 0.01f, 1000.0f));
    }
    m_frame_buffer_a->bind();
    renderer.clear();
    renderer.draw(scene, *m_editor_camera);
    renderer.draw(*m_skybox, *m_editor_camera);
    m_frame_buffer_b->bind();
    renderer.post_process(*m_post_process, *m_frame_buffer_a->color_texture());
    FrameBuffer::bind_default();
    ImGui::Image(m_frame_buffer_b->color_texture()->imgui_texture_id(),
        m_dimension, ImVec2(0.0f, 1.0f), ImVec2(1.0f, 0.0f));
    ImGui::End();
    ImGui::PopStyleVar();
    ImGui::PopStyleVar();
}

}
