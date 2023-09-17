#include "SomeGraphics/Light.hpp"

namespace sg {

Light::Light(glm::vec3 color) :
    color(color)
{
}

Light::Light(glm::vec3 color, float intensity) :
    color(color),
    intensity(intensity)
{
}

}
