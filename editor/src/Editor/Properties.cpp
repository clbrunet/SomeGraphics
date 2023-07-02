#include <cstdint>
#include <deque>
#include <iostream>
#include <stack>

#include "glm/ext/vector_float3.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "imgui.h"

#include "Editor/Properties.hpp"
#include "SomeGraphics.hpp"

namespace sg {

void Properties::on_render()
{
    ImGui::Begin("Properties");
    ImGui::End();
}

void Properties::on_render(SceneEntity& entity)
{
    ImGui::Begin("Properties");
    ImGui::Text("%s", entity.name().c_str());
    if (ImGui::TreeNodeEx("Transform", ImGuiTreeNodeFlags_DefaultOpen)) {
        glm::vec3 local_position = entity.transform().local_position();
        if (ImGui::DragFloat3("Position", glm::value_ptr(local_position))) {
            entity.transform().set_local_position(local_position);
        }
        ImGui::TreePop();
    }
    ImGui::End();
}

}
