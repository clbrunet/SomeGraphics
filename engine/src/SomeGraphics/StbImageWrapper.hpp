#pragma once

#include <sys/types.h>
#include <optional>

namespace sg {

class StbImageWrapper {
public:
    static std::optional<StbImageWrapper> load(const char* filename,
        uint channels_count_desired = 0);
    static std::optional<StbImageWrapper> load_from_memory(const u_char* buffer, uint length,
        uint channels_count_desired = 0);
    StbImageWrapper() = delete;
    StbImageWrapper(StbImageWrapper&& other);
    StbImageWrapper(const StbImageWrapper& other) = delete;
    StbImageWrapper& operator=(StbImageWrapper&& other);
    StbImageWrapper& operator=(const StbImageWrapper& other) = delete;
    ~StbImageWrapper();

    const u_char* pixels() const;
    uint width() const;
    uint height() const;
    uint channels_count() const;
private:
    u_char* m_pixels = nullptr;
    uint m_width;
    uint m_height;
    uint m_channels_count;

    StbImageWrapper(u_char* pixels, uint width, uint height, uint channels_count);
};

}
