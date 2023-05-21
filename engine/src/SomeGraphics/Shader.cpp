#include <iostream>
#include <fstream>
#include <memory>
#include <sstream>
#include <string>
#include <vector>

#include "SomeGraphics/Shader.hpp"

namespace sg {

std::optional<std::unique_ptr<Shader>> Shader::create(GLenum type, const char* filename)
{
    std::optional<std::string> source_optional = read_file(filename);
    if (!source_optional.has_value()) {
        return std::nullopt;
    }
    const char* source = source_optional.value().c_str();
    uint shader = glCreateShader(type);
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

Shader::~Shader()
{
    glDeleteShader(m_id);
}

Shader::Shader(uint id) :
    m_id(id)
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
