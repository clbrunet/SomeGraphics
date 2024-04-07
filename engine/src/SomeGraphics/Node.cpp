#include "SomeGraphics/Node.hpp"

namespace sg {

Node Node::create_scene_root(entt::handle handle)
{
    Node root;
    root.m_name = "SceneRoot";
    root.m_handle = handle;
    root.m_is_model_matrix_dirty = false;
    return root;
}

Node::Node(std::string name, entt::handle handle, Node& parent) :
	m_name(std::move(name)),
    m_handle(handle),
    m_parent(&parent)
{
    init_relationships();
}

Node::Node(std::string name, Transform local_transform, entt::handle handle, Node& parent) :
	m_name(std::move(name)),
	m_local_transform(std::move(local_transform)),
    m_handle(handle),
	m_parent(&parent)
{
    init_relationships();
}

const std::string& Node::name() const
{
    return m_name;
}

const glm::vec3& Node::local_position() const
{
    return m_local_transform.position;
}

const glm::vec3& Node::local_rotation() const
{
    return m_local_transform.rotation;
}

const glm::vec3& Node::local_scale() const
{
    return m_local_transform.scale;
}

entt::entity Node::entity() const
{
    return m_handle.entity();
}

entt::registry& Node::registry() const
{
    return *m_handle.registry();
}

entt::handle Node::handle() const
{
    return m_handle;
}

void Node::set_local_position(glm::vec3 local_position)
{
    m_local_transform.position = local_position;
    set_model_matrix_dirty();
}

void Node::set_local_rotation(glm::vec3 local_rotation)
{
    m_local_transform.rotation = local_rotation;
    set_model_matrix_dirty();
}

void Node::set_local_scale(glm::vec3 local_scale)
{
    m_local_transform.scale = local_scale;
    set_model_matrix_dirty();
}

const glm::mat4& Node::model_matrix() const
{
    if (m_is_model_matrix_dirty) {
        m_model_matrix = m_parent->model_matrix() * m_local_transform.compute_matrix();
        m_is_model_matrix_dirty = false;
    }
    return m_model_matrix;
}

bool Node::is_parent() const
{
    return m_first_child != nullptr;
}

void Node::init_relationships()
{
    Node*& parent_first_child = m_parent->m_first_child;
    m_next_sibling = parent_first_child;
    parent_first_child = this;
}

void Node::set_model_matrix_dirty()
{
    m_is_model_matrix_dirty = true;
    for_each_child([](Node& node) {
        node.set_model_matrix_dirty();
    });
}

}