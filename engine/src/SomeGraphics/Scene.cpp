#include "SomeGraphics/Scene.hpp"
#include "SomeGraphics/SceneEntity.hpp"

namespace sg {

const std::shared_ptr<SceneEntity>& Scene::root_entity() const
{
    return m_root_entity;
}

void Scene::add_entity(std::shared_ptr<SceneEntity>&& entity)
{
    m_root_entity->add_child(std::move(entity));
}

}
