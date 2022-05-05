#include "renderer.hpp"

#include <array>
#include <future>
#include <atomic>
#include <ctime>

void Renderer::write_color(const glm::ivec2& pos, glm::vec3 color)
{
    // Write the translated [0,255] value of each color component.
    color = glm::sqrt(color / static_cast<float>(config.samples_per_pixel));
    glm::vec<3, uint8_t> pixel{
        256 * std::clamp(color.r, 0.0f, 0.999f),
        256 * std::clamp(color.g, 0.0f, 0.999f),
        256 * std::clamp(color.b, 0.0f, 0.999f)
    };

    fmt::print("before {}, {}\n", pos.x, pos.y);
    render_target->set_pixel(pos.x, pos.y, pixel);
    fmt::print("after\n");
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

// Multithreaded render
void Renderer::render(Scene& scene, Camera& camera)
{
    const auto width = render_target->get_width();
    const auto height = render_target->get_height();

    std::array<std::thread,4> threads;

    std::atomic_size_t progress = height;
    auto work = [&](size_t from, size_t to) {
        for(size_t j = from; j < to; ++j)
        {
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
            --progress;
        }
    };

    const size_t lines_per_worker = height / threads.size();
    for(size_t f = 0; f < threads.size(); ++f)
    {
        auto from = f * lines_per_worker;
        auto to = (f == threads.size()-1)
            ? height
            : (f+1) * lines_per_worker;
        threads[f] = std::thread(work, from, to);
    }

    while(progress > 0)
    {
        fmt::print("\rRows remaining: {}   ", progress);
        fflush(stdout);
        sleep(0.1);
    }

    for(size_t f = 0; f < threads.size(); ++f)
        threads[f].join();
}

// // Async render
// void Renderer::render(Scene& scene, Camera& camera)
// {
//     const auto width = render_target->get_width();
//     const auto height = render_target->get_height();

//     std::array<std::future<void>,4> futures;
//     std::mutex mutex;
//     std::condition_variable cv_results;

//     std::atomic_size_t progress = height;
//     auto work = [&](size_t from, size_t to) {
//         for(size_t j = from; j < to; ++j)
//         {
//             for(size_t i = 0; i < width; ++i)
//             {
//                 glm::vec3 color{0,0,0};
//                 // Stochastic sampling
//                 auto min_u = (i-0.5f) / (width-1.f);
//                 auto max_u = (i+0.5f) / (width-1.f);
//                 auto min_v = (j-0.5f) / (height-1.f);
//                 auto max_v = (j+0.5f) / (height-1.f);

//                 for(size_t s = 0 ; s < config.samples_per_pixel; ++s)
//                 {
//                     auto u = random_float(min_u, max_u);
//                     auto v = random_float(min_v, max_v);

//                     // Instanciate Ray
//                     auto r = camera.get_ray(u, v);

//                     color += ray_color(r, scene, config.num_iterations);
//                 }
//                 // Write color to render target
//                 write_color(glm::ivec2{i,height-j-1}, color);
//             }
//             --progress;
//         }
//     };

//     const size_t lines_per_worker = height / futures.size();
//     for(size_t f = 0; f < futures.size(); ++f)
//     {
//         auto from = f * lines_per_worker;
//         auto to = (f == futures.size()-1)
//             ? height
//             : (f+1) * lines_per_worker;

//         // fmt::print("from: {}, to: {}\n", from, to);
//         std::lock_guard<std::mutex> lock(mutex);
//         futures[f] = std::async(std::launch::async | std::launch::deferred, work, from, to);
//     }

//     // std::unique_lock<std::mutex> lock(mutex);
//     // cv_results.wait(lock, [&] {
//     //     // fmt::print("\rRows remaining: {}   ", progress);
//     //     // fflush(stdout);
//     //     return progress == height;
//     // });

//     while(progress > 0)
//     {
//         fmt::print("\rRows remaining: {}   ", progress);
//         fflush(stdout);
//         sleep(1.0);
//     }

//     for(size_t f = 0; f < futures.size(); ++f)
//         futures[f].wait();
// }

// // Single threaded render
// void Renderer::render(Scene& scene, Camera& camera)
// {
//     const auto width = render_target->get_width();
//     const auto height = render_target->get_height();
//     for(size_t j = 0; j < height; ++j)
//     {
//         fmt::print("\rRows remaining: {}   ", height-j-1);
//         fflush(stdout);
//         for(size_t i = 0; i < width; ++i)
//         {
//             glm::vec3 color{0,0,0};
//             // Stochastic sampling
//             auto min_u = (i-0.5f) / (width-1.f);
//             auto max_u = (i+0.5f) / (width-1.f);
//             auto min_v = (j-0.5f) / (height-1.f);
//             auto max_v = (j+0.5f) / (height-1.f);

//             for(size_t s = 0 ; s < config.samples_per_pixel; ++s)
//             {
//                 auto u = random_float(min_u, max_u);
//                 auto v = random_float(min_v, max_v);

//                 // Instanciate Ray
//                 auto r = camera.get_ray(u, v);

//                 color += ray_color(r, scene, config.num_iterations);
//             }
//             // Write color to render target
//             write_color(render_target, glm::ivec2{i,height-j-1}, color);
//         }
//     }
// }