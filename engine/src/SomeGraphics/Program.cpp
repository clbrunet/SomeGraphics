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
    std::optional<std::unique_ptr<Shader>> vert_optional = Shader::create(GL_VERTEX_SHADER,
                                                                          vert_filename);
    std::optional<std::unique_ptr<Shader>> frag_optional = Shader::create(GL_FRAGMENT_SHADER,
                                                                          frag_filename);
    if (!vert_optional.has_value() || !frag_optional.has_value()) {
        return std::nullopt;
    }
    std::unique_ptr<Shader> vert = std::move(vert_optional.value());
    std::unique_ptr<Shader> frag = std::move(frag_optional.value());
    std::optional<uint> program = create_program(*vert, *frag);
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

std::optional<uint> Program::create_program(const Shader& vert, const Shader& frag)
{
    uint program = glCreateProgram();
    glAttachShader(program, vert.m_id);
    glAttachShader(program, frag.m_id);
    glLinkProgram(program);
    glDetachShader(program, vert.m_id);
    glDetachShader(program, frag.m_id);
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
