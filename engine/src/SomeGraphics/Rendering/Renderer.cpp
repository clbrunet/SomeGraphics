#include <cstdint>
#include <iostream>
#include <vector>

#include <glm/ext/vector_int2.hpp>
#include <glm/common.hpp>
#include <glad/gl.h>

#include "SomeGraphics/Rendering/Renderer.hpp"
#include "SomeGraphics/Rendering/Program.hpp"
#include "SomeGraphics/Rendering/Texture.hpp"
#include "SomeGraphics/Rendering/VertexArray.hpp"
#include "SomeGraphics/Rendering/Material.hpp"
#include "SomeGraphics/Camera.hpp"
#include "SomeGraphics/Scene.hpp"
#include "SomeGraphics/Skybox.hpp"
#include "SomeGraphics/PostProcess.hpp"
#include "SomeGraphics/Mesh.hpp"

namespace sg {

Renderer::Renderer() :
    m_globals_uniform_buffer(0, sizeof(GlobalsUniformBlockData))
{
#if SG_DEBUG
    GLint flags;
    glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
    if (flags & GL_CONTEXT_FLAG_DEBUG_BIT)
    {
        glEnable(GL_DEBUG_OUTPUT);
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
        glDebugMessageCallback(gl_debug_message_callback, nullptr);
        glDebugMessageControl(GL_DEBUG_SOURCE_SHADER_COMPILER, GL_DONT_CARE, GL_DONT_CARE,
            0, nullptr, GL_FALSE);
    }
#endif
    glEnable(GL_DEPTH_TEST);
}

void Renderer::set_viewport(glm::ivec2 dimension) const
{
    glViewport(0, 0, dimension.x, dimension.y);
}

void Renderer::clear() const
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::set_clear_color(float red, float green, float blue, float opacity) const
{
    glClearColor(red, green, blue, opacity);
}

void Renderer::draw(const Scene& scene, const Camera& camera) const
{
    const std::vector<std::shared_ptr<Entity>>& lights = scene.lights();
    uint8_t lights_count = glm::min(lights.size(), (size_t)32);
    GlobalsUniformBlockData globals = {
        .view_projection = camera.view_projection(),
        .camera_position = camera.position(),
        .lights_count = lights_count,
        .lights = {},
    };
    for (uint8_t i = 0; i < lights_count; i++) {
        const std::shared_ptr<Entity>& light = lights[i];
        globals.lights[i] = {
            .position = light->model_matrix()[3],
            .hdr_color = light->light()->color * light->light()->intensity,
        };
    }
    m_globals_uniform_buffer.update_data<GlobalsUniformBlockData>(globals);
    draw(*scene.root(), scene, camera);
}

void Renderer::draw(const Mesh& mesh) const
{
    draw(*mesh.vertex_array());
}

void Renderer::draw(const VertexArray& vertex_array) const
{
    vertex_array.bind();
    glDrawElements(GL_TRIANGLES, vertex_array.index_buffer()->count(),
        vertex_array.index_buffer()->format(), 0);
}

void Renderer::draw(const Skybox& skybox, const Camera& camera) const
{
    glDepthMask(GL_FALSE);
    glDepthFunc(GL_LEQUAL);
    skybox.program()->use();
    skybox.program()->set_mat4("u_view", camera.view());
    skybox.program()->set_mat4("u_projection", camera.projection());
    skybox.program()->set_texture("u_skybox", 0, *skybox.cubemap());
    draw(*skybox.mesh());
    glDepthFunc(GL_LESS);
    glDepthMask(GL_TRUE);
}

void Renderer::post_process(const PostProcess& post_process, const Texture& texture) const
{
    post_process.program()->use();
    post_process.program()->set_texture("u_texture", 0, texture);
    draw(*post_process.quad());
}

void Renderer::set_framebuffer_srbg(bool state) const
{
    if (state) {
        glEnable(GL_FRAMEBUFFER_SRGB);
    } else {
        glDisable(GL_FRAMEBUFFER_SRGB);
    }
}

void Renderer::draw(const Entity& entity, const Scene& scene, const Camera& camera) const
{
    if (entity.mesh() && entity.material()) {
        const std::shared_ptr<Program> program = entity.material()->program();
        program->use();
        program->set_mat4("u_model", entity.model_matrix());
        entity.material()->set_program_data();
        draw(*entity.mesh());
    }
    for (const std::shared_ptr<Entity>& child : entity.children()) {
        draw(*child, scene, camera);
    }
}

void GLAPIENTRY Renderer::gl_debug_message_callback(GLenum source, GLenum type,
    [[maybe_unused]] GLuint id, GLenum severity, [[maybe_unused]] GLsizei length,
    const GLchar* message, [[maybe_unused]] const void* user_param)
{
    std::cerr << std::hex;
    std::cerr << "OpenGL debug message :\n";
    std::cerr << "Source : ";
    switch (source) {
        case GL_DEBUG_SOURCE_API:
            std::cerr << "API";
            break;
        case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
            std::cerr << "window system";
            break;
        case GL_DEBUG_SOURCE_SHADER_COMPILER:
            std::cerr << "shader compiler";
            break;
        case GL_DEBUG_SOURCE_THIRD_PARTY:
            std::cerr << "third party";
            break;
        case GL_DEBUG_SOURCE_APPLICATION:
            std::cerr << "application";
            break;
        default:
            std::cerr << source;
    }
    std::cerr << "\nType : ";
    switch (type) {
        case GL_DEBUG_TYPE_ERROR:
            std::cerr << "error";
            break;
        case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
            std::cerr << "deprecated behavior";
            break;
        case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
            std::cerr << "undefined behavior";
            break;
        case GL_DEBUG_TYPE_PORTABILITY:
            std::cerr << "portability";
            break;
        case GL_DEBUG_TYPE_PERFORMANCE:
            std::cerr << "performance";
            break;
        case GL_DEBUG_TYPE_MARKER:
            std::cerr << "marker";
            break;
        case GL_DEBUG_TYPE_PUSH_GROUP:
            std::cerr << "push group";
            break;
        case GL_DEBUG_TYPE_POP_GROUP:
            std::cerr << "pop group";
            break;
        case GL_DEBUG_TYPE_OTHER:
            std::cerr << "other";
            break;
        default:
            std::cerr << type;
    }
    std::cerr << "\nSeverity : ";
    switch (severity) {
        case GL_DEBUG_SEVERITY_HIGH:
            std::cerr << "high";
            break;
        case GL_DEBUG_SEVERITY_MEDIUM:
            std::cerr << "medium";
            break;
        case GL_DEBUG_SEVERITY_LOW:
            std::cerr << "low";
            break;
        case GL_DEBUG_SEVERITY_NOTIFICATION:
            std::cerr << "notification";
            break;
        default:
            std::cerr << severity;
    }
    std::cerr << "\nMessage :\n" << message << std::endl;
    std::cerr << std::dec;
}

}
