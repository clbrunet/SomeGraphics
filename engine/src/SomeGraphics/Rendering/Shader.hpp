#pragma once

#include <sys/types.h>
#include <memory>
#include <optional>

#include "glad/gl.h"

namespace sg {

class Shader {
public:
    static std::optional<std::unique_ptr<Shader>> create(GLenum type, const char* filename);
    Shader() = delete;
    Shader(Shader&& other);
    Shader(const Shader& other) = delete;
    Shader& operator=(Shader&& other);
    Shader& operator=(const Shader& other) = delete;
    ~Shader();

    void attach(uint program) const;
    void detach(uint program) const;
private:
    uint m_renderer_id = 0;

    Shader(uint renderer_id);

    static std::optional<std::string> read_file(const char* filename);
};

}
