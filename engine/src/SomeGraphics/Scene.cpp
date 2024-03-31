#include <algorithm>

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>

#include "SomeGraphics/Scene.hpp"
#include "SomeGraphics/Window.hpp"
#include "SomeGraphics/Animation.hpp"
#include "SomeGraphics/AssimpHelper.hpp"
#include "SomeGraphics/ResourcesCache.hpp"

namespace sg {

Node& Scene::create_node(std::string name, entt::entity parent)
{
    entt::entity entity = registry.create();
    return registry.emplace<Node>(entity, std::move(name), entt::handle(registry, entity), parent);
}

Node* Scene::load_model(const char* filename, entt::entity parent)
{
    Assimp::Importer importer;
    const aiScene* ai_scene = importer.ReadFile(filename, aiProcess_Triangulate | aiProcess_FlipUVs
        | aiProcess_GenNormals | aiProcess_CalcTangentSpace);
    if (ai_scene == nullptr) {
        std::cerr << "Model loading error : " << importer.GetErrorString() << '\n';
        return nullptr;
    }
    std::vector<LoadModelSkinRefs> skin_refs_vec;
    Node* node = load_model_first_pass(filename,
        *ai_scene->mRootNode, *ai_scene, parent, skin_refs_vec);
    if (!node) {
        return nullptr;
    }
    for (const LoadModelSkinRefs& skin_refs : skin_refs_vec) {
        std::optional<std::shared_ptr<Skin>> skin_opt
            = ResourcesCache::skin_from_ai_node(filename,
                skin_refs.ai_node, *ai_scene, node->handle());
        if (!skin_opt.has_value()) {
            return nullptr;
        }
        registry.emplace<std::shared_ptr<Skin>>(skin_refs.entity, std::move(skin_opt.value()));
    }
    for (const aiAnimation* ai_anim : std::span(ai_scene->mAnimations, ai_scene->mNumAnimations)) {
        registry.emplace<Animation>(node->entity(), Animation::from_ai_animation(ai_anim, node->handle()));
    }
    return node;
}

entt::entity Scene::search(std::string_view name, entt::handle handle)
{
    Node& node = handle.get<Node>();
    if (node.name() == name) {
        return handle.entity();
    }
    entt::entity found = entt::null;
    node.find_child([name, &found](Node& child) {
        found = search(name, child.handle());
        return found != entt::null;
    });
    return found;
}


void Scene::update(const Window& window)
{
    for (auto [entity, animation] : registry.view<Animation>().each()) {
        animation.update(window.delta_time());
    }
}

Node* Scene::load_model_first_pass(std::string_view filename,
    const aiNode& ai_node, const aiScene& ai_scene, entt::entity parent,
    std::vector<LoadModelSkinRefs>& skin_refs_vec)
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
    entt::entity entity = registry.create();
    registry.emplace<Node>(entity, std::move(name), std::move(transform), entt::handle(registry, entity), parent);
    if (mesh) {
        registry.emplace<std::shared_ptr<Mesh>>(entity, std::move(mesh));
    }
    if (is_skin) {
        skin_refs_vec.emplace_back(ai_node, entity);
    }
    for (int i = ai_node.mNumChildren - 1; i >= 0; i--) {
        if (!load_model_first_pass(filename, *ai_node.mChildren[i],
            ai_scene, entity, skin_refs_vec)) {
            registry.destroy(entity);
            return nullptr;
        }
    }
    return &registry.get<Node>(entity);
}

}
