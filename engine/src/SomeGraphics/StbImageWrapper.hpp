#pragma once

#include <sys/types.h>
#include <cstdint>
#include <optional>

namespace sg {

class StbImageWrapper {
public:
    static std::optional<StbImageWrapper> load(const char* filename,
        uint32_t channels_count_desired = 0);
    static std::optional<StbImageWrapper> load_from_memory(const uint8_t* buffer, uint32_t length,
        uint32_t channels_count_desired = 0);
    StbImageWrapper() = delete;
    StbImageWrapper(StbImageWrapper&& other);
    StbImageWrapper(const StbImageWrapper& other) = delete;
    StbImageWrapper& operator=(StbImageWrapper&& other);
    StbImageWrapper& operator=(const StbImageWrapper& other) = delete;
    ~StbImageWrapper();

    const uint8_t* pixels() const;
    uint32_t width() const;
    uint32_t height() const;
    uint32_t channels_count() const;
private:
    uint8_t* m_pixels = nullptr;
    uint32_t m_width;
    uint32_t m_height;
    uint32_t m_channels_count;

    StbImageWrapper(uint8_t* pixels, uint32_t width, uint32_t height, uint32_t channels_count);
};

}
