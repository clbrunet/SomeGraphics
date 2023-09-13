#pragma once

#include <memory>

#include "Editor/Selection.hpp"

namespace sg {

class Scene;
class Entity;

class Outliner {
public:
    Outliner() = default;
    Outliner(Outliner&& other) = default;
    Outliner(const Outliner& other) = default;
    Outliner& operator=(Outliner&& other) = default;
    Outliner& operator=(const Outliner& other) = default;
    ~Outliner() = default;

    void on_render(const Scene& scene, Selection& selection);
private:
    void render(const std::shared_ptr<Entity>& entity,
        const std::shared_ptr<Entity>& selected_entity, Selection& selection);
};

}
