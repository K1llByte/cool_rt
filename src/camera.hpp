#pragma once

#include <glm/glm.hpp>

#include "ray.hpp"
#include "utils.hpp"

class Camera
{
    private:
    glm::vec3 origin;
    glm::vec3 lower_left_corner;
    glm::vec3 horizontal;
    glm::vec3 vertical;
    glm::vec3 back;
    glm::vec3 up;
    glm::vec3 right;

    float viewport_height;
    float viewport_width;
    float vfov;
    float lens_radius;

    public:
    Camera(
        glm::vec3 position,
        glm::vec3 lookat,
        glm::vec3 up,
        float vfov,
        float aspect_ratio);

    Ray get_ray(float u, float v) const;

    void set_defocus_blur(float focus_dist, float aperture);
};