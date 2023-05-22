#pragma once

#include <sys/types.h>
#include <memory>
#include <optional>

#include <glad/gl.h>

namespace sg {

class Shader;

class Program {
public:
    static std::optional<std::unique_ptr<Program>> create(const char* vert_filename,
        const char* frag_filename);
    ~Program();
    void use() const;
private:
    Program(uint id);

    uint m_id = 0;
    static std::optional<uint> create_program(const Shader& vert, const Shader& frag);
};

}
