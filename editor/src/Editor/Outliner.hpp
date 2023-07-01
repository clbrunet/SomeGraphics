#pragma once

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

    void on_render(const Scene& scene);
};

}
