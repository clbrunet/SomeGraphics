#pragma once

#include "Editor/Selection.hpp"

namespace sg {

class SceneEntity;

class Properties {
public:
    Properties() = default;
    Properties(Properties&& other) = default;
    Properties(const Properties& other) = default;
    Properties& operator=(Properties&& other) = default;
    Properties& operator=(const Properties& other) = default;
    ~Properties() = default;

    void on_render(Selection& selection) const;
private:
    void on_render() const;
    void on_render(SceneEntity& entity, Selection& selection) const;
    void on_render(Material& material, Selection& selection) const;
};

}
