#include "Editor/EditorApplication.hpp"
#include "SomeGraphics.hpp"

std::unique_ptr<sg::Application> create_app()
{
    return std::make_unique<sg::EditorApplication>("SomeGraphics Editor");
}

namespace sg {

EditorApplication::EditorApplication(const std::string& name) :
    Application(name)
{
    m_renderer.set_clear_color(0.0f, 0.5f, 0.0f, 1.0f);
}

EditorApplication::~EditorApplication()
{
}

void EditorApplication::on_update()
{
}

void EditorApplication::on_render()
{
    m_renderer.clear();
}

}

