#include <cstdint>
#include <deque>
#include <iostream>
#include <memory>
#include <stack>

#include "imgui.h"

#include "Editor/Outliner.hpp"
#include "SomeGraphics.hpp"

namespace sg {

void Outliner::on_render(const Scene& scene, std::weak_ptr<SceneEntity>& selected_entity)
{
    ImGui::Begin("Outliner");
    std::deque<std::shared_ptr<SceneEntity>> entities = std::deque(scene.entities().begin(),
        scene.entities().end());
    std::stack<uint> children_count_stack = std::stack<uint>({ static_cast<unsigned int>(scene.entities().size()) });
    uintptr_t i = 0;
    while (!entities.empty()) {
        std::shared_ptr<SceneEntity> entity = entities.front();
        entities.pop_front();
        children_count_stack.top() -= 1;
        ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow
            | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanAvailWidth;
        if (selected_entity.lock() == entity) {
            flags |= ImGuiTreeNodeFlags_Selected;
        }
        if (entity->children().size() == 0) {
            flags |= ImGuiTreeNodeFlags_Leaf;
        }
        if (ImGui::TreeNodeEx((void*)i, flags,  "%s", entity->name().c_str())) {
            std::for_each(entity->children().crbegin(), entity->children().crend(),
                [&entities](const std::shared_ptr<SceneEntity>& child) {
                    entities.push_front(child);
                });
            children_count_stack.push(entity->children().size());
        }
        if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen()) {
            selected_entity = entity;
        }
        while (children_count_stack.size() > 1 && children_count_stack.top() == 0) {
            ImGui::TreePop();
            children_count_stack.pop();
        }
        i++;
    }
    ImGui::End();
}

}
