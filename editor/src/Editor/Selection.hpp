#pragma once

#include <variant>
#include <memory>

namespace sg {

class Entity;
class Material;

using Selection = std::variant<std::monostate, std::weak_ptr<Entity>, std::weak_ptr<Material>>;

}
