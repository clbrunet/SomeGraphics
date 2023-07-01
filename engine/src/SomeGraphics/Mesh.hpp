#pragma once

#include <iostream>
#include <memory>
#include <sys/types.h>
#include <vector>

#include "glm/ext/vector_float3.hpp"
#include "glm/gtx/string_cast.hpp"

#include "SomeGraphics/Rendering/VertexArray.hpp"

namespace sg {

class Mesh {
public:
    Mesh() = delete;
    template<typename T>
    Mesh(const std::vector<T>& vertices, std::initializer_list<VertexAttribute> attributes,
        const std::vector<uint>& indices) :
        m_vertex_array(std::make_unique<VertexArray>(vertices, attributes, indices))
    {
    }

    Mesh(Mesh&& other) = default;
    Mesh(const Mesh& other) = delete;
    Mesh& operator=(Mesh&& other) = default;
    Mesh& operator=(const Mesh& other) = delete;
    ~Mesh() = default;

    const VertexArray& vertex_array() const;
private:
    std::unique_ptr<VertexArray> m_vertex_array;
};

}
