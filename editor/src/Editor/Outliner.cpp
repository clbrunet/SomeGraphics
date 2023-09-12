#include <cstdint>
#include <iostream>
#include <memory>
#include <variant>
#include <vector>

#include "imgui.h"

#include "Editor/Outliner.hpp"
#include "SomeGraphics.hpp"

namespace sg {

void Outliner::on_render(const Scene& scene, Selection& selection)
{
    std::shared_ptr<SceneEntity> selected_entity;
    if (std::holds_alternative<std::weak_ptr<SceneEntity>>(selection)) {
        selected_entity = std::get<std::weak_ptr<SceneEntity>>(selection).lock();
    }
    ImGui::Begin("Outliner");
    for (const std::shared_ptr<SceneEntity>& child : scene.root_entity()->children()) {
        render(child, selected_entity, selection);
    }
    ImGui::End();
}

void Outliner::render(const std::shared_ptr<SceneEntity>& entity,
    const std::shared_ptr<SceneEntity>& selected_entity, Selection& selection)
{
    ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow
        | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanAvailWidth;
    if (entity == selected_entity) {
        flags |= ImGuiTreeNodeFlags_Selected; 
    }
    if (entity->children().size() == 0) {
        flags |= ImGuiTreeNodeFlags_Leaf;
    }
    bool open = ImGui::TreeNodeEx(entity.get(), flags,  "%s", entity->name().c_str());
    if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen()) {
        selection = entity;
    }
    if (open) {
        for (const std::shared_ptr<SceneEntity>& child : entity->children()) {
            render(child, selected_entity, selection);
        }
        ImGui::TreePop();
    }
}

}
