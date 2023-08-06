#pragma once

#include <map>
#include <string>
#include <memory>
#include <optional>

#include "glm/ext/vector_float2.hpp"
#include "assimp/texture.h"
#include "SomeGraphics/Rendering/Texture.hpp"

namespace sg {

class TexturesCache {
public:
    static std::shared_ptr<Texture> white_1px();
    static std::optional<std::shared_ptr<Texture>> from_ai_texture(const char* key,
        const aiTexture& ai_texture, ColorSpace color_space);

    static void clear_unused_textures();

    TexturesCache() = delete;
    TexturesCache(TexturesCache&& other) = delete;
    TexturesCache(const TexturesCache& other) = delete;
    TexturesCache& operator=(TexturesCache&& other) = delete;
    TexturesCache& operator=(const TexturesCache& other) = delete;
    ~TexturesCache() = delete;
private:
    static std::weak_ptr<Texture> white_1px_cache;
    static std::map<std::string, std::weak_ptr<Texture>> cache;
};

}
