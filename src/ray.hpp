#pragma once

// #include <glm/fwd.hpp>
#include <glm/glm.hpp>

class Ray
{
    public:
    glm::vec3 origin;
    glm::vec3 direction;

    public:
    glm::vec3 at(const float t) const
    {
        return origin + t*direction;
    }
};