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

    void attach(uint32_t program) const;
    void detach(uint32_t program) const;
private:
    uint32_t m_renderer_id = 0;

    Shader(uint32_t renderer_id);

    static std::optional<std::string> read_file(const char* filename);
};

}
