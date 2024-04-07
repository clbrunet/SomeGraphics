#include <algorithm>

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>

#include "SomeGraphics/Scene.hpp"
#include "SomeGraphics/Window.hpp"
#include "SomeGraphics/Animation.hpp"
#include "SomeGraphics/AssimpHelper.hpp"
#include "SomeGraphics/ResourcesCache.hpp"

namespace sg {

Node& Scene::create_node(std::string name, Node& parent)
{
    entt::handle handle = entt::handle(m_registry, m_registry.create());
    return handle.emplace<Node>(std::move(name), handle, parent);
}

Node* Scene::load_model(const char* filename, Node& parent)
{
    Assimp::Importer importer;
    const aiScene* ai_scene = importer.ReadFile(filename, aiProcess_Triangulate | aiProcess_FlipUVs
        | aiProcess_GenNormals | aiProcess_CalcTangentSpace);
    if (ai_scene == nullptr) {
        std::cerr << "Model loading error : " << importer.GetErrorString() << '\n';
        return nullptr;
    }
    std::vector<LoadModelSkinRefs> skin_refs_vec;
    Node* node = load_model_first_pass(filename, *ai_scene->mRootNode, *ai_scene, parent, skin_refs_vec);
    if (!node) {
        return nullptr;
    }
    for (const LoadModelSkinRefs& skin_refs : skin_refs_vec) {
        std::optional<std::shared_ptr<Skin>> skin_opt = ResourcesCache::skin_from_ai_node(filename,
            skin_refs.ai_node, *ai_scene, *node);
        if (!skin_opt.has_value()) {
            return nullptr;
        }
        skin_refs.node.handle().emplace<std::shared_ptr<Skin>>(std::move(skin_opt.value()));
    }
    for (const aiAnimation* ai_anim : std::span(ai_scene->mAnimations, ai_scene->mNumAnimations)) {
        node->handle().emplace<Animation>(Animation::from_ai_animation(ai_anim, *node));
    }
    return node;
}

Node* Scene::search(std::string_view name, Node& search_root)
{
    return const_cast<Node*>(search(name, const_cast<const Node&>(search_root)));
}

const Node* Scene::search(std::string_view name, const Node& search_root)
{
    if (search_root.name() == name) {
        return &search_root;
    }
    const Node* found = nullptr;
    search_root.find_child([name, &found](const Node& child) {
        found = search(name, child);
        return found != nullptr;
    });
    return found;
}

void Scene::update(const Window& window)
{
    for (auto&& [entity, animation] : m_registry.view<Animation>().each()) {
        animation.update(window.delta_time());
    }
}

Node* Scene::load_model_first_pass(std::string_view filename, const aiNode& ai_node, const aiScene& ai_scene,
    Node& parent, std::vector<LoadModelSkinRefs>& skin_refs_vec)
{
    bool is_skin = false;
    std::shared_ptr<Mesh> mesh;
    if (ai_node.mNumMeshes > 0) {
        if (assimp_helper::ai_node_has_bones(ai_node, ai_scene)) {
            is_skin = true;
        } else {
            std::optional<std::shared_ptr<Mesh>> mesh_opt
                = ResourcesCache::mesh_from_ai_node(std::string(filename), ai_node, ai_scene);
            if (!mesh_opt.has_value()) {
                return nullptr;
            }
            mesh = std::move(mesh_opt.value());
        }
    }
    std::string name = ai_node.mName.C_Str();
    Transform transform = Transform(assimp_helper::mat4(ai_node.mTransformation));
    entt::handle handle = entt::handle(m_registry, m_registry.create());
    Node& node = handle.emplace<Node>(std::move(name), std::move(transform), handle, parent);
    if (mesh) {
        handle.emplace<std::shared_ptr<Mesh>>(std::move(mesh));
    }
    if (is_skin) {
        skin_refs_vec.emplace_back(ai_node, node);
    }
    for (int i = ai_node.mNumChildren - 1; i >= 0; i--) {
        if (!load_model_first_pass(filename, *ai_node.mChildren[i], ai_scene, node, skin_refs_vec)) {
            handle.destroy();
            return nullptr;
        }
    }
    return &node;
}

}
