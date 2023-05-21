#pragma once

#include <memory>

#include "SomeGraphics/Application.hpp"

namespace sg {

class Program;
class IndexBuffer;

class EditorApplication final : public Application {
public:
    EditorApplication(const std::string& name);
    ~EditorApplication() final;
    void on_update() final override;
    void on_render() final override;
private:
    std::unique_ptr<Program> m_program;
    std::unique_ptr<IndexBuffer> m_index_buffer;
    uint m_vertex_array;
    uint m_element_buffer;
};

}
