#include <iostream>
#include <fstream>
#include <memory>
#include <sstream>
#include <string>
#include <vector>

#include "SomeGraphics/Rendering/Shader.hpp"

namespace sg {

std::optional<std::unique_ptr<Shader>> Shader::create(GLenum type, const char* filename)
{
    std::optional<std::string> source_opt = read_file(filename);
    if (!source_opt.has_value()) {
        return std::nullopt;
    }
    const char* source = source_opt.value().c_str();
    uint32_t shader = glCreateShader(type);
    glShaderSource(shader, 1, &source, nullptr);
    glCompileShader(shader);
    int success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (success)
    {
        return std::unique_ptr<Shader>(new Shader(shader));
    }
    int info_log_length;
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &info_log_length);
    std::vector<char> info_log(info_log_length);
    glGetShaderInfoLog(shader, info_log_length, nullptr, info_log.data());
    glDeleteShader(shader);
    std::cerr << "Shader '" << filename << "' compilation error :\n";
    for (char c : info_log) {
        std::cerr << c;
    }
    std::cerr << std::flush;
    return std::nullopt;
}

Shader::Shader(Shader&& other)
{
    *this = std::move(other);
}

Shader& Shader::operator=(Shader&& other)
{
    if (this == &other) {
        return *this;
    }
    glDeleteShader(m_renderer_id);
    m_renderer_id = other.m_renderer_id;
    other.m_renderer_id = 0;
    return *this;
}

Shader::~Shader()
{
    glDeleteShader(m_renderer_id);
}

void Shader::attach(uint32_t program) const
{
    glAttachShader(program, m_renderer_id);
}

void Shader::detach(uint32_t program) const
{
    glDetachShader(program, m_renderer_id);
}

Shader::Shader(uint32_t renderer_id) :
    m_renderer_id(renderer_id)
{
}

std::optional<std::string> Shader::read_file(const char* filename)
{
    std::ifstream ifstream(filename);
    if (ifstream.fail()) {
        std::cerr << "Couldn't read shader '" << filename << "'." << std::endl;
        return std::nullopt;
    }
    std::stringstream buffer;
    buffer << ifstream.rdbuf();
    return buffer.str();
}

}
