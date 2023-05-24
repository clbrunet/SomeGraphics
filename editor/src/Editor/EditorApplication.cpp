#include <cstdlib>
#include <iostream>
#include <memory>
#include <optional>
#include <vector>

#include <glm/vec3.hpp>

#include "Editor/EditorApplication.hpp"
#include "SomeGraphics/Program.hpp"
#include "SomeGraphics/IndexBuffer.hpp"
#include "SomeGraphics/VertexArray.hpp"
#include "SomeGraphics/VertexAttribute.hpp"
#include "SomeGraphics/VertexBuffer.hpp"

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
    m_vertex_array->bind();
    glDrawElements(GL_TRIANGLES, m_vertex_array->index_buffer().count(),
        m_vertex_array->index_buffer().format(), 0);
}

}
