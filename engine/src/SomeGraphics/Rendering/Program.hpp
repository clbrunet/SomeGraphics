#pragma once

#include <sys/types.h>
#include <memory>
#include <optional>

#include "glm/ext/matrix_float4x4.hpp"

namespace sg {

class Shader;

class Program {
public:
    static std::optional<std::unique_ptr<Program>> create(const char* vert_filename,
        const char* frag_filename);
    ~Program();

    void use() const;
    void set_int(const char* name, int i) const;
    void set_mat4(const char* name, const glm::mat4& mat4) const;
private:
    Program(uint renderer_id);

    uint m_renderer_id = 0;
    static std::optional<uint> create_program(const Shader& vert, const Shader& frag);
};

}
