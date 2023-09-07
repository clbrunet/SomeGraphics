#include <cstdint>
#include <deque>
#include <iostream>
#include <stack>
#include <type_traits>
#include <variant>

#include "glm/ext/vector_float3.hpp"
#include "glm/ext/vector_float4.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "imgui.h"

#include "Editor/Properties.hpp"
#include "SomeGraphics.hpp"
#include "Editor/Selection.hpp"

namespace sg {

void Properties::on_render(Selection& selection) const
{
    std::visit([this, &selection](auto&& arg) {
        using T = std::decay_t<decltype(arg)>;
        if constexpr (std::is_same_v<T, std::monostate>) {
            on_render();
        } else {
            if (arg.expired()) {
                selection = std::monostate();
                on_render();
            } else {
                on_render(*arg.lock(), selection);
            }
        }
    }, selection);
}

void Properties::on_render() const
{
    ImGui::Begin("Properties");
    ImGui::End();
}

void Properties::on_render(SceneEntity& entity, Selection& selection) const
{
    ImGui::Begin("Properties");
    ImGui::Text("%s", entity.name().c_str());
    if (ImGui::TreeNodeEx("Transform", ImGuiTreeNodeFlags_DefaultOpen)) {
        glm::vec3 local_position = entity.transform().local_position();
        if (ImGui::DragFloat3("Position", glm::value_ptr(local_position), 0.2f)) {
            entity.transform().set_local_position(local_position);
        }
        glm::vec3 local_rotation_degrees = glm::degrees(entity.transform().local_rotation());
        if (ImGui::DragFloat3("Rotation", glm::value_ptr((local_rotation_degrees)), 1.0f)) {
            entity.transform().set_local_rotation(glm::radians(local_rotation_degrees));
        }
        glm::vec3 local_scale = entity.transform().local_scale();
        if (ImGui::DragFloat3("Scale", glm::value_ptr(local_scale), 0.1f)) {
            entity.transform().set_local_scale(local_scale);
        }
        ImGui::TreePop();
    }
    if (ImGui::TreeNodeEx("Materials", ImGuiTreeNodeFlags_DefaultOpen)) {
        if (entity.material() && ImGui::Button("Select material")) {
            selection = entity.material();
        }
        ImGui::TreePop();
    }
    ImGui::End();
}

void Properties::on_render(Material& material, Selection& selection) const
{
    static_cast<void>(selection);
    ImGui::Begin("Properties");
    for (const auto& [location, vec4] : material.vec4s()) {
        glm::vec4 copy = vec4;
        if (ImGui::ColorEdit4(location.c_str(), glm::value_ptr(copy))) {
            material.set_vec4(location, std::move(copy));
        }
    }
    for (const auto& [location, texture] : material.textures()) {
        ImGui::Image(texture->imgui_texture_id(), ImVec2(64.0, 64.0));
        ImGui::SameLine();
        ImGui::Text("%s", location.c_str());

    }
    ImGui::End();
}

}
