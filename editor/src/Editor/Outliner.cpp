#include <cstdint>
#include <iostream>
#include <memory>
#include <variant>
#include <vector>

#include <imgui.h>
#include <SomeGraphics.hpp>

#include "Editor/Outliner.hpp"

namespace sg {

void Outliner::on_render(const Scene& scene, Selection& selection)
{
    std::shared_ptr<Entity> selected_entity;
    if (std::holds_alternative<std::weak_ptr<Entity>>(selection)) {
        selected_entity = std::get<std::weak_ptr<Entity>>(selection).lock();
    }
    ImGui::Begin("Outliner");
    for (const std::shared_ptr<Entity>& child : scene.root()->children()) {
        render(child, selected_entity, selection);
    }
    ImGui::End();
}

void Outliner::render(const std::shared_ptr<Entity>& entity,
    const std::shared_ptr<Entity>& selected_entity, Selection& selection)
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
        for (const std::shared_ptr<Entity>& child : entity->children()) {
            render(child, selected_entity, selection);
        }
        ImGui::TreePop();
    }
}

}
