#pragma once

#include <map>
#include <string>
#include <memory>
#include <optional>

#include <glm/ext/vector_float2.hpp>
#include <assimp/scene.h>
#include <assimp/texture.h>

#include "SomeGraphics/Rendering/Texture.hpp"
#include "SomeGraphics/Rendering/Program.hpp"
#include "SomeGraphics/Rendering/Material.hpp"
#include "SomeGraphics/Mesh.hpp"

namespace sg {

class ResourcesCache {
public:
    static std::shared_ptr<Texture> white_1px_texture();
    static std::optional<std::shared_ptr<Texture>> texture_from_ai_texture(
        std::string filename, const aiTexture& ai_texture, ColorSpace color_space);
    static std::optional<std::shared_ptr<Texture>> cubemap(const char* right,
        const char* left, const char* top, const char* bottom, const char* front, const char* back);
    static std::optional<std::shared_ptr<Program>> program(const char* vert_filename,
        const char* frag_filename);
    static std::optional<std::shared_ptr<Material>> material_from_ai_material(
        std::string filename, const aiMaterial& ai_material, const aiScene& ai_scene);
    static std::optional<std::shared_ptr<Mesh>> mesh_from_ai_node(std::string filename,
        const aiNode& ai_node, const aiScene& ai_scene);

    static void clear_unused();
    static void clear_unused_textures();
    static void clear_unused_programs();
    static void clear_unused_materials();
    static void clear_unused_meshes();

    ResourcesCache() = delete;
    ResourcesCache(ResourcesCache&& other) = delete;
    ResourcesCache(const ResourcesCache& other) = delete;
    ResourcesCache& operator=(ResourcesCache&& other) = delete;
    ResourcesCache& operator=(const ResourcesCache& other) = delete;
    ~ResourcesCache() = delete;
private:
    static std::weak_ptr<Texture> white_1px_texture_cache;
    static std::map<std::string, std::weak_ptr<Texture>> textures_cache;
    static std::map<std::string, std::weak_ptr<Program>> programs_cache;
    static std::map<std::string, std::weak_ptr<Material>> materials_cache;
    static std::map<std::string, std::weak_ptr<Mesh>> meshes_cache;

    template<typename T>
    static void clear_unused(std::map<std::string, std::weak_ptr<T>> cache)
    {
        typename std::map<std::string, std::weak_ptr<T>>::const_iterator cit = cache.cbegin();
        for (; cit != cache.cend(); ) {
            if (cit->second.expired()) {
                cit = cache.erase(cit);
            } else {
                ++cit;
            }
        }
    }
};

}
