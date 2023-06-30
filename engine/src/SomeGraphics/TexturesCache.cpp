#include <map>

#include "SomeGraphics/TexturesCache.hpp"

namespace sg {

std::optional<std::shared_ptr<Texture>> TexturesCache::from_ai_texture(const char* key,
    const aiTexture& ai_texture)
{
    std::map<std::string, std::weak_ptr<Texture>>::iterator it = cache.find(key);
    if (it != cache.end() && !it->second.expired()) {
        return it->second.lock();
    }
    std::optional<std::unique_ptr<Texture>> texture_opt = Texture::from_ai_texture(ai_texture);
    if (!texture_opt.has_value()) {
        return std::nullopt;
    }
    std::shared_ptr<Texture> texture = std::move(texture_opt.value());
    cache[key] = texture;
    return texture;
}

void TexturesCache::clear_unused_textures()
{
    std::map<std::string, std::weak_ptr<Texture>>::const_iterator cit = cache.cbegin();
    for (; cit != cache.cend(); ) {
        if (cit->second.expired()) {
            cit = cache.erase(cit);
        } else {
            ++cit;
        }
    }
}

std::map<std::string, std::weak_ptr<Texture>> TexturesCache::cache;

}
