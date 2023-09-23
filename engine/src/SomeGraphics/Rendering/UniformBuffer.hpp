#pragma once

#include <cstdint>
#include <string>
#include <vector>

namespace sg {

class UniformBuffer {
public:
    UniformBuffer() = delete;
    UniformBuffer(uint32_t binding, uint32_t size);
    UniformBuffer(UniformBuffer&& other);
    UniformBuffer(const UniformBuffer& other) = delete;
    UniformBuffer& operator=(UniformBuffer&& other);
    UniformBuffer& operator=(const UniformBuffer& other) = delete;
    ~UniformBuffer();

    template<typename T>
    void update_data(T& data) const
    {
        glNamedBufferSubData(m_renderer_id, 0, sizeof(T), &data);
    }
    void update_data(uint32_t offset, uint32_t size, const uint8_t* data) const;
private:
    uint32_t m_renderer_id = 0;
};

}
