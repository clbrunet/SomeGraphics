#include <cstdint>
#include <iostream>
#include <memory>
#include <stack>
#include <vector>

#include "imgui.h"

#include "Editor/Outliner.hpp"
#include "SomeGraphics.hpp"

namespace sg {

void Outliner::on_render(const Scene& scene, std::weak_ptr<SceneEntity>& selected_entity)
{
    ImGui::Begin("Outliner");
    std::stack<std::vector<std::shared_ptr<SceneEntity>>::const_iterator>
        iterators_stack({ scene.root_entity()->children().cbegin() });
    std::stack<std::vector<std::shared_ptr<SceneEntity>>::const_iterator>
        ends_stack({ scene.root_entity()->children().cend() });
    uintptr_t i = 0;
    while (iterators_stack.size() > 1 || iterators_stack.top() != ends_stack.top()) {
        const std::shared_ptr<SceneEntity>& entity = *iterators_stack.top();
        ++iterators_stack.top();
        ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow
            | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanAvailWidth;
        if (entity == selected_entity.lock()) {
            flags |= ImGuiTreeNodeFlags_Selected;
        }
        if (entity->children().size() == 0) {
            flags |= ImGuiTreeNodeFlags_Leaf;
        }
        if (ImGui::TreeNodeEx((void*)i, flags,  "%s", entity->name().c_str())) {
            iterators_stack.push(entity->children().cbegin());
            ends_stack.push(entity->children().cend());
        }
        if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen()) {
            selected_entity = entity;
        }
        while (iterators_stack.size() > 1 && iterators_stack.top() == ends_stack.top()) {
            ImGui::TreePop();
            iterators_stack.pop();
            ends_stack.pop();
        }
        i++;
    }
    ImGui::End();
}

}
