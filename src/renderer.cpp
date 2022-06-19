#define ASYNC_RENDERER
// #define MULTITHREADED_RENDERER
#include "renderer.hpp"

#include <array>
#include <future>
#include <atomic>
#include <ctime>
#include <algorithm>

void Renderer::write_color(const glm::ivec2& pos, glm::vec3 color)
{
    // Write the translated [0,255] value of each color component.
    color = glm::sqrt(color / static_cast<float>(config.samples_per_pixel));
    glm::vec<3, uint8_t> pixel{
        256 * std::clamp(color.r, 0.0f, 0.999f),
        256 * std::clamp(color.g, 0.0f, 0.999f),
        256 * std::clamp(color.b, 0.0f, 0.999f)
    };

    // fmt::print("before {}, {}\n", pos.x, pos.y);
    render_target->set_pixel(pos.x, pos.y, pixel);
    // fmt::print("after\n");
}

glm::vec3 Renderer::ray_color(const Ray& r, Scene& scene, float depth)
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
    }

    // Values are normalized from [-1, 1] to [0, 1].
    auto t = 0.5f * (glm::normalize(r.direction).g + 1.0f);
    return (1.0f-t) * glm::vec3{1.0f, 1.0f, 1.0f} + t * glm::vec3{0.5f, 0.7f, 1.0f};
}

#ifdef MULTITHREADED_RENDERER
// Multithreaded render
void Renderer::render(Scene& scene, Camera& camera)
{
    const auto width = render_target->get_width();
    const auto height = render_target->get_height();

    std::array<std::thread, 4> threads;

    std::atomic_size_t progress = height-1;
    auto work = [&]() {
        do {
            size_t j = progress--;
            for(size_t i = 0; i < width; ++i)
            {
                glm::vec3 color{0,0,0};
                // Stochastic sampling
                auto min_u = (i-0.5f) / (width-1.f);
                auto max_u = (i+0.5f) / (width-1.f);
                auto min_v = (j-0.5f) / (height-1.f);
                auto max_v = (j+0.5f) / (height-1.f);

                for(size_t s = 0 ; s < config.samples_per_pixel; ++s)
                {
                    auto u = random_float(min_u, max_u);
                    auto v = random_float(min_v, max_v);

                    // Instanciate Ray
                    auto r = camera.get_ray(u, v);

                    color += ray_color(r, scene, config.num_iterations);
                }
                // Write color to render target
                write_color(glm::ivec2{i,height-j-1}, color);
            }
        } while(progress > 0);
    };

    // Start threads
    for(size_t f = 0; f < threads.size(); ++f)
    {
        threads[f] = std::thread(work);
    }

    // Control progress
    while(progress > 0)
    {
        fmt::print("\rRows remaining: {}   ", progress);
        fflush(stdout);
        sleep(0.1);
    }

    // Wait for threads to end
    for(size_t f = 0; f < threads.size(); ++f)
        threads[f].join();
}
#endif

#ifdef ASYNC_RENDERER
// Async render
void Renderer::render(Scene& scene, Camera& camera)
{
    const auto width = render_target->get_width();
    const auto height = render_target->get_height();

    std::array<std::future<void>,4> futures;
    // std::array<std::future<void>, 4> futures;

    std::atomic_size_t progress = height-1;
    std::atomic_size_t thread_id = 0;
    auto work = [&]() {
        auto random_dist = RandomDistribution();
        size_t id = thread_id++;
        do {
            size_t j = progress--;
            for(size_t i = 0; i < width; ++i)
            {
                glm::vec3 color{0,0,0};
                // Stochastic sampling
                auto min_u = (i-0.5f) / (width-1.f);
                auto max_u = (i+0.5f) / (width-1.f);
                auto min_v = (j-0.5f) / (height-1.f);
                auto max_v = (j+0.5f) / (height-1.f);

                for(size_t s = 0 ; s < config.samples_per_pixel; ++s)
                {
                    auto u = random_float(random_dist, min_u, max_u);
                    auto v = random_float(random_dist, min_v, max_v);

                    // Instanciate Ray
                    auto r = camera.get_ray(u, v);

                    color += ray_color(r, scene, config.num_iterations);
                }
                // Write color to render target
                write_color(glm::ivec2{i,height-j-1}, color);
            }
        } while(progress > 0);
    };

    // Start jobs
    for(size_t f = 0; f < futures.size(); ++f)
    {
        futures[f] = std::async(std::launch::async | std::launch::deferred, work);
    }

    // Control progress
    while(progress > 0)
    {
        fmt::print("\rRows remaining: {}   ", progress);
        fflush(stdout);
        sleep(0.1);
    }

    for(size_t f = 0; f < futures.size(); ++f)
        futures[f].wait();
}
#endif

#if !defined(MULTITHREADED_RENDERER) && !defined(ASYNC_RENDERER)
// Single threaded render
void Renderer::render(Scene& scene, Camera& camera)
{
    const auto width = render_target->get_width();
    const auto height = render_target->get_height();
    for(size_t j = 0; j < height; ++j)
    {
        for(size_t i = 0; i < width; ++i)
        {
            if((j*width + i) % 50 == 0) {
                fmt::print("\rRows remaining: {}   ", height-j-1);
                fflush(stdout);
            }
            glm::vec3 color{0,0,0};
            // Stochastic sampling
            auto min_u = (i-0.5f) / (width-1.f);
            auto max_u = (i+0.5f) / (width-1.f);
            auto min_v = (j-0.5f) / (height-1.f);
            auto max_v = (j+0.5f) / (height-1.f);

            for(size_t s = 0 ; s < config.samples_per_pixel; ++s)
            {
                auto u = random_float(min_u, max_u);
                auto v = random_float(min_v, max_v);

                // Instanciate Ray
                auto r = camera.get_ray(u, v);

                color += ray_color(r, scene, config.num_iterations);
            }
            // Write color to render target
            write_color(glm::ivec2{i,height-j-1}, color);
        }
    }
}
#endif