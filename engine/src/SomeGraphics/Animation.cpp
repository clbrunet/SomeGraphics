#include <algorithm>
#include <glm/ext/quaternion_common.hpp>
#include <span>

#include "SomeGraphics/Animation.hpp"
#include "SomeGraphics/Entity.hpp"
#include "SomeGraphics/AssimpHelper.hpp"
#include "SomeGraphics/Window.hpp"

namespace sg {

Animation Animation::from_ai_animation(const aiAnimation* ai_animation,
        const std::shared_ptr<Entity>& asset_root)
{
    std::vector<EntityAnimation> entities;
    entities.reserve(ai_animation->mNumChannels);
    for (const aiNodeAnim* ai_node_anim
        : std::span(ai_animation->mChannels, ai_animation->mNumChannels)) {
        auto entity_opt = Entity::search(ai_node_anim->mNodeName.C_Str(), asset_root);
        if (!entity_opt.has_value()) {
            continue;
        }
        std::weak_ptr<Entity> entity = entity_opt.value();
        std::vector<AnimationKey<glm::vec3>> positions;
        positions.reserve(ai_node_anim->mNumPositionKeys);
        for (const aiVectorKey& position
            : std::span(ai_node_anim->mPositionKeys, ai_node_anim->mNumPositionKeys)) {
            positions.emplace_back(position.mTime / ai_animation->mTicksPerSecond,
                assimp_helper::vec3(position.mValue));
        }
        std::vector<AnimationKey<glm::quat>> rotations;
        rotations.reserve(ai_node_anim->mNumPositionKeys);
        for (const aiQuatKey& rotation
            : std::span(ai_node_anim->mRotationKeys, ai_node_anim->mNumRotationKeys)) {
            rotations.emplace_back(rotation.mTime / ai_animation->mTicksPerSecond,
                assimp_helper::quat(rotation.mValue));
        }
        std::vector<AnimationKey<glm::vec3>> scales;
        scales.reserve(ai_node_anim->mNumScalingKeys);
        for (const aiVectorKey& scale
            : std::span(ai_node_anim->mScalingKeys, ai_node_anim->mNumScalingKeys)) {
            scales.emplace_back(scale.mTime / ai_animation->mTicksPerSecond,
                assimp_helper::vec3(scale.mValue));
        }
        entities.emplace_back(entity, positions, rotations, scales);
    }
    return Animation((float)(ai_animation->mDuration / ai_animation->mTicksPerSecond),
        std::move(entities));
}

Animation::Animation(float duration, std::vector<EntityAnimation> entities) :
    m_duration(duration),
    m_entities(std::move(entities))
{
}

void Animation::on_update(float delta_time)
{
    m_time += delta_time;
    if (m_time > m_duration) {
        m_time -= m_duration;
    }
    for (auto it = m_entities.begin(); it != m_entities.end();) {
        if (it->entity.expired()) {
            m_entities.erase(it);
            continue;
        }
        Entity& entity = *it->entity.lock();
        auto is_next_key = [m_time = m_time](auto key) { return key.time > m_time; };
        auto get_interpolated_value = [&is_next_key, m_time = m_time](auto keys) {
            assert(!keys.empty());
            auto next = std::ranges::find_if(keys, is_next_key);
            if (next == keys.begin()) {
                return next->value;
            } else if (next == keys.end()) {
                return keys.back().value;
            } else {
                auto prev = next - 1;
                float t = (m_time - prev->time) / (next->time - prev->time);
                if constexpr (std::is_same_v<decltype(prev->value), glm::quat>) {
                    return glm::slerp(prev->value, next->value, t);
                } else {
                    return glm::mix(prev->value, next->value, t);
                }
            }
        };
        if (!it->positions.empty()) {
            entity.set_local_position(get_interpolated_value(it->positions));
        }
        if (!it->rotations.empty()) {
            entity.set_local_rotation(glm::eulerAngles(get_interpolated_value(it->rotations)));
        }
        if (!it->scales.empty()) {
            entity.set_local_scale(get_interpolated_value(it->scales));
        }
        it++;
    }
}

}
