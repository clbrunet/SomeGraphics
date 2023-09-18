#pragma once

#include <glm/ext/vector_int2.hpp>
#include <glad/gl.h>
#include <glm/ext/matrix_float4x4.hpp>

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
    Renderer(const Renderer& other) = default;
    Renderer& operator=(Renderer&& other) = default;
    Renderer& operator=(const Renderer& other) = default;
    ~Renderer() = default;

    void set_viewport(glm::ivec2 dimension) const;
    void clear() const;
    void set_clear_color(float red, float green, float blue, float opacity) const;
    void draw(const Scene& scene, const Camera& camera) const;
    void draw(const Entity& entity, const Scene& scene, const Camera& camera) const;
    void draw(const Mesh& mesh) const;
    void draw(const VertexArray& vertex_array) const;
    void draw(const Skybox& skybox, const Camera& camera) const;
    void post_process(const PostProcess& post_process, const Texture& texture) const;
    void set_framebuffer_srbg(bool state) const;
private:
    static void GLAPIENTRY gl_debug_message_callback(GLenum source, GLenum type, GLuint id,
        GLenum severity, GLsizei length, const GLchar* message, const void* user_param);

    constexpr static const char* light_position_locations[32] = {
        "u_lights[0].position", "u_lights[1].position", "u_lights[2].position",
        "u_lights[3].position", "u_lights[4].position", "u_lights[5].position",
        "u_lights[6].position", "u_lights[7].position", "u_lights[8].position",
        "u_lights[9].position", "u_lights[10].position", "u_lights[11].position",
        "u_lights[12].position", "u_lights[13].position", "u_lights[14].position",
        "u_lights[15].position", "u_lights[16].position", "u_lights[17].position",
        "u_lights[18].position", "u_lights[19].position", "u_lights[20].position",
        "u_lights[21].position", "u_lights[22].position", "u_lights[23].position",
        "u_lights[24].position", "u_lights[25].position", "u_lights[26].position",
        "u_lights[27].position", "u_lights[28].position", "u_lights[29].position",
        "u_lights[30].position", "u_lights[31].position"
    };
    constexpr static const char* light_hdr_color_locations[256] = {
        "u_lights[0].hdr_color", "u_lights[1].hdr_color", "u_lights[2].hdr_color",
        "u_lights[3].hdr_color", "u_lights[4].hdr_color", "u_lights[5].hdr_color",
        "u_lights[6].hdr_color", "u_lights[7].hdr_color", "u_lights[8].hdr_color",
        "u_lights[9].hdr_color", "u_lights[10].hdr_color", "u_lights[11].hdr_color",
        "u_lights[12].hdr_color", "u_lights[13].hdr_color", "u_lights[14].hdr_color",
        "u_lights[15].hdr_color", "u_lights[16].hdr_color", "u_lights[17].hdr_color",
        "u_lights[18].hdr_color", "u_lights[19].hdr_color", "u_lights[20].hdr_color",
        "u_lights[21].hdr_color", "u_lights[22].hdr_color", "u_lights[23].hdr_color",
        "u_lights[24].hdr_color", "u_lights[25].hdr_color", "u_lights[26].hdr_color",
        "u_lights[27].hdr_color", "u_lights[28].hdr_color", "u_lights[29].hdr_color",
        "u_lights[30].hdr_color", "u_lights[31].hdr_color"
    };
};

}
