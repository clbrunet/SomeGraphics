#pragma once

#include <memory>
#include <optional>
#include <sys/types.h>

#include "glad/gl.h"

namespace sg {

class Program {
public:
    ~Program();

    static std::optional<std::unique_ptr<Program>> create(const char* vert_filename,
                                                          const char* frag_filename);

private:
    Program(uint id);

    uint m_id = 0;
    static std::optional<uint> create_shader(GLenum type, const char* filename);
    static std::optional<uint> create_program(uint vert, uint frag);
};

}
