#include "camera.hpp"

Camera::Camera(float aspect_ratio)
{
    auto viewport_height = 2.0f;
    auto viewport_width = aspect_ratio * viewport_height;
    auto focal_length = 1.0f;

    origin = glm::vec3{0, 0, 0};
    horizontal = glm::vec3{viewport_width, 0.0, 0.0};
    vertical = glm::vec3{0.0, viewport_height, 0.0};
    lower_left_corner = origin - horizontal/2.f - vertical/2.f - glm::vec3{0, 0, focal_length};
}

Ray Camera::get_ray(float u, float v) const
{
    return Ray(origin, lower_left_corner + u*horizontal + v*vertical - origin);
}