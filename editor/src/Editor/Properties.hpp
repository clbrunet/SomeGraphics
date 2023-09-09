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
    void render(Material& material, const Renderer& renderer, Selection& selection) const;
};

}
