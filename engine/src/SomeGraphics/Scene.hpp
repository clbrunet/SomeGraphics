#pragma once

#include <memory>

#include "SomeGraphics/Entity.hpp"

namespace sg {

class Entity;

class Scene {
public:
    Scene() = default;
    Scene(Scene&& other) = default;
    Scene(const Scene& other) = delete;
    Scene& operator=(Scene&& other) = default;
    Scene& operator=(const Scene& other) = delete;
    ~Scene() = default;

    const std::shared_ptr<Entity>& root() const;
    void add_entity(std::shared_ptr<Entity>&& entity);
private:
    std::shared_ptr<Entity> m_root = Entity::create_scene_root();
};

}
