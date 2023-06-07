#pragma once

#include <memory>

#include "glm/mat4x4.hpp"

#include "SomeGraphics.hpp"
#include "Editor/EditorCamera.hpp"

namespace sg {

class Program;
class VertexArray;

class EditorApplication final : public Application {
public:
    EditorApplication(const std::string& name);
    ~EditorApplication() final;
    void on_update(float delta_time) final override;
    void on_render() final override;
private:
    std::unique_ptr<Program> m_program;
    std::unique_ptr<VertexArray> m_vertex_array;
    std::unique_ptr<EditorCamera> m_camera;
};

}
