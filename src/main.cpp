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

#define WIDTH  1600 // 400 
#define HEIGHT  900 // 300 
#define SAMPLES_PER_PIXEL 200
#define NUM_ITERATIONS 50

void write_color(std::ostream &out, glm::vec3 color)
{
    // glm::radians(360);
    // glm::radians(180);

    // Write the translated [0,255] value of each color component.
    color = glm::sqrt(color / static_cast<float>(SAMPLES_PER_PIXEL));
    glm::ivec3 res{
        256 * std::clamp(color.r, 0.0f, 0.999f),
        256 * std::clamp(color.g, 0.0f, 0.999f),
        256 * std::clamp(color.b, 0.0f, 0.999f)
    };
    out << res.r << ' ' << res.g << ' ' << res.b << '\n';
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

int main() {

    // Render Target
    std::ofstream file("image.ppm");
    if(not file.is_open())
    {
        std::cerr << "Error: Cannot open file\n";
        return 1;
    }

    // Image

    const int image_width = WIDTH;
    const int image_height = HEIGHT;
    const auto aspect_ratio = static_cast<float>(image_width) / static_cast<float>(image_height);

    // Camera
    auto camera = Camera(aspect_ratio);

    // Scene
    auto* material_ground = new Lambertian(glm::vec3{ 0.8, 0.8, 0.0 });
    auto* material_center = new Lambertian(glm::vec3{ 0.7, 0.3, 0.3 });
    auto* material_right = new Metal(glm::vec3{ 0.8, 0.8, 0.8 });
    auto* material_left = new Metal(glm::vec3{ 0.8, 0.6, 0.2 }, 1);
    auto* material_dialetric = new Dielectric(1.5);
    auto scene = Scene({
        Sphere(glm::vec3{ 0,     0, -1}, 0.5f, material_dialetric),
        Sphere(glm::vec3{ 1,     0, -1}, 0.5f, material_right),
        Sphere(glm::vec3{-1,     0, -1}, 0.5f, material_left),
        Sphere(glm::vec3{0, -100.5, -1},  100, material_ground)
    });
    
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

            write_color(file, color);
        }
    }

    file.close();
}