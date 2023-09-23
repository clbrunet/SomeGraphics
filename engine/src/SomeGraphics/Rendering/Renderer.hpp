#pragma once

#include <cstddef>
#include <cstdint>

#include <glm/ext/vector_int2.hpp>
#include <glad/gl.h>
#include <glm/ext/matrix_float4x4.hpp>

#include "SomeGraphics/Rendering/UniformBuffer.hpp"

namespace sg {

class Camera;
class Scene;
class Skybox;
class Mesh;
class VertexArray;
class PostProcess;
class Texture;
class Entity;

class Renderer {
public:
    Renderer();
    Renderer(Renderer&& other) = default;
    Renderer(const Renderer& other) = delete;
    Renderer& operator=(Renderer&& other) = default;
    Renderer& operator=(const Renderer& other) = delete;
    ~Renderer() = default;

    void set_viewport(glm::ivec2 dimension) const;
    void clear() const;
    void set_clear_color(float red, float green, float blue, float opacity) const;
    void draw(const Scene& scene, const Camera& camera) const;
    void draw(const Mesh& mesh) const;
    void draw(const VertexArray& vertex_array) const;
    void draw(const Skybox& skybox, const Camera& camera) const;
    void post_process(const PostProcess& post_process, const Texture& texture) const;
    void set_framebuffer_srbg(bool state) const;
private:
    UniformBuffer m_globals_uniform_buffer;

    void draw(const Entity& entity, const Scene& scene, const Camera& camera) const;

    static void GLAPIENTRY gl_debug_message_callback(GLenum source, GLenum type, GLuint id,
        GLenum severity, GLsizei length, const GLchar* message, const void* user_param);

    constexpr static uint32_t MAX_LIGHTS_COUNT = 32;
    struct LightUniformData {
        glm::vec3 position;
        int : 32;
        glm::vec3 hdr_color;
        int : 32;
    };
    static_assert(sizeof(LightUniformData) == 32);
    static_assert(offsetof(LightUniformData, position) == 0);
    static_assert(offsetof(LightUniformData, hdr_color) == 16);
    struct GlobalsUniformBlockData {
        glm::mat4 view_projection;
        glm::vec3 camera_position;
        unsigned lights_count;
        LightUniformData lights[MAX_LIGHTS_COUNT];
    };
    static_assert(sizeof(GlobalsUniformBlockData) == 1104);
    static_assert(offsetof(GlobalsUniformBlockData, view_projection) == 0);
    static_assert(offsetof(GlobalsUniformBlockData, camera_position) == 64);
    static_assert(offsetof(GlobalsUniformBlockData, lights_count) == 76);
    static_assert(offsetof(GlobalsUniformBlockData, lights[0]) == 80);
    static_assert(offsetof(GlobalsUniformBlockData, lights[1]) == 112);
};

}
