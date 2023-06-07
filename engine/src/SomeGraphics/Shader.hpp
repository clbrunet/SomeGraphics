#pragma once

#include <sys/types.h>
#include <memory>
#include <optional>

#include "glad/gl.h"

namespace sg {

class Shader {
public:
    static std::optional<std::unique_ptr<Shader>> create(GLenum type, const char* filename);
    ~Shader();

    void attach(uint program) const;
    void detach(uint program) const;
private:
    Shader(uint id);

    uint m_id = 0;
    static std::optional<std::string> read_file(const char* filename);
};

}
