#include <iostream>
#include <map>
#include <memory>

#include "SomeGraphics/ResourcesCache.hpp"

namespace sg {

std::shared_ptr<Texture> ResourcesCache::white_1px_texture()
{
    if (!white_1px_texture_cache.expired()) {
        return white_1px_texture_cache.lock();
    }
    std::shared_ptr<Texture> texture = Texture::white_1px();
    white_1px_texture_cache = texture;
    return texture;
}

std::optional<std::shared_ptr<Texture>> ResourcesCache::texture_from_ai_texture(
    const std::string& filename, const aiTexture& ai_texture, ColorSpace color_space)
{
    std::string key = filename + ai_texture.mFilename.C_Str();
    std::map<std::string, std::weak_ptr<Texture>>::iterator it = textures_cache.find(key);
    if (it != textures_cache.end() && !it->second.expired()) {
        return it->second.lock();
    }
    std::optional<std::unique_ptr<Texture>> texture_opt
        = Texture::from_ai_texture(ai_texture, color_space);
    if (!texture_opt.has_value()) {
        return std::nullopt;
    }
    std::shared_ptr<Texture> texture = std::move(texture_opt.value());
    textures_cache[key] = texture;
    return texture;
}

std::shared_ptr<Mesh> ResourcesCache::mesh_from_ai_node(const std::string& filename,
    const aiNode& ai_node, const aiScene& ai_scene)
{
    std::string key = filename + ai_scene.mMeshes[ai_node.mMeshes[0]]->mName.C_Str();
    std::map<std::string, std::weak_ptr<Mesh>>::iterator it = meshes_cache.find(key);
    if (it != meshes_cache.end() && !it->second.expired()) {
        return it->second.lock();
    }
    std::shared_ptr<Mesh> mesh = std::make_shared<Mesh>(ai_node, ai_scene);
    meshes_cache[key] = mesh;
    return mesh;
}

void ResourcesCache::clear_unused()
{
    clear_unused_textures();
    clear_unused_meshes();
}

void ResourcesCache::clear_unused_textures()
{
    clear_unused(textures_cache);
}

void ResourcesCache::clear_unused_meshes()
{
    clear_unused(meshes_cache);
}

std::weak_ptr<Texture> ResourcesCache::white_1px_texture_cache;
std::map<std::string, std::weak_ptr<Texture>> ResourcesCache::textures_cache;
std::map<std::string, std::weak_ptr<Mesh>> ResourcesCache::meshes_cache;

}
