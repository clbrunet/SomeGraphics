#pragma once

#include <memory>

#include "imgui.h"
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/vector_float2.hpp"
#include "glm/ext/vector_float3.hpp"
#include "glm/ext/matrix_float4x4.hpp"

#include "Editor/EditorCamera.hpp"

namespace sg {

class Renderer;
class Window;
class Renderer;
class FrameBuffer;
class EditorCamera;
class Skybox;
class Program;
class Scene;

class Viewport {
public:
    Viewport() = delete;
    Viewport(const Renderer& renderer);
    Viewport(Viewport&& other) = default;
    Viewport(const Viewport& other) = delete;
    Viewport& operator=(Viewport&& other) = default;
    Viewport& operator=(const Viewport& other) = delete;
    ~Viewport() = default;

    void on_update(const Window& window, float delta_time);
    void on_render(const Renderer& renderer, const Scene& scene);
private:
    ImVec2 m_dimension = ImVec2(800.0f, 450.0f);
    bool m_is_hovered = false;
    std::unique_ptr<FrameBuffer> m_frame_buffer = std::make_unique<FrameBuffer>(
        glm::vec2(m_dimension.x, m_dimension.y));
    std::unique_ptr<EditorCamera> m_editor_camera = std::make_unique<EditorCamera>(glm::vec3(10.0f, 10.0f, 10.0f),
            glm::vec2(-35.0f, 45.0f), glm::perspective(glm::radians(60.0f),
                m_dimension.x / m_dimension.y, 0.01f, 1000.0f));
    std::unique_ptr<Skybox> m_skybox;
    std::unique_ptr<Program> m_program;

    void render_scene(const Renderer& renderer, const Scene& scene) const;
};

}
