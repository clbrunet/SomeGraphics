#pragma once

#include <memory>
#include <vector>
#include <optional>

#include <assimp/scene.h>
#include <entt/entt.hpp>

#include "SomeGraphics/Node.hpp"

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

    entt::registry& registry() { return m_registry; }
    const entt::registry& registry() const { return m_registry; }

    [[nodiscard]] Node& root() { return m_root; };
    [[nodiscard]] const Node& root() const { return m_root; };

    Node& create_node(std::string name, Node& parent);
    Node* load_model(const char* filename, Node& parent);

    [[nodiscard]] static Node* search(std::string_view name, Node& search_root);
    [[nodiscard]] static const Node* search(std::string_view name, const Node& search_root);

    void update(const Window& window);
private:
    entt::registry m_registry;
    Node& m_root = create_root();

    [[nodiscard]] Node& create_root() {
        entt::handle handle = entt::handle(m_registry, m_registry.create());
        return handle.emplace<Node>(Node::create_scene_root(handle));
    }

    struct LoadModelSkinRefs {
        const aiNode& ai_node;
        Node& node;

        LoadModelSkinRefs(const aiNode& p_ai_node, Node& p_node) :
            ai_node(p_ai_node), node(p_node)
        {
        }
    };

    Node* load_model_first_pass(std::string_view filename, const aiNode& ai_node, const aiScene& ai_scene,
        Node& parent, std::vector<LoadModelSkinRefs>& skin_refs_vec);
};

}
