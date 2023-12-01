#include <array>
#include <cstdint>
#include <cstdlib>
#include <glm/ext/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/string_cast.hpp>
#include <iostream>
#include <string>
#include <vector>

#include <glm/ext/vector_int2.hpp>
#include <glm/common.hpp>
#include <glad/gl.h>

#include "SomeGraphics/Rendering/DepthFrameBuffer.hpp"
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
#include "SomeGraphics/ResourcesCache.hpp"

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
    glEnable(GL_CULL_FACE);
    for (std::unique_ptr<DepthFrameBuffer>& frame_buffer : m_shadow_pass_frame_buffers) {
        frame_buffer = std::make_unique<DepthFrameBuffer>(
            DepthFrameBuffer::create_cubemap(glm::ivec2(1024, 1024)));
    }
    std::optional<Program> program_opt
        = Program::create("engine/assets/shaders/shadow_mapping.vert",
            "engine/assets/shaders/shadow_mapping.frag");
    if (!program_opt.has_value()) {
        std::abort();
    }
    m_shadow_mapping_program = std::make_unique<Program>(std::move(program_opt.value()));
}

void Renderer::set_viewport(glm::ivec2 dimensions) const
{
    glViewport(0, 0, dimensions.x, dimensions.y);
}

void Renderer::clear() const
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::set_clear_color(float red, float green, float blue, float opacity) const
{
    glClearColor(red, green, blue, opacity);
}

void Renderer::draw(const Scene& scene, const Camera& camera, glm::ivec2 viewport_dimensions) const
{
    m_is_shadow_pass = true;
    set_viewport(glm::ivec2(1024, 1024));
    m_shadow_mapping_program->use();
    int frame_buffer_renderer_id;
    glGetIntegerv(GL_FRAMEBUFFER_BINDING, &frame_buffer_renderer_id);
    const std::vector<std::shared_ptr<Entity>>& lights = scene.lights();
    uint8_t shadow_maps_count = glm::min(lights.size(), m_shadow_pass_frame_buffers.size());
    for (uint8_t i = 0; i < shadow_maps_count; i++) {
        m_shadow_pass_light = lights[i].get();
        m_shadow_pass_frame_buffer = m_shadow_pass_frame_buffers[i].get();
        m_shadow_pass_frame_buffer->bind();
        for (CubemapFace face = CubemapFace::Begin;
            face < CubemapFace::End; face = CubemapFace(face + 1)) {
            m_shadow_pass_frame_buffer->attach_face(face);
            glClear(GL_DEPTH_BUFFER_BIT);
        }
        draw(*scene.root(), scene, camera);
    }
    m_is_shadow_pass = false;
    set_viewport(viewport_dimensions);
    uint8_t lights_count = glm::min(lights.size(), (size_t)MAX_LIGHTS_COUNT);
    GlobalsUniformBlockData globals = {
        .view_projection = camera.view_projection(),
        .camera_position = camera.position(),
        .lights_count = lights_count,
        .lights = {},
        .shadow_maps_count = shadow_maps_count,
    };
    for (uint8_t i = 0; i < lights_count; i++) {
        const std::shared_ptr<Entity>& light = lights[i];
        globals.lights[i] = {
            .position = light->model_matrix()[3],
            .hdr_color = light->light()->color * light->light()->intensity,
        };
    }
    m_globals_uniform_buffer.update_data<GlobalsUniformBlockData>(globals);
    for (uint8_t i = 0; i < m_shadow_pass_frame_buffers.size(); i++) {
        m_shadow_pass_frame_buffers[i]->depth_texture().bind_to_unit(15 - i);
    }
    glBindFramebuffer(GL_FRAMEBUFFER, frame_buffer_renderer_id);
    draw(*scene.root(), scene, camera);
}

void Renderer::draw(const Skybox& skybox, const Camera& camera) const
{
    glDepthMask(GL_FALSE);
    glDepthFunc(GL_LEQUAL);
    skybox.program()->use();
    skybox.program()->set_mat4("u_view", camera.view());
    skybox.program()->set_mat4("u_projection", camera.projection());
    skybox.program()->set_int("u_skybox", 0);
    skybox.cubemap()->bind_to_unit(0);

    draw(*skybox.vertex_array());
    glDepthFunc(GL_LESS);
    glDepthMask(GL_TRUE);
}

void Renderer::post_process(const PostProcess& post_process, const Texture& texture) const
{
    glDisable(GL_DEPTH_TEST);
    post_process.program()->use();
    post_process.program()->set_int("u_texture", 0);
    texture.bind_to_unit(0);
    draw(*post_process.vertex_array());
    glEnable(GL_DEPTH_TEST);
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
    if (entity.mesh()) {
        draw(*entity.mesh(), entity.model_matrix());
    }
    for (const std::shared_ptr<Entity>& child : entity.children()) {
        draw(*child, scene, camera);
    }
}

void Renderer::draw(const Mesh& mesh, const glm::mat4& model_matrix) const
{
    const VertexArray& vertex_array = *mesh.vertex_array();
    vertex_array.bind();
    GLenum index_buffer_format = vertex_array.index_buffer()->format();
    for (const SubMeshInfo& sub_mesh_info : mesh.sub_meshes_info()) {
        if (m_is_shadow_pass) {
            glm::vec3 light_position = m_shadow_pass_light->model_matrix()[3];
            std::array<glm::mat4, 6> views = {
                glm::lookAt(light_position, light_position
                    + glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)),
                glm::lookAt(light_position, light_position
                    + glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)),
                glm::lookAt(light_position, light_position
                    + glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f)),
                glm::lookAt(light_position, light_position
                    + glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f)),
                glm::lookAt(light_position, light_position
                    + glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, -1.0f, 0.0f)),
                glm::lookAt(light_position, light_position
                    + glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, -1.0f, 0.0f)),
            };
            for (CubemapFace face = CubemapFace::Begin;
                face < CubemapFace::End; face = CubemapFace(face + 1)) {
                m_shadow_pass_frame_buffer->attach_face(face);
                glm::mat4 projection = glm::perspective(glm::radians(90.0f), 1.0f, 0.01f, 30.0f);
                m_shadow_mapping_program->set_mat4("u_view_projection", projection * views[face]);
                m_shadow_mapping_program->set_mat4("u_model", model_matrix);
                m_shadow_mapping_program->set_vec3("u_light_position", light_position);
                glDrawElementsBaseVertex(GL_TRIANGLES, sub_mesh_info.indices_count(), index_buffer_format,
                    sub_mesh_info.index_buffer_offset(), sub_mesh_info.vertices_offset());
            }
        } else {
            const std::shared_ptr<Program>& program = sub_mesh_info.material()->program();
            program->use();
            program->set_mat4("u_model", model_matrix);
            for (uint8_t i = 0; i < m_shadow_pass_frame_buffers.size(); i++) {
                program->set_int(("u_shadow_maps[" + std::to_string(i) + "]").c_str(), 15 - i);
            }
            sub_mesh_info.material()->set_program_data();
            glDrawElementsBaseVertex(GL_TRIANGLES, sub_mesh_info.indices_count(), index_buffer_format,
                sub_mesh_info.index_buffer_offset(), sub_mesh_info.vertices_offset());
        }
    }
}

void Renderer::draw(const VertexArray& vertex_array) const
{
    vertex_array.bind();
    glDrawElements(GL_TRIANGLES, vertex_array.index_buffer()->count(),
        vertex_array.index_buffer()->format(), 0);
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
