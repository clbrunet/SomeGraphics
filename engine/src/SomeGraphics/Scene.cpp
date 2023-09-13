#include "SomeGraphics/Scene.hpp"
#include "SomeGraphics/Entity.hpp"

namespace sg {

const std::shared_ptr<Entity>& Scene::root() const
{
    return m_root;
}

void Scene::add_entity(std::shared_ptr<Entity>&& entity)
{
    m_root->add_child(std::move(entity));
}

}
