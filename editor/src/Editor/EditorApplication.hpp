#pragma once

#include "SomeGraphics.hpp"

namespace sg {

class EditorApplication final : public Application {
public:
    EditorApplication(const std::string& name);
    ~EditorApplication() final;
    void on_update() final override;
    void on_render() final override;
};

}
