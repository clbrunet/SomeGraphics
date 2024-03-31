#include <cstdint>
#include <iostream>
#include <memory>
#include <variant>
#include <vector>

#include <imgui.h>
#include <SomeGraphics.hpp>

#include "Editor/Outliner.hpp"

namespace sg {

void Outliner::render(const Scene& scene, Selection& selection)
{
    entt::entity selected_entity;
    if (std::holds_alternative<entt::handle>(selection)) {
        selected_entity = std::get<entt::handle>(selection).entity();
    }
    ImGui::Begin("Outliner");
    scene.root().for_each_child([this, selected_entity, &selection](const Node& child) {
        render(child, selected_entity, selection);
    });
    ImGui::End();
}

void Outliner::render(const Node& node, entt::entity selected_entity, Selection& selection)
{
    ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow
        | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanAvailWidth;
    if (node.entity() == selected_entity) {
        flags |= ImGuiTreeNodeFlags_Selected; 
    }
    if (!node.is_parent()) {
        flags |= ImGuiTreeNodeFlags_Leaf;
    }
    bool open = ImGui::TreeNodeEx(&node, flags,  "%s", node.name().c_str());
    if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen()) {
        selection = node.handle();
    }
    if (open) {
        node.for_each_child([this, selected_entity, &selection](const Node& child) {
            render(child, selected_entity, selection);
        });
        ImGui::TreePop();
    }
}

}
