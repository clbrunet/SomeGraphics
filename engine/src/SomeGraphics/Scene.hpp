#pragma once

#include <memory>
#include <vector>

#include "SomeGraphics/Entity.hpp"

namespace sg {

class Animation;
class Window;

class Scene {
public:
    Scene() = default;
    Scene(Scene&& other) = default;
    Scene(const Scene& other) = delete;
    Scene& operator=(Scene&& other) = default;
    Scene& operator=(const Scene& other) = delete;
    ~Scene() = default;

    const std::shared_ptr<Entity>& root() const;
    void add_entity(std::shared_ptr<Entity> entity);
    std::shared_ptr<Entity> add_light(std::string name, std::shared_ptr<Entity> parent);

    void on_update(float delta_time);

    const std::vector<std::shared_ptr<Entity>>& lights() const;
    std::vector<Animation> animations;
private:
    std::shared_ptr<Entity> m_root = Entity::create_scene_root(*this);
    mutable std::vector<std::shared_ptr<Entity>> m_lights;
};

}
