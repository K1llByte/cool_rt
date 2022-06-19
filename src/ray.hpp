#pragma once

// #include <glm/fwd.hpp>
#include <glm/glm.hpp>

class Ray
{
    public:
    glm::vec3 origin;
    glm::vec3 direction;
    float     time;

    public:
    Ray() = default;
    Ray(const glm::vec3& _origin, const glm::vec3& _direction, float _time = 0.f)
        : origin(_origin)
        , direction(_direction)
        , time(_time) {}

    glm::vec3 at(const float t) const
    {
        return origin + t*direction;
    }
};