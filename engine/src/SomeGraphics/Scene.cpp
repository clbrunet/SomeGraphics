#include <algorithm>

#include "SomeGraphics/Scene.hpp"
#include "SomeGraphics/Entity.hpp"
#include "SomeGraphics/Window.hpp"
#include "SomeGraphics/Animation.hpp"

namespace sg {

const std::shared_ptr<Entity>& Scene::root() const
{
    return m_root;
}

void Scene::add_entity(std::shared_ptr<Entity> entity)
{
    m_root->add_child(std::move(entity));
}

std::shared_ptr<Entity> Scene::add_light(std::string name, std::shared_ptr<Entity> parent)
{
    std::shared_ptr<Entity> light = std::make_shared<Entity>(name, parent);
    light->set_light(std::make_unique<Light>(glm::vec3(1.0f), 3.0f));
    parent->add_child(light);
    m_lights.push_back(light);
    return light;
}

void Scene::on_update(float delta_time)
{
    for (Animation& animation : animations) {
        animation.on_update(delta_time);
    }
}

const std::vector<std::shared_ptr<Entity>>& Scene::lights() const
{
    auto expired = std::remove_if(m_lights.begin(), m_lights.end(),
        [](const std::shared_ptr<Entity>& light) {
            return light.use_count() < 2 || !light->light();
        });
    m_lights.erase(expired, m_lights.end());
    return m_lights;
}

}
