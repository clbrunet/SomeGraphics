#include <iostream>
#include <memory>
#include <vector>

#include "SomeGraphics/Mesh.hpp"
#include "SomeGraphics/StbImageWrapper.hpp"
#include "glm/ext/vector_float2.hpp"
#include "glm/ext/vector_int2.hpp"
#include "imgui.h"

#include "Editor/Viewport.hpp"
#include "Editor/EditorCamera.hpp"
#include "SomeGraphics.hpp"

namespace sg {

Viewport::Viewport(const Renderer& renderer) :
    m_frame_buffer(std::make_unique<FrameBuffer>(glm::vec2(m_dimension.x, m_dimension.y))),
    m_editor_camera(std::make_unique<EditorCamera>(glm::vec3(2.0f, 2.0f, 2.0f),
            glm::vec2(-35.0f, 45.0f), glm::perspective(glm::radians(60.0f),
                m_dimension.x / m_dimension.y, 0.01f, 1000.0f)))
{
    std::vector<Mesh> meshes;
    meshes.emplace_back(std::vector<Vertex>({
        Vertex(glm::vec3(-1.0f,  1.0f, -1.0f)),
        Vertex(glm::vec3(-1.0f, -1.0f, -1.0f)),
        Vertex(glm::vec3(1.0f, -1.0f, -1.0f)),
        Vertex(glm::vec3(1.0f, -1.0f, -1.0f)),
        Vertex(glm::vec3(1.0f,  1.0f, -1.0f)),
        Vertex(glm::vec3(-1.0f,  1.0f, -1.0f)),

        Vertex(glm::vec3(-1.0f, -1.0f,  1.0f)),
        Vertex(glm::vec3(-1.0f, -1.0f, -1.0f)),
        Vertex(glm::vec3(-1.0f,  1.0f, -1.0f)),
        Vertex(glm::vec3(-1.0f,  1.0f, -1.0f)),
        Vertex(glm::vec3(-1.0f,  1.0f,  1.0f)),
        Vertex(glm::vec3(-1.0f, -1.0f,  1.0f)),

        Vertex(glm::vec3(1.0f, -1.0f, -1.0f)),
        Vertex(glm::vec3(1.0f, -1.0f,  1.0f)),
        Vertex(glm::vec3(1.0f,  1.0f,  1.0f)),
        Vertex(glm::vec3(1.0f,  1.0f,  1.0f)),
        Vertex(glm::vec3(1.0f,  1.0f, -1.0f)),
        Vertex(glm::vec3(1.0f, -1.0f, -1.0f)),

        Vertex(glm::vec3(-1.0f, -1.0f,  1.0f)),
        Vertex(glm::vec3(-1.0f,  1.0f,  1.0f)),
        Vertex(glm::vec3(1.0f,  1.0f,  1.0f)),
        Vertex(glm::vec3(1.0f,  1.0f,  1.0f)),
        Vertex(glm::vec3(1.0f, -1.0f,  1.0f)),
        Vertex(glm::vec3(-1.0f, -1.0f,  1.0f)),

        Vertex(glm::vec3(-1.0f,  1.0f, -1.0f)),
        Vertex(glm::vec3(1.0f,  1.0f, -1.0f)),
        Vertex(glm::vec3(1.0f,  1.0f,  1.0f)),
        Vertex(glm::vec3(1.0f,  1.0f,  1.0f)),
        Vertex(glm::vec3(-1.0f,  1.0f,  1.0f)),
        Vertex(glm::vec3(-1.0f,  1.0f, -1.0f)),

        Vertex(glm::vec3(-1.0f, -1.0f, -1.0f)),
        Vertex(glm::vec3(-1.0f, -1.0f,  1.0f)),
        Vertex(glm::vec3(1.0f, -1.0f, -1.0f)),
        Vertex(glm::vec3(1.0f, -1.0f, -1.0f)),
        Vertex(glm::vec3(-1.0f, -1.0f,  1.0f)),
        Vertex(glm::vec3(1.0f, -1.0f,  1.0f)),
    }),
    std::vector<uint>({
        0, 1, 2, 3, 4, 5,
        6, 7, 8, 9, 10, 11,
        12, 13, 14, 15, 16, 17,
        18, 19, 20, 21, 22, 23,
        24, 25, 26, 27, 28, 29,
        30, 31, 32, 33, 34, 35,
    }));
    std::optional<std::unique_ptr<Program>> program_opt
        = Program::create("editor/assets/shaders/skybox.vert", "editor/assets/shaders/skybox.frag");
    if (!program_opt.has_value()) {
        abort();
    }
    m_skybox_program = std::move(program_opt.value());
    m_skybox_model = std::make_unique<Model>(std::move(meshes));
    std::optional<std::unique_ptr<Texture>> texture_opt
        = Texture::create_cubemap("editor/assets/textures/skybox/right.jpg",
            "editor/assets/textures/skybox/left.jpg", "editor/assets/textures/skybox/top.jpg",
            "editor/assets/textures/skybox/bottom.jpg", "editor/assets/textures/skybox/front.jpg",
            "editor/assets/textures/skybox/back.jpg");
    if (!texture_opt.has_value()) {
        abort();
    }
    m_skybox_texture = std::move(texture_opt.value());
    renderer.set_viewport(glm::ivec2(m_dimension.x, m_dimension.y));
    renderer.set_clear_color(0.0f, 0.5f, 0.0f, 1.0f);
    program_opt
        = Program::create("editor/assets/shaders/flat.vert", "editor/assets/shaders/flat.frag");
    if (!program_opt.has_value()) {
        abort();
    }
    m_program = std::move(program_opt.value());
    std::optional<std::unique_ptr<Model>> model_opt
        = Model::load("editor/assets/models/teapot.obj");
    if (!model_opt.has_value()) {
        abort();
    }
    m_model = std::move(model_opt.value());
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

    glDepthMask(GL_FALSE);
    m_skybox_program->use();
    m_skybox_program->set_mat4("u_view_projection",
        m_editor_camera->projection() * glm::mat4(glm::mat3(m_editor_camera->view())));
    m_skybox_program->set_int("u_skybox", 0);
    m_skybox_texture->bind_to_unit(0);
    renderer.draw(*m_skybox_model);
    glDepthMask(GL_TRUE);

    m_program->use();
    m_program->set_mat4("u_view_projection", m_editor_camera->view_projection());
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
