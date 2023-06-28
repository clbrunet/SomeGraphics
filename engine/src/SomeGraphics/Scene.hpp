#pragma once

#include <memory>
#include <vector>

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

    const std::vector<std::shared_ptr<SceneEntity>>& entities() const;
    void add_entity(const std::shared_ptr<SceneEntity>& entity);
private:
    std::vector<std::shared_ptr<SceneEntity>> m_entities;
};

}
