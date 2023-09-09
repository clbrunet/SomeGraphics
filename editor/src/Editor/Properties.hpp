#pragma once

#include "imgui.h"

#include "Editor/Selection.hpp"

namespace sg {

class SceneEntity;
class Renderer;

class Properties {
public:
    Properties() = default;
    Properties(Properties&& other) = default;
    Properties(const Properties& other) = default;
    Properties& operator=(Properties&& other) = default;
    Properties& operator=(const Properties& other) = default;
    ~Properties() = default;

    void on_render(const Renderer& renderer, Selection& selection) const;
private:
    void render(SceneEntity& entity, const Renderer& renderer, Selection& selection) const;
    struct SrgbTextureCallbackData {
        const Renderer& renderer;
        bool framebuffer_srgb_state;
    };
    static void srgb_texture_callback(const ImDrawList* parent_list, const ImDrawCmd* cmd);
    void render(Material& material, const Renderer& renderer, Selection& selection) const;
};

}
