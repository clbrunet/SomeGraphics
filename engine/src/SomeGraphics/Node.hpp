#pragma once

#include <optional>

#include <entt/entt.hpp>

#include "SomeGraphics/Transform.hpp"

namespace sg {

class Node {
public:
    static Node create_scene_root(entt::handle handle);
    Node(std::string name, entt::handle handle, entt::entity parent);
    Node(std::string name, Transform local_transform, entt::handle handle, entt::entity parent);
    Node(Node&& other) = default;
    Node(const Node& other) = delete;
    Node& operator=(Node&& other) = default;
    Node& operator=(const Node& other) = delete;
    ~Node() = default;

    const std::string& name() const;
    const glm::vec3& local_position() const;
    const glm::vec3& local_rotation() const;
    const glm::vec3& local_scale() const;
    const glm::mat4& model_matrix() const;
    entt::entity entity() const;
    entt::registry& registry() const;
    entt::handle handle() const;

    void set_local_position(glm::vec3 local_position);
    void set_local_rotation(glm::vec3 local_rotation);
    void set_local_scale(glm::vec3 local_scale);

    bool is_parent() const;

    void for_each_child(auto fn)
    {
        entt::entity entity = m_first_child;
        while (entity != entt::null) {
            Node& node = m_handle.registry()->get<Node>(entity);
            fn(node);
            entity = node.m_next;
        }
    }

    void for_each_child(auto fn) const
    {
        entt::entity entity = m_first_child;
        while (entity != entt::null) {
            const Node& node = m_handle.registry()->get<const Node>(entity);
            fn(node);
            entity = node.m_next;
        }
    }

    Node* find_child(auto predicate)
    {
        entt::entity entity = m_first_child;
        while (entity != entt::null) {
            Node& node = m_handle.registry()->get<Node>(entity);
            if (predicate(node)) {
                return &node;
            }
            entity = node.m_next;
        }
        return nullptr;
    }
private:
    std::string m_name;
    Transform m_local_transform;
    mutable glm::mat4 m_model_matrix = glm::mat4(1.0f);
    mutable bool m_is_model_matrix_dirty = true;
    entt::handle m_handle;
    entt::entity m_parent;
    entt::entity m_first_child = entt::null;
    entt::entity m_next = entt::null;

    Node() = default;
    void init_relationships();
    void set_model_matrix_dirty();
};

}