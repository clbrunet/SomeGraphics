#include <cstdint>
#include <iostream>

#include <glad/gl.h>

#include "SomeGraphics/Rendering/UniformBuffer.hpp"

namespace sg {

UniformBuffer::UniformBuffer(uint32_t binding, uint32_t size)
{
    glCreateBuffers(1, &m_renderer_id);
    glNamedBufferStorage(m_renderer_id, size, nullptr, GL_DYNAMIC_STORAGE_BIT);
    // TODO: test to glbufferbind 0 for GL_UNIFORM_BUFFER
    glBindBufferBase(GL_UNIFORM_BUFFER, binding, m_renderer_id);
}

UniformBuffer::UniformBuffer(UniformBuffer&& other)
{
    *this = std::move(other);
}

UniformBuffer& UniformBuffer::operator=(UniformBuffer&& other)
{
    if (this == &other) {
        return *this;
    }
    glDeleteBuffers(1, &m_renderer_id);
    m_renderer_id = other.m_renderer_id;
    other.m_renderer_id = 0;
    return *this;
}

UniformBuffer::~UniformBuffer()
{
    glDeleteBuffers(1, &m_renderer_id);
}

void UniformBuffer::update_data(uint32_t offset, uint32_t size, const uint8_t* data) const
{
    glNamedBufferSubData(m_renderer_id, offset, size, data);
}

}
