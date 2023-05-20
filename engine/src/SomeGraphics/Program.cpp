#include <iostream>
#include <fstream>
#include <memory>
#include <sstream>
#include <string>
#include <vector>

#include "SomeGraphics/Program.hpp"

namespace sg {

std::optional<std::unique_ptr<Program>> Program::create(const char* vert_filename,
                                                        const char* frag_filename)
{
    std::optional<uint> vert_optional = create_shader(GL_VERTEX_SHADER, vert_filename);
    if (!vert_optional.has_value()) {
        return std::nullopt;
    }
    uint vert = vert_optional.value();
    std::optional<uint> frag_optional = create_shader(GL_FRAGMENT_SHADER, frag_filename);
    if (!frag_optional.has_value()) {
        glDeleteShader(vert);
        return std::nullopt;
    }
    uint frag = frag_optional.value();
    std::optional<uint> program = create_program(vert, frag);
    glDeleteShader(vert);
    glDeleteShader(frag);
    if (!program.has_value()) {
        return std::nullopt;
    }
    return std::unique_ptr<Program>(new Program(program.value()));
}

Program::~Program()
{
    glDeleteProgram(m_id);
}

void Program::use() const
{
    glUseProgram(m_id);
}

Program::Program(uint id) :
    m_id(id)
{
}

std::optional<uint> Program::create_shader(GLenum type, const char* filename)
{
    std::ifstream ifstream(filename);
    if (ifstream.fail()) {
        std::cerr << "Couldn't read shader '" << filename << "'." << std::endl;
        return std::nullopt;
    }
    std::stringstream buffer;
    buffer << ifstream.rdbuf();
    std::string string = buffer.str();
    const char* source = string.c_str();
    uint shader = glCreateShader(type);
    glShaderSource(shader, 1, &source, nullptr);
    glCompileShader(shader);
    int success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (success)
    {
        return shader;
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

std::optional<uint> Program::create_program(uint vert, uint frag)
{
    uint program = glCreateProgram();
    glAttachShader(program, vert);
    glAttachShader(program, frag);
    glLinkProgram(program);
    glDetachShader(program, vert);
    glDetachShader(program, frag);
    int success;
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (success)
    {
        return program;
    }
    int info_log_length;
    glGetProgramiv(program, GL_INFO_LOG_LENGTH, &info_log_length);
    std::vector<char> info_log(info_log_length);
    glGetProgramInfoLog(program, info_log_length, nullptr, info_log.data());
    std::cerr << "Program link error :\n";
    for (char c : info_log) {
        std::cerr << c;
    }
    std::cerr << std::flush;
    return std::nullopt;
}

}
