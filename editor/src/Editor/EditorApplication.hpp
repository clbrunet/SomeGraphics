#pragma once

#include <memory>

#include "SomeGraphics.hpp"
#include "SomeGraphics/Program.hpp"

namespace sg {

class EditorApplication final : public Application {
public:
    EditorApplication(const std::string& name);
    ~EditorApplication() final;
    void on_update() final override;
    void on_render() final override;
private:
    std::unique_ptr<Program> m_program;
    uint m_vertex_array;
    uint m_element_buffer;
};

}
