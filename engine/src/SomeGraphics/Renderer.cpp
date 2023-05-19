#include <glad/gl.h>

#include "SomeGraphics/Renderer.hpp"

namespace sg {

Renderer::Renderer()
{
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
