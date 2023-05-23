#pragma once

#include <initializer_list>
namespace sg {

class VertexAttribute;

class VertexFormat {
public:
    VertexFormat(std::initializer_list<VertexAttribute> attributes);
    ~VertexFormat();

private:
    int m_stride;
};

}
