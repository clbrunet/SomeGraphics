#pragma once

#include <imgui.h>

#include "Editor/Selection.hpp"

namespace sg {

class Entity;
struct Light;
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
    void render(Entity& entity, const Renderer& renderer, Selection& selection) const;
    void render(Material& material, const Renderer& renderer, Selection& selection) const;
    void render_local_transform(Entity& entity) const;
    void render_light(Light& light) const;
};

}
