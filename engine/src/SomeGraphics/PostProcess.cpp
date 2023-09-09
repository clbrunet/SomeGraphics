#include <optional>

#include "SomeGraphics/PostProcess.hpp"
#include "SomeGraphics/ResourcesCache.hpp"
#include "SomeGraphics/Rendering/Program.hpp"
#include "SomeGraphics/Mesh.hpp"

namespace sg {

PostProcess::PostProcess()
{
    std::optional<std::shared_ptr<Program>> program_opt
        = ResourcesCache::program("engine/assets/shaders/post_processing.vert",
            "engine/assets/shaders/post_processing.frag");
    m_program = std::move(program_opt.value());
    m_quad = std::make_unique<Mesh>(std::vector<Vertex>({
        Vertex(glm::vec2(-1.0f, 1.0f), glm::vec2(0.0f, 1.0f)),
        Vertex(glm::vec2(-1.0f, -1.0f), glm::vec2(0.0f, 0.0f)),
        Vertex(glm::vec2(1.0f, -1.0f), glm::vec2(1.0f, 0.0f)),
        Vertex(glm::vec2(1.0f, 1.0f), glm::vec2(1.0f, 1.0f)),
    }), std::initializer_list<VertexAttribute>({
        VertexAttributeType::Vec2,
        VertexAttributeType::Vec2,
    }), std::vector<uint32_t>({
        0, 1, 2, 0, 2, 3,
    }));
}

const std::shared_ptr<Program>& PostProcess::program() const
{
    return m_program;
}

const std::unique_ptr<Mesh>& PostProcess::quad() const
{
    return m_quad;
}

}
