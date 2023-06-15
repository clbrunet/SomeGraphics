#pragma once

#include <memory>

#include "SomeGraphics.hpp"
#include "Editor/EditorCamera.hpp"

namespace sg {

class Viewport;

class EditorApplication final : public Application {
public:
    EditorApplication(const std::string& name);
    ~EditorApplication() final;
    void on_update(float delta_time) final override;
    void on_render() final override;
private:
    std::unique_ptr<Viewport> m_viewport;
};

}
