#pragma once

#include "image.hpp"
#include "utils.hpp"
#include "camera.hpp"
#include "scene.hpp"
#include "ray.hpp"

#include <fmt/core.h>

#define SAMPLES_PER_PIXEL 50
#define NUM_ITERATIONS 50

inline void write_color(Image& out, const glm::ivec2& pos, glm::vec3 color)
{
    // Write the translated [0,255] value of each color component.
    color = glm::sqrt(color / static_cast<float>(SAMPLES_PER_PIXEL));
    glm::ivec3 pixel{
        256 * std::clamp(color.r, 0.0f, 0.999f),
        256 * std::clamp(color.g, 0.0f, 0.999f),
        256 * std::clamp(color.b, 0.0f, 0.999f)
    };

    out.set_pixel(pos.x, pos.y, pixel);
}

inline glm::vec3 ray_color(const Ray& r, Scene& scene, float depth)
{
    // If we've exceeded the ray bounce limit, no more light is gathered.
    if (depth <= 0)
        return glm::vec3{0,0,0};

    // Hit sphere
    Intersection intersect;
    glm::vec3 color{0.8,0.7,0};
    if(scene.hit(r, 0.001, infinity, intersect)) {
        Ray scattered;
        glm::vec3 attenuation;
        if(intersect.material->scatter(r, intersect, attenuation, scattered))
            return attenuation * ray_color(scattered, scene, depth-1);
        return glm::vec3{0,0,0};
        // glm::vec3 target = intersect.point + intersect.normal + glm::normalize(random_in_unit_sphere());
        // return /* 0.5f */ color * ray_color(Ray{intersect.point, target - intersect.point}, scene, depth-1);
    }

    // Values are normalized from [-1, 1] to [0, 1].
    auto t = 0.5f * (glm::normalize(r.direction).g + 1.0f);
    return (1.0f-t) * glm::vec3{1.0f, 1.0f, 1.0f} + t * glm::vec3{0.5f, 0.7f, 1.0f};
}

// struct RayTracerConfig
// {

// };

class Renderer
{
    public:
    Image& render_target;
    
    public:
    Renderer(Image& _render_target)
        : render_target(_render_target) {}

    void render(Scene& scene, Camera& camera)
    {
        // const size_t samples_per_pixel = 50;
        // const size_t num_iterations = 50;
        
        const auto width = render_target.get_width();
        const auto height = render_target.get_height();
        // FIXME: Change the logic here
        for(ssize_t j = height-1; j >= 0; --j)
        {
            fmt::print("\rRows remaining: {}\n", j);
            for(size_t i = 0; i < width; ++i)
            {
                glm::vec3 color{0,0,0};
                // Stochastic sampling
                auto min_u = (i-0.5f) / (width-1.f);
                auto max_u = (i+0.5f) / (width-1.f);
                auto min_v = (j-0.5f) / (height-1.f);
                auto max_v = (j+0.5f) / (height-1.f);

                for(size_t s = 0 ; s < SAMPLES_PER_PIXEL; ++s)
                {
                    auto u = random_float(min_u, max_u);
                    auto v = random_float(min_v, max_v);

                    // Instanciate Ray
                    auto r = camera.get_ray(u, v);

                    color += ray_color(r, scene, NUM_ITERATIONS);
                }
                // Write color to render target
                write_color(render_target, glm::ivec2{i,height-j}, color);
            }
        }

    }
};