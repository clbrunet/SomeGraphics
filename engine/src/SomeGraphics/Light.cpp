#include "SomeGraphics/Light.hpp"

namespace sg {

Light::Light(glm::vec3 p_color) :
    color(p_color)
{
}

Light::Light(glm::vec3 p_color, float p_intensity) :
    color(p_color),
    intensity(p_intensity)
{
}

}
