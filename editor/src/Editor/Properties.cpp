#include <cstdint>
#include <deque>
#include <iostream>
#include <memory>
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

void Properties::on_render(const Renderer& renderer, Selection& selection) const
{
    ImGui::Begin("Properties");
    std::visit([this, &renderer, &selection](auto&& arg) {
        using T = std::decay_t<decltype(arg)>;
        if constexpr (!(std::is_same_v<T, std::monostate>)) {
            if (arg.expired()) {
                selection = std::monostate();
            } else {
                render(*arg.lock(), renderer, selection);
            }
        }
    }, selection);
    ImGui::End();
}

void Properties::render(SceneEntity& entity, const Renderer& renderer, Selection& selection) const
{
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
    if (entity.material() && ImGui::TreeNodeEx("Material", ImGuiTreeNodeFlags_DefaultOpen)) {
        const char* edit_button_label = "Edit";
        ImGuiStyle& style = ImGui::GetStyle();
        float size = ImGui::CalcTextSize(edit_button_label).x + style.FramePadding.x * 2.0f;
        float avail = ImGui::GetContentRegionAvail().x;
        float off = (avail - size) * 0.5f;
        if (off > 0.0f) {
            ImGui::SetCursorPosX(ImGui::GetCursorPosX() + off);
        }
        if (ImGui::Button("Edit")) {
            selection = entity.material();
        }
        ImGui::BeginDisabled();
        render(*entity.material(), renderer, selection);
        ImGui::EndDisabled();
        ImGui::TreePop();
    }
}

void Properties::render(Material& material, const Renderer& renderer, [[maybe_unused]] Selection& selection) const
{
    for (const auto& [location, vec4] : material.vec4s()) {
        glm::vec4 copy = vec4;
        if (ImGui::ColorEdit4(location.c_str(), glm::value_ptr(copy))) {
            material.set_vec4(location, std::move(copy));
        }
    }
    struct SrgbTextureCallbackData {
        const Renderer& renderer;
        bool framebuffer_srgb_state;
    };
    auto srgb_texture_callback = []([[maybe_unused]] const ImDrawList* parent_list,
        const ImDrawCmd* cmd) {
        SrgbTextureCallbackData* data
            = reinterpret_cast<SrgbTextureCallbackData*>(cmd->UserCallbackData);
        data->renderer.set_framebuffer_srbg(data->framebuffer_srgb_state);
    };
    static SrgbTextureCallbackData enabling = { .renderer = renderer,
        .framebuffer_srgb_state = true };
    static SrgbTextureCallbackData disabling = { .renderer = renderer,
        .framebuffer_srgb_state = false };
    for (const auto& [location, texture] : material.textures()) {
        ImDrawList* draw_list = ImGui::GetWindowDrawList();
        if (texture->color_space() == ColorSpace::Srgb) {
            draw_list->AddCallback(srgb_texture_callback, &enabling);
        }
        ImGui::Image(texture->imgui_texture_id(), ImVec2(64.0, 64.0));
        if (texture->color_space() == ColorSpace::Srgb) {
            draw_list->AddCallback(srgb_texture_callback, &disabling);
        }
        ImGui::SameLine();
        ImGui::Text("%s", location.c_str());

    }
}

}
