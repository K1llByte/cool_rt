#include "renderer.hpp"
#include "time.hpp"

#include <thread>

#define FAST_RENDER

#ifdef FAST_RENDER
#   define WIDTH  400
#   define HEIGHT 300
#   define SAMPLES_PER_PIXEL 10
#   define NUM_ITERATIONS 50
#else
#   define WIDTH  1920
#   define HEIGHT 1080
#   define SAMPLES_PER_PIXEL 100
#   define NUM_ITERATIONS 50
#endif

/////////////////// Render to window ///////////////////

// int main()
// {
//     // Image
//     // auto window = Image(WIDTH, HEIGHT);
//     auto window = GuiWindow(WIDTH, HEIGHT);
//     // Renderer
//     auto config = RayTracerConfig {
//         .samples_per_pixel = SAMPLES_PER_PIXEL,
//         .num_iterations = NUM_ITERATIONS,
//     };
//     auto renderer = Renderer(&window, config);

//     // Camera
//     auto position = glm::vec3{ 0,0, 2};
//     auto lookat =   glm::vec3{ 0,0,-1};
//     auto camera = Camera(
//         position, // position
//         lookat, // lookat
//         glm::vec3{0,1,0}, // up
//         50,
//         window.aspect_ratio());

//     camera.set_defocus_blur(glm::length(position-lookat), 0.2);

//     // Scene
//     auto* ground =       new Lambertian(glm::vec3{ 0.0, 0.5, 0.5 });
//     auto* mirror_metal = new Metal(glm::vec3{ 0.8, 0.8, 0.8 }, 0);
//     auto* glossy_metal = new Metal(glm::vec3{ 0.6, 0.6, 0.2 }, 0.4);
//     auto* glass =        new Dielectric(1.5);
//     auto scene = Scene({
//         Sphere(glm::vec3{ 1.2,      0, -1}, 0.5f, mirror_metal),
//         Sphere(glm::vec3{   0,      0, -1}, 0.5f, glossy_metal),
//         Sphere(glm::vec3{-1.2,      0, -1}, 0.5f, glass),
//         Sphere(glm::vec3{   0, -100.5, -1},  100, ground)
//     });

//     // Launch RayTracer in separate thread
//     std::thread a([&]{
//         // Render scene with camera
//         Time::delta();
//         renderer.render(scene, camera);
//         auto time = Time::delta();
//         fmt::print("\nTook: {} secs", time);
//     });

//     // Render in a loop
//     while(!window.should_close())
//     {
//         window.present();
//     }
//     a.join();

//     // Save render to image file
//     // render_target.save("image.png");
// }

/////////////////// Render to file ///////////////////

int main()
{
    // Image
    auto render_target = Image(WIDTH, HEIGHT);
    // Renderer
    auto config = RayTracerConfig {
        .samples_per_pixel = SAMPLES_PER_PIXEL,
        .num_iterations = NUM_ITERATIONS,
    };
    auto renderer = Renderer(&render_target, config);

    // Camera
    auto position = glm::vec3{ 0,0, 2};
    auto lookat =   glm::vec3{ 0,0,-1};
    auto camera = Camera(
        position, // position
        lookat, // lookat
        glm::vec3{0,1,0}, // up
        50,
        render_target.aspect_ratio(),
        0.f,
        1.f);

    camera.set_defocus_blur(glm::length(position-lookat), 0.2);

    // Scene
    auto* ground =       new Lambertian(glm::vec3{ 0.0, 0.5, 0.5 });
    auto* mirror_metal = new Metal(glm::vec3{ 0.8, 0.8, 0.8 }, 0);
    auto* glossy_metal = new Metal(glm::vec3{ 0.6, 0.6, 0.2 }, 0.4);
    auto* glass =        new Dielectric(1.5);
    auto scene = Scene({
        // new Sphere(glm::vec3{ 1.2, 0, -1}, 0.5f, mirror_metal),
        new AnimatedSphere(
            glm::vec3{ 1.2, 0, -1},
            glm::vec3{ 1.2, 0.5, -1},
            0.f,
            1.f,
            0.5f,
            mirror_metal),
        new Sphere(glm::vec3{   0,      0, -1}, 0.5f, glossy_metal),
        new Sphere(glm::vec3{-1.2,      0, -1}, 0.5f, glass),
        new Sphere(glm::vec3{   0, -100.5, -1},  100, ground)
    });

    Time::delta();
    renderer.render(scene, camera);
    auto time = Time::delta();
    fmt::print("\nTook: {} secs", time);

    // Save render to image file
    render_target.save("image.png");
}

///////////////////////////////////////////////////////////

// struct Deedee
// {
//     virtual void deedee() = 0;
// };

// struct Doodoo
// {
//     virtual void doodoo() = 0;
// };

// template<typename T>
//     requires std::is_base_of<Deedee, T>::value
//         && std::is_base_of<Doodoo, T>::value 
// struct Foo
// {
//     T* inner;
// };


// struct Deedoo:
//     public Deedee,
//     public Doodoo
// {
//     void deedee() override { fmt::print("deedee"); }
//     void doodoo() override { fmt::print("doodoo"); }
// };

// int main() {
//     Foo<Deedoo> foo;
//     foo.inner->deedee();
// }