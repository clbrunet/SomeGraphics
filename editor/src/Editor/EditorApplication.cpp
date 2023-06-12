#include <cstdint>
#include <cstdlib>
#include <memory>
#include <optional>
#include <vector>

#include "SomeGraphics/FrameBuffer.hpp"
#include "glm/ext/matrix_float4x4.hpp"
#include "glm/ext/vector_float2.hpp"
#include "glm/ext/vector_int2.hpp"
#include "glm/vec3.hpp"
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/ext/vector_float3.hpp"
#include "imgui.h"

#include "Editor/EditorApplication.hpp"

std::unique_ptr<sg::Application> create_app()
{
    return std::make_unique<sg::EditorApplication>("SomeGraphics Editor");
}

namespace sg {

EditorApplication::EditorApplication(const std::string& name) :
    Application(name)
{
    m_renderer.set_clear_color(0.0f, 0.5f, 0.0f, 1.0f);
    m_frame_buffer = std::make_unique<FrameBuffer>(glm::vec2(800, 450));
    std::optional<std::unique_ptr<Program>> program_optional
        = Program::create("assets/shaders/color.vert", "assets/shaders/color.frag");
    if (!program_optional.has_value()) {
        abort();
    }
    m_program = std::move(program_optional.value());
    struct Vertex {
        glm::vec3 position;
        glm::vec3 color;

        Vertex(const glm::vec3& position, const glm::vec3& color) :
            position(position),
            color(color)
        {
        }
    };
    m_vertex_array = std::make_unique<VertexArray>(std::vector<Vertex>({
        Vertex(glm::vec3(-0.5f, -0.5f, 0.5f), glm::vec3(0.0f, 0.0f, 0.0f)),
        Vertex(glm::vec3(-0.5f, 0.5f, 0.5f), glm::vec3(0.0f, 0.0f, 1.0f)),
        Vertex(glm::vec3(0.5f, -0.5f, 0.5f), glm::vec3(0.0f, 1.0f, 0.0f)),
        Vertex(glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(0.0f, 1.0f, 1.0f)),
        Vertex(glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(1.0f, 0.0f, 0.0f)),
        Vertex(glm::vec3(-0.5f, 0.5f, -0.5f), glm::vec3(1.0f, 0.0f, 1.0f)),
        Vertex(glm::vec3(0.5f, -0.5f, -0.5f), glm::vec3(1.0f, 1.0f, 0.0f)),
        Vertex(glm::vec3(0.5f, 0.5f, -0.5f), glm::vec3(1.0f, 1.0f, 1.0f)),
    }), std::initializer_list<VertexAttribute>({
        VertexAttribute(VertexAttributeType::Vec3),
        VertexAttribute(VertexAttributeType::Vec3),
    }), std::vector<uint>({
        2, 1, 0, 2, 3, 1,
        1, 3, 5, 3, 7, 5,
        2, 6, 3, 6, 7, 3,
        4, 0, 5, 0, 1, 5,
        4, 6, 0, 6, 2, 0,
        4, 5, 6, 5, 7, 6,
    }));
    glm::vec2 dimension = m_window.get_dimension();
    m_camera = std::make_unique<EditorCamera>(glm::vec3(2.0f, 2.0f, 2.0f), glm::vec2(-35.0f, 45.0f),
        glm::perspective(glm::radians(60.0f), dimension.x / dimension.y, 0.01f, 1000.0f));
}

EditorApplication::~EditorApplication()
{
}

void EditorApplication::on_update(float delta_time)
{
    m_camera->on_update(m_window, delta_time);
}

void EditorApplication::on_render()
{
    m_frame_buffer->bind();
    m_renderer.clear();
    m_program->use();
    m_program->set_mat4("u_view_projection", m_camera->view_projection());
    m_program->set_mat4("u_model", glm::translate(glm::mat4(1.0f), glm::vec3(0.0f)));
    m_renderer.draw(*m_vertex_array);
    m_program->set_mat4("u_model", glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -2.0f, -2.0f)));
    m_renderer.draw(*m_vertex_array);

    bool show_demo_window = true;
    ImGui::ShowDemoWindow(&show_demo_window);
    ImGui::Begin("Viewport");
    ImGui::Image((void*)(std::intptr_t)m_frame_buffer->color_texture().id(),
        ImVec2(800.0f, 450.0f), ImVec2(0.0f, 1.0f), ImVec2(1.0f, 0.0f));
    ImGui::End();
}

}
