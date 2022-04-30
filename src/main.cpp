#include <iostream>
#include <fstream>
#include <algorithm>

#include <glm/glm.hpp>
#include <glm/gtx/norm.hpp>

#include "ray.hpp"
#include "intersection.hpp"
#include "sphere.hpp"
#include "camera.hpp"
#include "utils.hpp"

#define SAMPLES_PER_PIXEL 16

void write_color(std::ostream &out, glm::vec3 color)
{
    // Write the translated [0,255] value of each color component.
    color /= static_cast<float>(SAMPLES_PER_PIXEL);
    glm::ivec3 res{
        256 * std::clamp(color.r, 0.0f, 0.999f),
        256 * std::clamp(color.g, 0.0f, 0.999f),
        256 * std::clamp(color.b, 0.0f, 0.999f)
    };
    out << res.r << ' ' << res.g << ' ' << res.b << '\n';
}

float hit_sphere(const glm::vec3& center, float radius, const Ray& r)
{
    // Simplification explained in 6.2
    glm::vec3 oc = r.origin - center;
    auto a = glm::length2(r.direction);
    auto half_b = glm::dot(oc, r.direction);
    auto c = glm::dot(oc, oc) - radius * radius;
    auto discriminant = half_b*half_b - a*c;
    // If there's no solutions return -1
    // otherwise return the distance of the hit point
    // this will be used to compute the normal since we
    // already know the other values.
    return (discriminant < 0)
        ? -1.0
        : (-half_b-sqrt(discriminant)) / a;
}

glm::vec3 ray_color(const Ray& r)
{
    // Hit sphere
    Intersection intersect;
    Sphere sphere(glm::vec3{0, 0, -1}, 0.5f);
    if(sphere.hit(r, 0, 2.f, intersect)) {
        // Values are normalized from [-1, 1] to [0, 1].
        return 0.5f * (intersect.normal+1.f);
    }

    // Values are normalized from [-1, 1] to [0, 1].
    auto t = 0.5f * (glm::normalize(r.direction).g + 1.0f);
    return (1.0f-t) * glm::vec3{1.0f, 1.0f, 1.0f} + t * glm::vec3{0.5f, 0.7f, 1.0f};
}

int main() {

    // Render Target
    std::ofstream file("image.ppm");
    if(not file.is_open())
    {
        std::cerr << "Error: Cannot open file\n";
        return 1;
    }

    // Image

    const int image_width = 400;
    const int image_height = 300;
    const auto aspect_ratio = static_cast<float>(image_width) / static_cast<float>(image_height);

    // Camera
    auto camera = Camera(aspect_ratio);
    // auto viewport_height = 2.0;
    // auto viewport_width = aspect_ratio * viewport_height;
    // auto focal_length = 1.0;

    // const glm::vec3 origin{0, 0, 0};
    // auto horizontal = glm::vec3{viewport_width, 0, 0};
    // auto vertical = glm::vec3{0, viewport_height, 0};
    // // Lower left corner of the viewport
    // const auto lower_left_corner = origin - horizontal/2.f - vertical/2.f - glm::vec3(0, 0, focal_length);


    // Render

    file << "P3\n" << image_width << ' ' << image_height << "\n255\n";

    for (int j = image_height-1; j >= 0; --j)
    {
        std::cerr << "\rRows remaining: " << j << " \n";
        for (int i = 0; i < image_width; ++i)
        {
#if SAMPLES_PER_PIXEL == 1
            // Linear sampling
            auto u = i / (image_width-1.f);
            auto v = j / (image_height-1.f);

            // Instanciate Ray
            auto r = camera.get_ray(u, v);

            glm::vec3 color = ray_color(r);
#else
            glm::vec3 color{};
            // Stochastic sampling
            auto min_u = (i-0.5f) / (image_width-1.f);
            auto max_u = (i+0.5f) / (image_width-1.f);
            auto min_v = (j-0.5f) / (image_height-1.f);
            auto max_v = (j+0.5f) / (image_height-1.f);

            for(size_t s = 0 ; s < SAMPLES_PER_PIXEL; ++s)
            {
                auto u = random_float(min_u, max_u);
                auto v = random_float(min_v, max_v);

                // Instanciate Ray
                auto r = camera.get_ray(u, v);

                color += ray_color(r);
            }
#endif

            write_color(file, color);
        }
    }

    file.close();
}