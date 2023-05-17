#pragma once

#include <memory>

#include "SomeGraphics/Application.hpp"

extern std::unique_ptr<sg::Application> create_app();
