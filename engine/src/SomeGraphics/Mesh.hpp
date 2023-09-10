#pragma once

#include <iostream>
#include <memory>
#include <sys/types.h>
#include <vector>

#include "assimp/scene.h"
#include "glm/ext/vector_float3.hpp"
#include "glm/gtx/string_cast.hpp"

#include "SomeGraphics/Rendering/VertexArray.hpp"

namespace sg {

class Mesh {
public:
    Mesh() = delete;
    Mesh(const aiNode& ai_node, const aiScene& ai_scene);
    template<typename T>
    Mesh(const std::vector<T>& vertices, std::initializer_list<VertexAttribute> attributes,
        const std::vector<uint32_t>& indices) :
        m_vertex_array(std::make_unique<VertexArray>(vertices, attributes, indices))
    {
    }

    Mesh(Mesh&& other) = default;
    Mesh(const Mesh& other) = delete;
    Mesh& operator=(Mesh&& other) = default;
    Mesh& operator=(const Mesh& other) = delete;
    ~Mesh() = default;

    const std::unique_ptr<VertexArray>& vertex_array() const;
private:
    std::unique_ptr<VertexArray> m_vertex_array;

    struct Vertex {
        glm::vec3 position;
        glm::vec3 normal;
        glm::vec2 texture_coordinates;

        Vertex() = delete;
        Vertex(glm::vec3 position, glm::vec3 normal, glm::vec2 texture_coordinates) :
            position(std::move(position)),
            normal(std::move(normal)),
            texture_coordinates(std::move(texture_coordinates))
        {
        }
        Vertex(Vertex&& other) = default;
        Vertex(const Vertex& other) = default;
        Vertex& operator=(Vertex&& other) = default;
        Vertex& operator=(const Vertex& other) = default;
        ~Vertex() = default;
    };
};

}
