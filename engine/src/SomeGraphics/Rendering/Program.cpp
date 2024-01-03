#include <algorithm>
#include <cstdint>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

#include <glm/gtc/type_ptr.hpp>

#include "SomeGraphics/Rendering/Program.hpp"
#include "SomeGraphics/Rendering/Shader.hpp"
#include "SomeGraphics/Rendering/Texture.hpp"

namespace sg {

std::optional<Program> Program::create(const char* vert_filename, const char* frag_filename)
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
    std::optional<uint32_t> program = create_program(*vert, *frag);
    if (!program.has_value()) {
        return std::nullopt;
    }
    return Program(program.value());
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

void Program::set_bool(const char* name, bool b) const
{
    glUniform1i(glGetUniformLocation(m_renderer_id, name), (int)b);
}

void Program::set_int(const char* name, int i) const
{
    glUniform1i(glGetUniformLocation(m_renderer_id, name), i);
}

void Program::set_ints(const char* name, std::span<const int> ints) const
{
    glUniform1iv(glGetUniformLocation(m_renderer_id, name), (int)ints.size(), ints.data());
}

void Program::set_uint(const char* name, unsigned u) const
{
    glUniform1ui(glGetUniformLocation(m_renderer_id, name), u);
}

void Program::set_float(const char* name, float f) const
{
    glUniform1f(glGetUniformLocation(m_renderer_id, name), f);
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
    glUniformMatrix4fv(glGetUniformLocation(m_renderer_id, name),
        1, GL_FALSE, glm::value_ptr(mat4));
}

void Program::print_uniform_block_layout(const char* name) const
{
    uint32_t index = glGetUniformBlockIndex(m_renderer_id, name);
    int binding;
    int size;
    glGetActiveUniformBlockiv(m_renderer_id, index, GL_UNIFORM_BLOCK_BINDING, &binding);
    glGetActiveUniformBlockiv(m_renderer_id, index, GL_UNIFORM_BLOCK_DATA_SIZE, &size);
    std::cout << "Uniform block '" << name << "' :\n";
    std::cout << "  index : " << index << '\n';
    std::cout << "  binding : " << binding << '\n';
    std::cout << "  size : " << size << '\n';
    int members_count;
    glGetActiveUniformBlockiv(m_renderer_id, index,
        GL_UNIFORM_BLOCK_ACTIVE_UNIFORMS, &members_count);
    if (members_count == 0) {
        std::cout << std::flush;
        return;
    }
    std::vector<uint32_t> indices((size_t)members_count);
    glGetActiveUniformBlockiv(m_renderer_id, index,
        GL_UNIFORM_BLOCK_ACTIVE_UNIFORM_INDICES, (int*)indices.data());
    std::vector<int> offsets((size_t)members_count);
    std::vector<int> sizes((size_t)members_count);
    std::vector<int> names_lengths((size_t)members_count);
    glGetActiveUniformsiv(m_renderer_id, members_count,
        indices.data(), GL_UNIFORM_OFFSET, offsets.data());
    glGetActiveUniformsiv(m_renderer_id, members_count,
        indices.data(), GL_UNIFORM_SIZE, sizes.data());
    glGetActiveUniformsiv(m_renderer_id, members_count,
        indices.data(), GL_UNIFORM_NAME_LENGTH, names_lengths.data());
    int maximum_name_length = *std::max_element(names_lengths.begin(), names_lengths.end());
    std::unique_ptr<char[]> member_name = std::make_unique<char[]>((size_t)maximum_name_length);
    std::cout << "  members :\n";
    for (uint32_t i = 0; i < (uint32_t)members_count; i++) {
        glGetActiveUniformName(m_renderer_id, indices[i],
            maximum_name_length, NULL, member_name.get());
        std::cout << "    " << member_name.get() << " :\n";
        std::cout << "      index : " << indices[i] << '\n';
        std::cout << "      offset : " << offsets[i] << '\n';
        std::cout << "      size(units of the type) : " << sizes[i] << '\n';
    }
    std::cout << std::flush;
}

Program::Program(uint32_t renderer_id) :
    m_renderer_id(renderer_id)
{
}

std::optional<uint32_t> Program::create_program(const Shader& vert, const Shader& frag)
{
    uint32_t program = glCreateProgram();
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
    std::vector<char> info_log((size_t)info_log_length);
    glGetProgramInfoLog(program, info_log_length, nullptr, info_log.data());
    std::clog << "Program link error :\n";
    for (char c : info_log) {
        std::clog << c;
    }
    std::clog << std::flush;
    return std::nullopt;
}

}
