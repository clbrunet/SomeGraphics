#pragma once

#include <memory>

#include "GLFW/glfw3.h"

#include "SomeGraphics/Application.hpp"
#include "SomeGraphics/Rendering/Program.hpp"
#include "SomeGraphics/Rendering/VertexArray.hpp"
#include "SomeGraphics/Rendering/FrameBuffer.hpp"
#include "SomeGraphics/Rendering/Texture.hpp"
#include "SomeGraphics/Camera.hpp"
#include "SomeGraphics/Rendering/Renderer.hpp"

extern std::unique_ptr<sg::Application> create_app();
