#pragma once

#include <memory>
#include <SomeGraphics.hpp>

#include "Editor/Selection.hpp"

namespace sg {

class Viewport;
class Outliner;
class Properties;

class EditorApplication final : public Application {
public:
    EditorApplication() = delete;
    EditorApplication(std::string name);
    EditorApplication(EditorApplication&& other) = default;
    EditorApplication(const EditorApplication& other) = delete;
    EditorApplication& operator=(EditorApplication&& other) = delete;
    EditorApplication& operator=(const EditorApplication& other) = delete;
    ~EditorApplication() final override = default;

    void on_update(float delta_time) final override;
    void on_render() final override;
private:
    std::unique_ptr<Scene> m_scene = std::make_unique<Scene>();
    Selection m_selection;
    std::unique_ptr<Viewport> m_viewport;
    std::unique_ptr<Outliner> m_outliner = std::make_unique<Outliner>();
    std::unique_ptr<Properties> m_properties = std::make_unique<Properties>();
};

}
