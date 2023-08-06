#include <iostream>
#include <fstream>
#include <memory>
#include <sstream>
#include <string>
#include <vector>

#include "glm/gtc/type_ptr.hpp"

#include "SomeGraphics/Rendering/Program.hpp"
#include "SomeGraphics/Rendering/Shader.hpp"

namespace sg {

std::optional<std::unique_ptr<Program>> Program::create(const char* vert_filename,
    const char* frag_filename)
{
    std::optional<std::unique_ptr<Shader>> vert_opt = Shader::create(GL_VERTEX_SHADER,
        vert_filename);
    std::optional<std::unique_ptr<Shader>> frag_opt = Shader::create(GL_FRAGMENT_SHADER,
        frag_filename);
    if (!vert_opt.has_value() || !frag_opt.has_value()) {
        return std::nullopt;
    }
    std::unique_ptr<Shader> vert = std::move(vert_opt.value());
    std::unique_ptr<Shader> frag = std::move(frag_opt.value());
    std::optional<uint> program = create_program(*vert, *frag);
    if (!program.has_value()) {
        return std::nullopt;
    }
    return std::unique_ptr<Program>(new Program(program.value()));
}

Program::Program(Program&& other)
{
    *this = std::move(other);
}

Program& Program::operator=(Program&& other)
{
    if (this == &other) {
        return *this;
    }
    glDeleteProgram(m_renderer_id);
    m_renderer_id = other.m_renderer_id;
    other.m_renderer_id = 0;
    return *this;
}

Program::~Program()
{
    glDeleteProgram(m_renderer_id);
}

void Program::use() const
{
    glUseProgram(m_renderer_id);
}

void Program::set_int(const char* name, int i) const
{
    glUniform1i(glGetUniformLocation(m_renderer_id, name), i);
}

void Program::set_vec3(const char* name, const glm::vec3& vec3) const
{
    glUniform3fv(glGetUniformLocation(m_renderer_id, name), 1, glm::value_ptr(vec3));
}

void Program::set_vec4(const char* name, const glm::vec4& vec4) const
{
    glUniform4fv(glGetUniformLocation(m_renderer_id, name), 1, glm::value_ptr(vec4));
}

void Program::set_mat4(const char* name, const glm::mat4& mat4) const
{
    glUniformMatrix4fv(glGetUniformLocation(m_renderer_id, name), 1, GL_FALSE, glm::value_ptr(mat4));
}

Program::Program(uint renderer_id) :
    m_renderer_id(renderer_id)
{
}

std::optional<uint> Program::create_program(const Shader& vert, const Shader& frag)
{
    uint program = glCreateProgram();
    vert.attach(program);
    frag.attach(program);
    glLinkProgram(program);
    vert.detach(program);
    frag.detach(program);
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
