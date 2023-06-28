#include "SomeGraphics/Scene.hpp"

namespace sg {

const std::vector<std::shared_ptr<SceneEntity>>& Scene::entities() const
{
    return m_entities;
}

void Scene::add_entity(const std::shared_ptr<SceneEntity>& entity)
{
    m_entities.push_back(entity);
}

}
