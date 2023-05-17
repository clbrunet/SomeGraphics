#include "Editor/EditorApplication.hpp"
#include "SomeGraphics.hpp"

namespace sg {

EditorApplication::EditorApplication(const std::string& name) :
    Application(name)
{
}

EditorApplication::~EditorApplication()
{
}

}

std::unique_ptr<sg::Application> create_app()
{
    return std::make_unique<sg::EditorApplication>("SomeGraphics Editor");
}
