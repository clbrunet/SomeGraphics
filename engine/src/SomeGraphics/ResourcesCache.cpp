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
    std::string key = filename + "/" + ai_texture.mFilename.C_Str();
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

std::optional<std::shared_ptr<Texture>> ResourcesCache::cubemap(const char* right,
    const char* left, const char* top, const char* bottom, const char* front, const char* back)
{
    std::string key = std::string(right) + "-" + left + "-"
        + top + "-" + bottom + "-" + front + "-" + back;
    std::map<std::string, std::weak_ptr<Texture>>::iterator it = textures_cache.find(key);
    if (it != textures_cache.end() && !it->second.expired()) {
        return it->second.lock();
    }
    std::optional<std::unique_ptr<Texture>> cubemap_opt
        = Texture::create_cubemap(right, left, top, bottom, front, back);
    if (!cubemap_opt.has_value()) {
        return std::nullopt;
    }
    std::shared_ptr<Texture> cubemap = std::move(cubemap_opt.value());
    textures_cache[key] = cubemap;
    return cubemap;
}

std::optional<std::shared_ptr<Program>> ResourcesCache::program(const char* vert_filename,
    const char* frag_filename)
{
    std::string key = std::string(vert_filename) + "-" + frag_filename;
    std::map<std::string, std::weak_ptr<Program>>::iterator it = programs_cache.find(key);
    if (it != programs_cache.end() && !it->second.expired()) {
        return it->second.lock();
    }
    std::optional<std::shared_ptr<Program>> program_opt
        = Program::create(vert_filename, frag_filename);
    if (!program_opt.has_value()) {
        return std::nullopt;
    }
    std::shared_ptr<Program> program = std::move(program_opt.value());
    programs_cache[key] = program;
    return program;
}


std::shared_ptr<Mesh> ResourcesCache::mesh_from_ai_node(const std::string& filename,
    const aiNode& ai_node, const aiScene& ai_scene)
{
    std::string key = filename + "/" + ai_scene.mMeshes[ai_node.mMeshes[0]]->mName.C_Str();
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
    clear_unused_programs();
    clear_unused_meshes();
}

void ResourcesCache::clear_unused_textures()
{
    clear_unused(textures_cache);
}

void ResourcesCache::clear_unused_programs()
{
    clear_unused(programs_cache);
}

void ResourcesCache::clear_unused_meshes()
{
    clear_unused(meshes_cache);
}

std::weak_ptr<Texture> ResourcesCache::white_1px_texture_cache;
std::map<std::string, std::weak_ptr<Texture>> ResourcesCache::textures_cache;
std::map<std::string, std::weak_ptr<Program>> ResourcesCache::programs_cache;
std::map<std::string, std::weak_ptr<Mesh>> ResourcesCache::meshes_cache;

}
