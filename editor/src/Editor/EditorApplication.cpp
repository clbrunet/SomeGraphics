#include <cstdlib>
#include <iostream>
#include <memory>
#include <optional>
#include <vector>

#include <glm/vec3.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include "glm/ext/matrix_transform.hpp"

#include "Editor/EditorApplication.hpp"
#include "glm/ext/vector_float3.hpp"

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
        glm::vec3(-0.5f, -0.5f, 0.0f),
        glm::vec3(0.0f, 0.5f, 0.0f),
        glm::vec3(0.5f, -0.5f, 0.0f),
    }), std::initializer_list<VertexAttribute>({
        VertexAttribute(VertexAttributeType::Vec3),
    }), std::vector<uint>({ 0, 1, 2 }));
    m_view = glm::lookAt(glm::vec3(0.0f, 0.0f, 5.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    m_projection = glm::perspective(glm::radians(45.0f), 800.0f / 450.0f, 0.001f, 1000.0f);
}

EditorApplication::~EditorApplication()
{
}

void EditorApplication::on_update()
{
}

void EditorApplication::on_render()
{
    m_renderer.clear();
    m_program->use();
    m_program->set_mat4("view", m_view);
    m_program->set_mat4("projection", m_projection);
    m_renderer.draw(*m_vertex_array);
}

}
