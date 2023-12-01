#pragma once

#include <sys/types.h>

#include <glm/ext/vector_int2.hpp>

#include "SomeGraphics/Rendering/Texture.hpp"
#include "SomeGraphics/Rendering/RenderBuffer.hpp"

namespace sg {

class FrameBuffer {
public:
    FrameBuffer() = delete;
    FrameBuffer(const glm::ivec2& dimensions, TextureFormat texture_format);
    FrameBuffer(FrameBuffer&& other);
    FrameBuffer(const FrameBuffer& other) = delete;
    FrameBuffer& operator=(FrameBuffer&& other);
    FrameBuffer& operator=(const FrameBuffer& other) = delete;
    ~FrameBuffer();

    static void bind_default();
    void bind() const;
    const Texture& color_texture() const;
private:
    uint32_t m_renderer_id = 0;
    Texture m_color_texture;
    RenderBuffer m_depth_and_stencil_render_buffer;
};

}
