#include "camera.hpp"

Camera::Camera(
    glm::vec3 position,
    glm::vec3 lookat,
    glm::vec3 _up,
    float _vfov,
    float aspect_ratio,
    float _time0,
    float _time1)
    : origin(position)
    , vfov(_vfov) 
    , time0(_time0) 
    , time1(_time1)
{
    // float h = tan(glm::radians(vfov)/2.f);
    // // viewport_height varies according to the fov
    // auto viewport_height = 2.0f * h;
    // auto viewport_width = aspect_ratio * viewport_height;
    // auto focal_length = 1.0f;

    // front = glm::normalize(position - lookat);
    // left = glm::normalize(glm::cross(up, front));
    // up = glm::cross(front, left);

    // horizontal = focus_dist * viewport_width * left;
    // vertical = focus_dist * viewport_height * up;
    // lower_left_corner = origin - horizontal/2.f - vertical/2.f - focus_dist * front;

    // lens_radius = aperture / 2;


    float h = tan(glm::radians(vfov)/2.f);
    // viewport_height varies according to the fov
    viewport_height = 2.0f * h;
    viewport_width = aspect_ratio * viewport_height;

    back = glm::normalize(position - lookat);
    right = glm::normalize(glm::cross(_up, back));
    up = glm::cross(back, right);

    horizontal = viewport_width * right;
    vertical = viewport_height * up;
    lower_left_corner = origin - horizontal/2.f - vertical/2.f - back;

    lens_radius = 0.f;
}

Ray Camera::get_ray(float u, float v) const
{
    glm::vec3 rd = lens_radius * random_in_unit_disk();
    glm::vec3 offset = right * rd.x + up * rd.y;

    return Ray(
        origin + offset,
        lower_left_corner + u*horizontal + v*vertical - origin - offset,
        random_float(time0, time1)
    );
}

void Camera::set_defocus_blur(float focus_dist, float aperture)
{
    horizontal = focus_dist * viewport_width * right;
    vertical = focus_dist * viewport_height * up;
    lower_left_corner = origin - horizontal/2.f - vertical/2.f - focus_dist * back;

    lens_radius = aperture / 2;
}