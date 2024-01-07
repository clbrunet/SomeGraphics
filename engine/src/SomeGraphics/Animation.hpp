#pragma once

#include <memory>
#include <vector>

#include <glm/ext/vector_float3.hpp>
#include <glm/ext/quaternion_float.hpp>
#include <assimp/anim.h>

namespace sg {

class Entity;
class Window;

template<typename Value>
struct AnimationKey {
    float time;
    Value value;

    AnimationKey(float p_time, const Value& p_value) :
        time(p_time),
        value(p_value)
    {
    }
};

struct EntityAnimation {
    std::weak_ptr<Entity> entity;
    std::vector<AnimationKey<glm::vec3>> positions;
    std::vector<AnimationKey<glm::quat>> rotations;
    std::vector<AnimationKey<glm::vec3>> scales;

    EntityAnimation(std::weak_ptr<Entity> p_entity, std::vector<AnimationKey<glm::vec3>> p_positions,
        std::vector<AnimationKey<glm::quat>> p_rotations,
        std::vector<AnimationKey<glm::vec3>> p_scales) :
        entity(std::move(p_entity)),
        positions(std::move(p_positions)),
        rotations(std::move(p_rotations)),
        scales(std::move(p_scales))
    {
    }
};

class Animation {
public:
    static Animation from_ai_animation(const aiAnimation* ai_animation,
        const std::shared_ptr<Entity>& asset_root);
    Animation() = delete;
    Animation(float duration, std::vector<EntityAnimation> entities);
    Animation(Animation&& other) = default;
    Animation(const Animation& other) = default;
    Animation& operator=(Animation&& other) = default;
    Animation& operator=(const Animation& other) = default;
    ~Animation() = default;

    void on_update(float delta_time);
private:
    float m_duration;
    std::vector<EntityAnimation> m_entities;
    float m_time = 0.0f;
};

}
