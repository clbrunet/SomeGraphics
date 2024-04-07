#pragma once

#include <imgui.h>

#include "Editor/Selection.hpp"

namespace sg {

class Renderer;
class Mesh;
struct Light;

class Properties {
public:
    Properties() = default;
    Properties(Properties&& other) = default;
    Properties(const Properties& other) = default;
    Properties& operator=(Properties&& other) = default;
    Properties& operator=(const Properties& other) = default;
    ~Properties() = default;

    void render(const Renderer& renderer, Selection& selection) const;
private:
    void render(Node& node, const Renderer& renderer, Selection& selection) const;
    void render(Material& material, const Renderer& renderer, Selection& selection) const;
    void render_local_transform(Node& node) const;
    void render_mesh(const Mesh& mesh, const Renderer& renderer, Selection& selection) const;
    void render_light(Light& light) const;
};

}
