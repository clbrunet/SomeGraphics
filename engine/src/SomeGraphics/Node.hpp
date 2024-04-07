#pragma once

#include <optional>

#include <entt/entt.hpp>

#include "SomeGraphics/Transform.hpp"

namespace sg {

class Node {
public:
    static Node create_scene_root(entt::handle handle);
    Node(std::string name, entt::handle handle, Node& parent);
    Node(std::string name, Transform local_transform, entt::handle handle, Node& parent);
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
        for (Node* node = m_first_child; node != nullptr; node = node->m_next_sibling) {
            fn(*node);
        }
    }

    void for_each_child(auto fn) const
    {
        for (const Node* node = m_first_child; node != nullptr; node = node->m_next_sibling) {
            fn(*node);
        }
    }

    Node* find_child(auto predicate)
    {
        for (Node* node = m_first_child; node != nullptr; node = node->m_next_sibling) {
            if (predicate(*node)) {
                return node;
            }
        }
        return nullptr;
    }

    const Node* find_child(auto predicate) const
    {
        for (const Node* node = m_first_child; node != nullptr; node = node->m_next_sibling) {
            if (predicate(*node)) {
                return node;
            }
        }
        return nullptr;
    }
private:
    std::string m_name;
    Transform m_local_transform;
    mutable glm::mat4 m_model_matrix = glm::mat4(1.0f);
    mutable bool m_is_model_matrix_dirty = true;
    entt::handle m_handle;
    Node* m_parent = nullptr;
    Node* m_first_child = nullptr;
    Node* m_next_sibling = nullptr;

    Node() = default;
    void init_relationships();
    void set_model_matrix_dirty();
};

}