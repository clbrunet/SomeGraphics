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
#include "SomeGraphics/Node.hpp"
#include "SomeGraphics/Skybox.hpp"
#include "SomeGraphics/PostProcess.hpp"
#include "SomeGraphics/Mesh.hpp"
#include "SomeGraphics/Light.hpp"
#include "SomeGraphics/ResourcesCache.hpp"

namespace sg {

Renderer::Renderer() :
    m_globals_uniform_buffer(0, sizeof(GlobalsUniformBlockData)),
    m_mesh_info_uniform_buffer(1, sizeof(MeshInfoUniformBlockData))
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
        GLuint id = 131185;
        glDebugMessageControl(GL_DEBUG_SOURCE_API, GL_DEBUG_TYPE_OTHER, GL_DONT_CARE,
            1, &id, GL_FALSE);
        id = 131218;
        glDebugMessageControl(GL_DEBUG_SOURCE_API, GL_DEBUG_TYPE_PERFORMANCE, GL_DONT_CARE,
            1, &id, GL_FALSE);
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

void Renderer::render(const Scene& scene, const Camera& camera,
    glm::ivec2 viewport_dimensions) const
{
    int frame_buffer_renderer_id;
    glGetIntegerv(GL_FRAMEBUFFER_BINDING, &frame_buffer_renderer_id);
    auto lights = scene.registry.view<Light>();
    m_shadow_maps_count = (uint8_t)glm::min(lights.size(), m_shadow_pass_frame_buffers.size());
    for (uint8_t i = 0; i < m_shadow_maps_count; i++) {
        const DepthFrameBuffer& frame_buffer = *m_shadow_pass_frame_buffers[i];
        frame_buffer.bind();
        for (CubemapFace face = CubemapFace::Begin;
            face < CubemapFace::End; face = CubemapFace(face + 1)) {
            frame_buffer.attach_face(face);
            glClear(GL_DEPTH_BUFFER_BIT);
        }
        glm::vec3 light_position = scene.registry.get<Node>(lights[i]).model_matrix()[3];
        m_shadow_map_infos[i].light_position = light_position;
        static const glm::mat4 projection
            = glm::perspective(glm::radians(90.0f), 1.0f, 0.01f, 30.0f);
        m_shadow_map_infos[i].view_projections = {
            projection * glm::lookAt(light_position, light_position
                + glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)),
            projection * glm::lookAt(light_position, light_position
                + glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)),
            projection * glm::lookAt(light_position, light_position
                + glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f)),
            projection * glm::lookAt(light_position, light_position
                + glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f)),
            projection * glm::lookAt(light_position, light_position
                + glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, -1.0f, 0.0f)),
            projection * glm::lookAt(light_position, light_position
                + glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, -1.0f, 0.0f)),
        };
    }
    set_viewport(glm::ivec2(1024, 1024));
    m_shadow_mapping_program->use();
    render<RenderPass::Shadow>(scene.root().entity(), scene, camera);
    set_viewport(viewport_dimensions);
    uint8_t lights_count = (uint8_t)glm::min(lights.size(), (size_t)MAX_LIGHTS_COUNT);
    GlobalsUniformBlockData globals = {
        .view_projection = camera.view_projection(),
        .camera_position = camera.position(),
        .lights_count = lights_count,
        .lights = {},
        .shadow_maps_count = m_shadow_maps_count,
    };
    for (uint8_t i = 0; i < lights_count; i++) {
        auto [light] = lights.get(lights[i]);
        globals.lights[i] = {
            .position = scene.registry.get<Node>(lights[i]).model_matrix()[3],
            .hdr_color =  light.color * light.intensity,
        };
    }
    m_globals_uniform_buffer.update_data(globals);
    for (uint8_t i = 0; i < m_shadow_pass_frame_buffers.size(); i++) {
        m_shadow_pass_frame_buffers[i]->depth_texture().bind_to_unit(15 - i);
    }
    glBindFramebuffer(GL_FRAMEBUFFER, frame_buffer_renderer_id);
    render<RenderPass::Shading>(scene.root().entity(), scene, camera);
}

void Renderer::render(const Skybox& skybox, const Camera& camera) const
{
    glDepthMask(GL_FALSE);
    glDepthFunc(GL_LEQUAL);
    skybox.program()->use();
    skybox.program()->set_mat4("u_view", camera.view());
    skybox.program()->set_mat4("u_projection", camera.projection());
    skybox.program()->set_int("u_skybox", 0);
    skybox.cubemap()->bind_to_unit(0);
    render(*skybox.vertex_array());
    glDepthFunc(GL_LESS);
    glDepthMask(GL_TRUE);
}

void Renderer::post_process(const PostProcess& post_process, const Texture& texture) const
{
    glDisable(GL_DEPTH_TEST);
    post_process.program()->use();
    post_process.program()->set_int("u_texture", 0);
    texture.bind_to_unit(0);
    render(*post_process.vertex_array());
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

void Renderer::render(const VertexArray& vertex_array) const
{
    vertex_array.bind();
    glDrawElements(GL_TRIANGLES, vertex_array.index_buffer()->count(),
        vertex_array.index_buffer()->format(), 0);
}

void GLAPIENTRY Renderer::gl_debug_message_callback(GLenum source, GLenum type,
    [[maybe_unused]] GLuint id, GLenum severity, [[maybe_unused]] GLsizei length,
    const GLchar* message, [[maybe_unused]] const void* user_param)
{
    std::clog << std::hex;
    std::clog << "OpenGL debug message :\n";
    std::clog << "Source : ";
    switch (source) {
        case GL_DEBUG_SOURCE_API:
            std::clog << "API";
            break;
        case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
            std::clog << "window system";
            break;
        case GL_DEBUG_SOURCE_SHADER_COMPILER:
            std::clog << "shader compiler";
            break;
        case GL_DEBUG_SOURCE_THIRD_PARTY:
            std::clog << "third party";
            break;
        case GL_DEBUG_SOURCE_APPLICATION:
            std::clog << "application";
            break;
        default:
            std::clog << source;
    }
    std::clog << "\nType : ";
    switch (type) {
        case GL_DEBUG_TYPE_ERROR:
            std::clog << "error";
            break;
        case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
            std::clog << "deprecated behavior";
            break;
        case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
            std::clog << "undefined behavior";
            break;
        case GL_DEBUG_TYPE_PORTABILITY:
            std::clog << "portability";
            break;
        case GL_DEBUG_TYPE_PERFORMANCE:
            std::clog << "performance";
            break;
        case GL_DEBUG_TYPE_MARKER:
            std::clog << "marker";
            break;
        case GL_DEBUG_TYPE_PUSH_GROUP:
            std::clog << "push group";
            break;
        case GL_DEBUG_TYPE_POP_GROUP:
            std::clog << "pop group";
            break;
        case GL_DEBUG_TYPE_OTHER:
            std::clog << "other";
            break;
        default:
            std::clog << type;
    }
    std::clog << "\nSeverity : ";
    switch (severity) {
        case GL_DEBUG_SEVERITY_HIGH:
            std::clog << "high";
            break;
        case GL_DEBUG_SEVERITY_MEDIUM:
            std::clog << "medium";
            break;
        case GL_DEBUG_SEVERITY_LOW:
            std::clog << "low";
            break;
        case GL_DEBUG_SEVERITY_NOTIFICATION:
            std::clog << "notification";
            break;
        default:
            std::clog << severity;
    }
    std::clog << "\nMessage :\n" << message << std::endl;
    std::clog << std::dec;
}

}
