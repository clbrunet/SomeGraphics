#include <optional>
#include <array>

#include "assimp/scene.h"
#include "glm/ext/vector_float4.hpp"

#include "SomeGraphics/Rendering/Material.hpp"
#include "SomeGraphics/Rendering/Program.hpp"
#include "SomeGraphics/Rendering/Texture.hpp"
#include "SomeGraphics/AssimpToGlm.hpp"
#include "SomeGraphics/ResourcesCache.hpp"

namespace sg {

std::optional<std::unique_ptr<Material>> Material::from_ai_material(const std::string& filename,
    const aiMaterial& ai_material, const aiScene& ai_scene)
{
    std::optional<std::shared_ptr<Program>> program_opt
        = ResourcesCache::program("engine/assets/shaders/pbr.vert",
            "engine/assets/shaders/pbr.frag");
    if (!program_opt.has_value()) {
        return std::nullopt;
    }
    std::map<std::string, glm::vec4> vec4s;
    aiColor3D ai_color3;
    ai_material.Get(AI_MATKEY_BASE_COLOR, ai_color3);
    vec4s["u_color"] = glm::vec4(AssimpToGlm::vec3(ai_color3), 1.0f);

    std::map<std::string, std::shared_ptr<Texture>> textures;
    struct TextureInfo {
        std::string location;
        aiTextureType type;
        ColorSpace color_space;
    };
    std::array<TextureInfo, 3> textures_info = {
        TextureInfo { "u_albedo_map", aiTextureType_BASE_COLOR, ColorSpace::Srgb },
        TextureInfo { "u_roughness_map", aiTextureType_DIFFUSE_ROUGHNESS, ColorSpace::Linear },
        TextureInfo { "u_metalness_map", aiTextureType_METALNESS, ColorSpace::Linear },
    };
    for (const TextureInfo& texture_info : textures_info) {
        if (ai_material.GetTextureCount(texture_info.type) == 0) {
            textures[texture_info.location] = ResourcesCache::white_1px_texture();
        } else {
            aiString path;
            ai_material.GetTexture(texture_info.type, 0, &path);
            const aiTexture* ai_texture = ai_scene.GetEmbeddedTexture(path.C_Str());
            std::optional<std::shared_ptr<Texture>> texture_opt
                = ResourcesCache::texture_from_ai_texture(filename, *ai_texture,
                    texture_info.color_space);
            if (!texture_opt.has_value()) {
                return std::nullopt;
            }
            textures[texture_info.location] = std::move(texture_opt.value());
        }
    }
    return std::unique_ptr<Material>(new Material(std::move(program_opt.value()),
            std::move(vec4s), std::move(textures)));
}

const std::shared_ptr<Program>& Material::program() const
{
    return m_program;
}

const std::map<std::string, glm::vec4>& Material::vec4s() const
{
    return m_vec4s;
}

const std::map<std::string, std::shared_ptr<Texture>>& Material::textures() const
{
    return m_textures;
}

void Material::set_vec4(const std::string& location, glm::vec4 vec4)
{
    m_vec4s[location] = std::move(vec4);
}

void Material::set_program_data() const
{
    for (const auto& [location, vec4] : m_vec4s) {
        m_program->set_vec4(location.c_str(), vec4);
    }
    int i = 0;
    for (const auto& [location, texture] : m_textures) {
        m_program->set_int(location.c_str(), i);
        texture->bind_to_unit(i);
        i++;
    }
}

Material::Material(std::shared_ptr<Program>&& program, std::map<std::string, glm::vec4>&& vec4s,
        std::map<std::string, std::shared_ptr<Texture>>&& textures) :
    m_program(std::move(program)),
    m_vec4s(std::move(vec4s)),
    m_textures(std::move(textures))
{
}

}
