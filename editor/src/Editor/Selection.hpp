#pragma once

#include <variant>
#include <memory>

namespace sg {

class SceneEntity;
class Material;

using Selection = std::variant<std::monostate, std::weak_ptr<SceneEntity>, std::weak_ptr<Material>>;

}
