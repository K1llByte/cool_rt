#pragma once

#include <glm/glm.hpp>

#include "ray.hpp"

class Camera
{
    private:
    glm::vec3 origin;
    glm::vec3 lower_left_corner;
    glm::vec3 horizontal;
    glm::vec3 vertical;

    public:
    Camera(float aspect_ratio);

    Ray get_ray(float u, float v) const;
};