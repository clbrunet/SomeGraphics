#include <cstdlib>
#include <iostream>
#include <memory>
#include <optional>
#include <vector>

#include "glm/vec3.hpp"
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/ext/vector_float3.hpp"

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
    std::optional<std::unique_ptr<Program>> program_optional
        = Program::create("assets/shaders/flat_color.vert", "assets/shaders/flat_color.frag");
    if (!program_optional.has_value()) {
        abort();
    }
    m_program = std::move(program_optional.value());
    m_vertex_array = std::make_unique<VertexArray>(std::vector<glm::vec3>({
        glm::vec3(-0.5f, -0.5f, 0.5f),
        glm::vec3(-0.5f, 0.5f, 0.5f),
        glm::vec3(0.5f, -0.5f, 0.5f),
        glm::vec3(0.5f, 0.5f, 0.5f),
        glm::vec3(-0.5f, -0.5f, -0.5f),
        glm::vec3(-0.5f, 0.5f, -0.5f),
        glm::vec3(0.5f, -0.5f, -0.5f),
        glm::vec3(0.5f, 0.5f, -0.5f),
    }), std::initializer_list<VertexAttribute>({
        VertexAttribute(VertexAttributeType::Vec3),
    }), std::vector<uint>({
        2, 1, 0, 2, 3, 1,
        1, 3, 5, 3, 7, 5,
        2, 6, 3, 6, 7, 3,
        4, 0, 5, 0, 1, 5,
        4, 6, 0, 6, 2, 0,
        4, 5, 6, 5, 7, 6,
    }));
    m_camera = std::make_unique<EditorCamera>(glm::vec3(2.0f, 2.0f, 2.0f), glm::vec2(-35.0f, 45.0f),
        glm::perspective(glm::radians(45.0f), 800.0f / 450.0f, 0.01f, 1000.0f));
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
    m_renderer.clear();
    m_program->use();
    m_program->set_mat4("view_projection", m_camera->view_projection());
    m_renderer.draw(*m_vertex_array);
}

}
