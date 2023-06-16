#pragma once

#include <memory>

#include "GLFW/glfw3.h"

#include "SomeGraphics/Application.hpp"
#include "SomeGraphics/Program.hpp"
#include "SomeGraphics/VertexArray.hpp"
#include "SomeGraphics/FrameBuffer.hpp"
#include "SomeGraphics/Texture.hpp"
#include "SomeGraphics/Camera.hpp"
#include "SomeGraphics/Renderer.hpp"

extern std::unique_ptr<sg::Application> create_app();
