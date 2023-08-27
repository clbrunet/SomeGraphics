#include <iostream>

#include "stb_image.h"

#include "SomeGraphics/StbImageWrapper.hpp"

namespace sg {

std::optional<StbImageWrapper> StbImageWrapper::load(const char* filename,
    uint32_t channels_count_desired)
{
    int width, height, channels_count_in_file;
    uint8_t* pixels = stbi_load(filename, &width, &height,
        &channels_count_in_file, channels_count_desired);
    if (pixels == nullptr) {
        std::cerr << "Image '" << filename << "' loading error" << std::endl;
        return std::nullopt;
    }
    return StbImageWrapper(pixels, width, height,
        channels_count_desired == 0 ? channels_count_in_file : channels_count_desired);
}

std::optional<StbImageWrapper> StbImageWrapper::load_from_memory(const uint8_t* buffer, uint32_t size,
    uint32_t channels_count_desired)
{
    int width, height, channels_count_in_buffer;
	uint8_t* pixels = stbi_load_from_memory((const stbi_uc*)buffer, size, &width, &height,
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

const uint8_t* StbImageWrapper::pixels() const
{
    return m_pixels;
}

uint32_t StbImageWrapper::width() const
{
    return m_width;
}

uint32_t StbImageWrapper::height() const
{
    return m_height;
}

uint32_t StbImageWrapper::channels_count() const
{
    return m_channels_count;
}

StbImageWrapper::StbImageWrapper(uint8_t* pixels, uint32_t width, uint32_t height, uint32_t channels_count) :
    m_pixels(pixels), m_width(width), m_height(height), m_channels_count(channels_count)
{
}

}
