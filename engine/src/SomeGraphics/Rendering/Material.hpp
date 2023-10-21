#pragma once

#include <map>
#include <optional>
#include <memory>

#include <assimp/material.h>
#include <assimp/scene.h>
#include <glm/ext/vector_float4.hpp>

namespace sg {

class Program;
class Texture;

class Material {
public:
    Material() = delete;
    static std::optional<std::unique_ptr<Material>> from_ai_material(const std::string& filename,
        const aiMaterial& ai_material, const aiScene& ai_scene);
    Material(Material&& other) = default;
    Material(const Material& other) = delete;
    Material& operator=(Material&& other) = default;
    Material& operator=(const Material& other) = delete;
    ~Material() = default;

    const std::shared_ptr<Program>& program() const;
    const std::map<std::string, bool>& bools() const;
    const std::map<std::string, float>& floats() const;
    const std::map<std::string, glm::vec4>& vec4s() const;
    const std::map<std::string, std::shared_ptr<Texture>>& textures() const;

    void set_bool(const std::string& location, bool b);
    void set_float(const std::string& location, float f);
    void set_vec4(const std::string& location, glm::vec4 vec4);

    void set_program_data() const;
private:
    std::shared_ptr<Program> m_program;
    std::map<std::string, bool> m_bools;
    std::map<std::string, float> m_floats;
    std::map<std::string, glm::vec4> m_vec4s;
    std::map<std::string, std::shared_ptr<Texture>> m_textures;

    Material(std::shared_ptr<Program> program);
};

}
