#pragma once

#include <memory>

#include "SomeGraphics/Application.hpp"
#include "SomeGraphics/Program.hpp"
#include "SomeGraphics/VertexArray.hpp"
#include "SomeGraphics/Camera.hpp"

extern std::unique_ptr<sg::Application> create_app();
