#pragma once

#include <memory>

#include "SomeGraphics.hpp"
#include "Editor/EditorCamera.hpp"

namespace sg {

class Viewport;

class EditorApplication final : public Application {
public:
    EditorApplication() = delete;
    EditorApplication(const std::string& name);
    EditorApplication(EditorApplication&& other) = default;
    EditorApplication(const EditorApplication& other) = delete;
    EditorApplication& operator=(EditorApplication&& other) = default;
    EditorApplication& operator=(const EditorApplication& other) = delete;
    ~EditorApplication() final override;

    void on_update(float delta_time) final override;
    void on_render() final override;
private:
    std::unique_ptr<Viewport> m_viewport;
};

}
