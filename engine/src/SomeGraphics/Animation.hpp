#pragma once

#include <memory>
#include <vector>

#include <assimp/anim.h>
#include <entt/entt.hpp>
#include <glm/ext/vector_float3.hpp>
#include <glm/ext/quaternion_float.hpp>

namespace sg {

class Window;
class Node;

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
    entt::entity entity;
    std::vector<AnimationKey<glm::vec3>> positions;
    std::vector<AnimationKey<glm::quat>> rotations;
    std::vector<AnimationKey<glm::vec3>> scales;

    EntityAnimation(entt::entity p_entity, std::vector<AnimationKey<glm::vec3>> p_positions,
        std::vector<AnimationKey<glm::quat>> p_rotations,
        std::vector<AnimationKey<glm::vec3>> p_scales) :
        entity(p_entity),
        positions(std::move(p_positions)),
        rotations(std::move(p_rotations)),
        scales(std::move(p_scales))
    {
    }
};

class Animation {
public:
    static Animation from_ai_animation(const aiAnimation* ai_animation, const Node& asset_root);
    Animation() = delete;
    Animation(float duration, entt::registry& registry, std::vector<EntityAnimation> entities);
    Animation(Animation&& other) = default;
    Animation(const Animation& other) = default;
    Animation& operator=(Animation&& other) = default;
    Animation& operator=(const Animation& other) = default;
    ~Animation() = default;

    void update(float delta_time);
private:
    float m_duration;
    entt::registry& m_registry;
    std::vector<EntityAnimation> m_entities;
    float m_time = 0.0f;
};

}
