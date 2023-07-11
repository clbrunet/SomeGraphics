#pragma once

#include <memory>

#include "SomeGraphics/SceneEntity.hpp"

namespace sg {

class SceneEntity;

class Scene {
public:
    Scene() = default;
    Scene(Scene&& other) = default;
    Scene(const Scene& other) = delete;
    Scene& operator=(Scene&& other) = default;
    Scene& operator=(const Scene& other) = delete;
    ~Scene() = default;

    const std::shared_ptr<SceneEntity>& root_entity() const;
    void add_entity(std::shared_ptr<SceneEntity>&& entity);
private:
    std::shared_ptr<SceneEntity> m_root_entity = SceneEntity::create_scene_root();
};

}
