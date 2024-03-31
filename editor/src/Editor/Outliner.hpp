#pragma once

#include <memory>

#include "Editor/Selection.hpp"

namespace sg {

class Scene;

class Outliner {
public:
    Outliner() = default;
    Outliner(Outliner&& other) = default;
    Outliner(const Outliner& other) = default;
    Outliner& operator=(Outliner&& other) = default;
    Outliner& operator=(const Outliner& other) = default;
    ~Outliner() = default;

    void render(const Scene& scene, Selection& selection);
private:
    void render(const Node& node, entt::entity selected_entity, Selection& selection);
};

}
