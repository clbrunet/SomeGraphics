#pragma once

#include "SomeGraphics.hpp"

namespace sg {

class EditorApplication final : public Application {
public:
    EditorApplication(const std::string& name);
    ~EditorApplication() final;
};

}
