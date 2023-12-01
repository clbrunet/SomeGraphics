#pragma once

#include <sys/types.h>

#include <glm/ext/vector_int2.hpp>

#include "SomeGraphics/Rendering/Texture.hpp"

namespace sg {

class DepthFrameBuffer {
public:
    DepthFrameBuffer() = delete;
    DepthFrameBuffer(const glm::ivec2& dimensions);
    static DepthFrameBuffer create_cubemap(const glm::ivec2& dimensions);
    DepthFrameBuffer(DepthFrameBuffer&& other);
    DepthFrameBuffer(const DepthFrameBuffer& other) = delete;
    DepthFrameBuffer& operator=(DepthFrameBuffer&& other);
    DepthFrameBuffer& operator=(const DepthFrameBuffer& other) = delete;
    ~DepthFrameBuffer();

    void attach_face(CubemapFace face) const;
    void bind() const;
    const Texture& depth_texture() const;
private:
    uint32_t m_renderer_id = 0;
    Texture m_depth_texture;

    DepthFrameBuffer(Texture depth_texture);
};

}
