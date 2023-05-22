#include <cstdlib>
#include <iostream>
#include <memory>
#include <optional>
#include <vector>

#include "Editor/EditorApplication.hpp"
#include "SomeGraphics/Program.hpp"
#include "SomeGraphics/IndexBuffer.hpp"
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
    glCreateVertexArrays(1, &m_vertex_array);
    glBindVertexArray(m_vertex_array);
    m_vertex_buffer = std::make_unique<VertexBuffer>(std::vector<float>({
            -0.5f, -0.5f, 0.0f,
            0.0f, 0.5f, 0.0f,
            +0.5f, -0.5f, 0.0f,
            }));
    VertexAttribute vertex_attribute(VertexAttributeType::Float);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, 0);
    glEnableVertexAttribArray(0);
    m_index_buffer = std::make_unique<IndexBuffer>(std::vector<uint>({ 0, 1, 2 }));
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
    glBindVertexArray(m_vertex_array);
    glDrawElements(GL_TRIANGLES, m_index_buffer->count(), GL_UNSIGNED_INT, 0);
}

}

