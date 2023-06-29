#include <iostream>

#include "stb_image.h"

#include "SomeGraphics/StbImageWrapper.hpp"

namespace sg {

std::optional<StbImageWrapper> StbImageWrapper::load(const char* filename,
    int channels_count_desired)
{
    int width, height, channels_count_in_file;
    u_char *pixels = stbi_load(filename, &width, &height,
        &channels_count_in_file, channels_count_desired);
    if (pixels == nullptr) {
        std::cerr << "Image '" << filename << "' loading error" << std::endl;
        return std::nullopt;
    }
    return StbImageWrapper(pixels, width, height,
        channels_count_desired == 0 ? channels_count_in_file : channels_count_desired);
}

std::optional<StbImageWrapper> StbImageWrapper::load_from_memory(const u_char* buffer, uint size, 
    int channels_count_desired)
{
    int width, height, channels_count_in_buffer;
    u_char *pixels = stbi_load_from_memory(buffer, size, &width, &height,
        &channels_count_in_buffer, channels_count_desired);
    if (pixels == nullptr) {
        std::cerr << "Image at memory " << buffer << " loading error" << std::endl;
        return std::nullopt;
    }
    return StbImageWrapper(pixels, width, height,
        channels_count_desired == 0 ? channels_count_in_buffer : channels_count_desired);
}

StbImageWrapper::StbImageWrapper(StbImageWrapper&& other)
{
    *this = std::move(other);
}

StbImageWrapper& StbImageWrapper::operator=(StbImageWrapper&& other)
{
    if (this == &other) {
        return *this;
    }
    stbi_image_free(m_pixels);
    m_pixels = other.m_pixels;
    m_width = other.m_width;
    m_height = other.m_height;
    m_channels_count = other.m_channels_count;
    other.m_pixels = nullptr;
    return *this;
}

StbImageWrapper::~StbImageWrapper()
{
    stbi_image_free(m_pixels);
}

const u_char* StbImageWrapper::pixels() const
{
    return m_pixels;
}

int StbImageWrapper::width() const
{
    return m_width;
}

int StbImageWrapper::height() const
{
    return m_height;
}

int StbImageWrapper::channels_count() const
{
    return m_channels_count;
}

StbImageWrapper::StbImageWrapper(u_char* pixels, int width, int height, int channels_count) :
    m_pixels(pixels), m_width(width), m_height(height), m_channels_count(channels_count)
{
}

}
