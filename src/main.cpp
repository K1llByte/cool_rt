#include <iostream>
#include <fstream>
#include <algorithm>

#include <glm/glm.hpp>
#include <glm/gtx/norm.hpp>

#include "ray.hpp"
#include "material.hpp"
#include "intersection.hpp"
#include "sphere.hpp"
#include "camera.hpp"
#include "utils.hpp"
#include "scene.hpp"
#include "image.hpp"

#define FAST_RENDER

#ifdef FAST_RENDER
#   define WIDTH  400 // 1600  
#   define HEIGHT 300 //  900  
#   define SAMPLES_PER_PIXEL 50
#   define NUM_ITERATIONS 50
#else
#   define WIDTH  1600
#   define HEIGHT  900
#   define SAMPLES_PER_PIXEL 200
#   define NUM_ITERATIONS 50
#endif

// void write_color(std::ostream &out, glm::vec3 color)
// {
//     // glm::radians(360);
//     // glm::radians(180);

//     // Write the translated [0,255] value of each color component.
//     color = glm::sqrt(color / static_cast<float>(SAMPLES_PER_PIXEL));
//     glm::ivec3 res{
//         256 * std::clamp(color.r, 0.0f, 0.999f),
//         256 * std::clamp(color.g, 0.0f, 0.999f),
//         256 * std::clamp(color.b, 0.0f, 0.999f)
//     };
//     out << res.r << ' ' << res.g << ' ' << res.b << '\n';
// }

void write_color(Image& out, const glm::ivec2& pos, glm::vec3 color)
{
    // Write the translated [0,255] value of each color component.
    color = glm::sqrt(color / static_cast<float>(SAMPLES_PER_PIXEL));
    glm::ivec3 pixel{
        256 * std::clamp(color.r, 0.0f, 0.999f),
        256 * std::clamp(color.g, 0.0f, 0.999f),
        256 * std::clamp(color.b, 0.0f, 0.999f)
    };

    out.set_pixel(pos.x, pos.y, pixel);
    // out << res.r << ' ' << res.g << ' ' << res.b << '\n';
}

glm::vec3 ray_color(const Ray& r, Scene& scene, float depth)
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

int main()
{
    // Render Target
    // std::ofstream file("image.ppm");
    // if(not file.is_open())
    // {
    //     std::cerr << "Error: Cannot open file\n";
    //     return 1;
    // }
    auto render_target = Image(WIDTH, HEIGHT);

    // Image
    const int image_width = WIDTH;
    const int image_height = HEIGHT;
    const auto aspect_ratio = static_cast<float>(image_width) / static_cast<float>(image_height);

    // Camera
    auto position = glm::vec3{-2,2, 1};
    auto lookat =   glm::vec3{ 0,0,-1};
    auto camera = Camera(
        position, // position
        lookat, // lookat
        glm::vec3{0,1,0}, // up
        50,
        aspect_ratio,
        glm::length(position-lookat),
        0.5);

    // Scene
    auto* ground =       new Lambertian(glm::vec3{ 0.8, 0.8, 0.0 });
    auto* mirror_metal = new Metal(glm::vec3{ 0.8, 0.8, 0.8 }, 0);
    auto* glossy_metal = new Metal(glm::vec3{ 0.6, 0.6, 0.2 }, 0.1);
    auto* glass =        new Dielectric(1.5);
    auto scene = Scene({
        Sphere(glm::vec3{ 0,     0, -1}, 0.5f, glossy_metal),
        Sphere(glm::vec3{ 1,     0, -1}, 0.5f, glass),
        Sphere(glm::vec3{-1,     0, -1}, 0.5f, mirror_metal),
        Sphere(glm::vec3{0, -100.5, -1},  100, ground)
    });

    // Render
    // file << "P3\n" << image_width << ' ' << image_height << "\n255\n";

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

            glm::vec3 color = ray_color(r, scene, NUM_ITERATIONS);
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

                color += ray_color(r, scene, NUM_ITERATIONS);
            }
#endif

            // write_color(file, color);
            write_color(render_target, glm::ivec2{i,image_height-j}, color);
        }
    }

    render_target.save("image.ppm");

}