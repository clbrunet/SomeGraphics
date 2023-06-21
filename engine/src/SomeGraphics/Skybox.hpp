#pragma once

#include <optional>
#include <memory>

namespace sg {

class Program;
class Model;
class Texture;

class Skybox {
public:
    Skybox() = delete;
    static std::optional<std::unique_ptr<Skybox>> create(const char* right, const char* left,
        const char* top, const char* bottom, const char* front, const char* back);
    Skybox(Skybox&& other) = default;
    Skybox(const Skybox& other) = delete;
    Skybox& operator=(Skybox&& other) = delete;
    Skybox& operator=(const Skybox& other) = delete;
    ~Skybox();

    const Program& program() const;
    const Model& model() const;
    const Texture& texture() const;
protected:
    std::unique_ptr<Program> m_program;
    std::unique_ptr<Model> m_model;
    std::unique_ptr<Texture> m_texture;
    static bool is_instantiated;

    Skybox(std::unique_ptr<Program>&& program,
        std::unique_ptr<Model>&& model, std::unique_ptr<Texture>&& texture);
};

}
