#pragma once

#include <cstdint>
#include <optional>

#include <glm/ext/matrix_float4x4.hpp>

namespace sg {

#if SG_ENGINE
class Shader;
#endif
class Texture;

class Program {
public:
    static std::optional<Program> create(const char* vert_filename, const char* frag_filename);
    Program() = delete;
    Program(Program&& other);
    Program(const Program& other) = delete;
    Program& operator=(Program&& other);
    Program& operator=(const Program& other) = delete;
    ~Program();

    void use() const;
    void set_bool(const char* name, bool b) const;
    void set_int(const char* name, int i) const;
    void set_uint(const char* name, unsigned u) const;
    void set_float(const char* name, float f) const;
    void set_vec3(const char* name, const glm::vec3& vec3) const;
    void set_vec4(const char* name, const glm::vec4& vec4) const;
    void set_mat4(const char* name, const glm::mat4& mat4) const;

    void print_uniform_block_layout(const char* name) const;
private:
    uint32_t m_renderer_id = 0;

    Program(uint32_t renderer_id);

#if SG_ENGINE
    static std::optional<uint32_t> create_program(const Shader& vert, const Shader& frag);
#endif
};

}
