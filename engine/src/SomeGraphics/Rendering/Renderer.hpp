#pragma once

#include "glm/ext/vector_int2.hpp"
#include "glad/gl.h"

namespace sg {

class Model;
class Mesh;
class VertexArray;

class Renderer {
public:
    Renderer();
    ~Renderer();

    void set_viewport(glm::ivec2 dimension) const;
    void clear() const;
    void set_clear_color(float red, float green, float blue, float opacity) const;
    void draw(const Model& model) const;
    void draw(const Mesh& mesh) const;
    void draw(const VertexArray& vertex_array) const;
private:
    static void GLAPIENTRY gl_debug_message_callback(GLenum source, GLenum type, GLuint id,
        GLenum severity, GLsizei length, const GLchar* message, const void* user_param);
};

}
