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

    entt::registry registry;

    [[nodiscard]] Node& root() { return m_root; };
    [[nodiscard]] const Node& root() const { return m_root; };

    Node& create_node(std::string name, entt::entity parent);
    Node* load_model(const char* filename, entt::entity parent);
    [[nodiscard]] static entt::entity search(std::string_view name, entt::handle handle);

    void update(const Window& window);
private:
    Node& m_root = create_root();

    [[nodiscard]] Node& create_root() {
        entt::entity entity = registry.create();
        return registry.emplace<Node>(entity, Node::create_scene_root(entt::handle(registry, entity)));
    }

    struct LoadModelSkinRefs {
        const aiNode& ai_node;
        entt::entity entity;

        LoadModelSkinRefs(const aiNode& p_ai_node, entt::entity p_entity) :
            ai_node(p_ai_node), entity(p_entity)
        {
        }
    };

    Node* load_model_first_pass(std::string_view filename,
        const aiNode& ai_node, const aiScene& ai_scene, entt::entity parent,
        std::vector<LoadModelSkinRefs>& skin_refs_vec);
};

}
