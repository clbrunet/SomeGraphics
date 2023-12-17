#include <optional>

#include "SomeGraphics/PostProcess.hpp"
#include "SomeGraphics/ResourcesCache.hpp"
#include "SomeGraphics/Rendering/Program.hpp"
#include "SomeGraphics/Rendering/VertexArray.hpp"

namespace sg {

PostProcess::PostProcess()
{
    std::optional<std::shared_ptr<Program>> program_opt
        = ResourcesCache::program("engine/assets/shaders/post_processing.vert",
            "engine/assets/shaders/post_processing.frag");
    m_program = std::move(program_opt.value());
    m_vertex_array = std::make_unique<VertexArray>(std::span<const Vertex>(
        std::initializer_list<Vertex>({
            Vertex(glm::vec2(-1.0f, 1.0f), glm::vec2(0.0f, 1.0f)),
            Vertex(glm::vec2(-1.0f, -1.0f), glm::vec2(0.0f, 0.0f)),
            Vertex(glm::vec2(1.0f, -1.0f), glm::vec2(1.0f, 0.0f)),
            Vertex(glm::vec2(1.0f, 1.0f), glm::vec2(1.0f, 1.0f)),
        })), Vertex::attributes(),
        std::initializer_list<uint32_t>({ 0, 1, 2, 0, 2, 3, }));
}

const std::shared_ptr<Program>& PostProcess::program() const
{
    return m_program;
}

const std::unique_ptr<VertexArray>& PostProcess::vertex_array() const
{
    return m_vertex_array;
}

}
