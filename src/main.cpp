#include "renderer.hpp"

#define FAST_RENDER

#ifdef FAST_RENDER
#   define WIDTH  400
#   define HEIGHT 300
#   define SAMPLES_PER_PIXEL 50
#   define NUM_ITERATIONS 50
#else
#   define WIDTH  1600
#   define HEIGHT  900
#   define SAMPLES_PER_PIXEL 200
#   define NUM_ITERATIONS 50
#endif

int main()
{
    // Image
    auto render_target = Image(WIDTH, HEIGHT);
    // Renderer
    auto config = RayTracerConfig {
        .samples_per_pixel = SAMPLES_PER_PIXEL,
        .num_iterations = NUM_ITERATIONS,
    };
    auto renderer = Renderer(render_target, config);

    // Camera
    auto position = glm::vec3{-2,2, 1};
    auto lookat =   glm::vec3{ 0,0,-1};
    auto camera = Camera(
        position, // position
        lookat, // lookat
        glm::vec3{0,1,0}, // up
        50,
        render_target.aspect_ratio(),
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

    // Render scene with camera
    renderer.render(scene, camera);

    // Save render to image file
    render_target.save("image.ppm");
}