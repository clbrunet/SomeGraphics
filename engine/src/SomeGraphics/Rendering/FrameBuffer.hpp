#pragma once

#include <sys/types.h>
#include <memory>

#include "glm/ext/vector_float2.hpp"

#include "SomeGraphics/Rendering/Texture.hpp"
#include "SomeGraphics/Rendering/RenderBuffer.hpp"

namespace sg {

class FrameBuffer {
public:
    FrameBuffer() = delete;
    FrameBuffer(const glm::vec2& dimension);
    FrameBuffer(FrameBuffer&& other);
    FrameBuffer(const FrameBuffer& other) = delete;
    FrameBuffer& operator=(FrameBuffer&& other);
    FrameBuffer& operator=(const FrameBuffer& other) = delete;
    ~FrameBuffer();

    static void bind_default();
    void bind() const;
    const std::unique_ptr<Texture>& color_texture() const;
private:
    uint32_t m_renderer_id = 0;
    std::unique_ptr<Texture> m_color_texture;
    std::unique_ptr<RenderBuffer> m_depth_and_stencil_render_buffer;
};

}
