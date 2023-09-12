#pragma once

#include "glm/ext/vector_int2.hpp"
#include "glad/gl.h"
#include "glm/ext/matrix_float4x4.hpp"

namespace sg {

class Camera;
class Scene;
class Skybox;
class Mesh;
class VertexArray;
class PostProcess;
class Texture;
class SceneEntity;

class Renderer {
public:
    Renderer();
    Renderer(Renderer&& other) = default;
    Renderer(const Renderer& other) = default;
    Renderer& operator=(Renderer&& other) = default;
    Renderer& operator=(const Renderer& other) = default;
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
    void draw(const SceneEntity& entity, const glm::mat4& parent_transform, const Camera& camera) const;

    static void GLAPIENTRY gl_debug_message_callback(GLenum source, GLenum type, GLuint id,
        GLenum severity, GLsizei length, const GLchar* message, const void* user_param);
};

}
