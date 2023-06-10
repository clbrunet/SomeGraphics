#pragma once

#include "glad/gl.h"

namespace sg {

class VertexArray;

class Renderer {
public:
    Renderer();
    ~Renderer();
    void clear() const;
    void set_clear_color(float red, float green, float blue, float opacity);
    void draw(const VertexArray& vertex_array) const;
private:
    static void GLAPIENTRY gl_debug_message_callback(GLenum source, GLenum type, GLuint id,
        GLenum severity, GLsizei length, const GLchar* message, const void* user_param);
};

}
