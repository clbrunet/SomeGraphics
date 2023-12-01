#pragma once

#include <memory>

#include <imgui.h>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/vector_float2.hpp>
#include <glm/ext/vector_float3.hpp>
#include <glm/ext/matrix_float4x4.hpp>

#include "Editor/EditorCamera.hpp"
#include "SomeGraphics/Rendering/Texture.hpp"

namespace sg {

class Renderer;
class Window;
class Renderer;
class FrameBuffer;
class EditorCamera;
class Skybox;
class Program;
class Scene;
class PostProcess;

class Viewport {
public:
    Viewport() = delete;
    Viewport(const Renderer& renderer);
    Viewport(Viewport&& other) = default;
    Viewport(const Viewport& other) = delete;
    Viewport& operator=(Viewport&& other) = default;
    Viewport& operator=(const Viewport& other) = delete;
    ~Viewport() = default;

    void on_update(const Window& window);
    void on_render(const Renderer& renderer, const Scene& scene);
private:
    bool m_is_hovered = false;
    bool m_is_free_flying = false;
    glm::ivec2 m_dimensions = glm::ivec2(800, 450);
    std::unique_ptr<FrameBuffer> m_frame_buffer_a
        = std::make_unique<FrameBuffer>(glm::vec2(m_dimensions.x, m_dimensions.y), TextureFormat::F16Rgb);
    std::unique_ptr<FrameBuffer> m_frame_buffer_b
        = std::make_unique<FrameBuffer>(glm::vec2(m_dimensions.x, m_dimensions.y), TextureFormat::F16Rgb);
    float m_fov_y = glm::radians(60.0f);
    float m_z_near = 0.01f;
    float m_z_far = 1000.0f;
    std::unique_ptr<EditorCamera> m_editor_camera
        = std::make_unique<EditorCamera>(glm::vec3(14.0f, 0.0f, 14.0f),
            glm::vec2(0.0f, 45.0f), glm::perspective(m_fov_y,
                (float)m_dimensions.x / (float)m_dimensions.y, m_z_near, m_z_far));
    std::unique_ptr<Skybox> m_skybox;
    std::unique_ptr<PostProcess> m_post_process = std::make_unique<PostProcess>();
};

}
