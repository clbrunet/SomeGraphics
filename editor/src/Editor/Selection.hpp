#pragma once

#include <variant>
#include <memory>

#include <entt/entt.hpp>

namespace sg {

class Material;

using Selection = std::variant<std::monostate, entt::handle, std::weak_ptr<Material>>;

}
