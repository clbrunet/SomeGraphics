#include <iostream>

#include <glad/gl.h>

#include "SomeGraphics/Renderer.hpp"

namespace sg {

static void GLAPIENTRY gl_debug_message_callback(
    GLenum source,
    GLenum type,
    GLuint id,
    GLenum severity,
    GLsizei length,
    const GLchar* message,
    const void* user_param
)
{
    static_cast<void>(length);
    static_cast<void>(user_param);
    std::cout << "OpenGL debug message :\nSource : " << source << ", type : " << type
        << ", id : " << id << ", severity : " << severity << '\n' << message << std::endl;
}

Renderer::Renderer()
{
#if SG_DEBUG
    GLint flags;
    glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
    if (flags & GL_CONTEXT_FLAG_DEBUG_BIT)
    {
        glEnable(GL_DEBUG_OUTPUT);
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
        glDebugMessageCallback(gl_debug_message_callback, nullptr);
    }
#endif
}

Renderer::~Renderer()
{
}

void Renderer::clear() const
{
    glClear(GL_COLOR_BUFFER_BIT);
}

void Renderer::set_clear_color(float red, float green, float blue, float opacity)
{
    glClearColor(red, green, blue, opacity);
}

}
