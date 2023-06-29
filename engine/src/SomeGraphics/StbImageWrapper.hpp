#pragma once

#include <sys/types.h>
#include <optional>

namespace sg {

class StbImageWrapper {
public:
    static std::optional<StbImageWrapper> load(const char* filename,
        int channels_count_desired = 0);
    static std::optional<StbImageWrapper> load_from_memory(const u_char* buffer, uint length,
        int channels_count_desired = 0);
    StbImageWrapper() = delete;
    StbImageWrapper(StbImageWrapper&& other);
    StbImageWrapper(const StbImageWrapper& other) = delete;
    StbImageWrapper& operator=(StbImageWrapper&& other);
    StbImageWrapper& operator=(const StbImageWrapper& other) = delete;
    ~StbImageWrapper();

    const u_char* pixels() const;
    int width() const;
    int height() const;
    int channels_count() const;
private:
    u_char* m_pixels = nullptr;
    int m_width;
    int m_height;
    int m_channels_count;

    StbImageWrapper(u_char* pixels, int width, int height, int channels_count);
};

}
