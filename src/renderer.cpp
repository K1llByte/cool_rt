#include "renderer.hpp"

void Renderer::write_color(Image& out, const glm::ivec2& pos, glm::vec3 color)
{
    // Write the translated [0,255] value of each color component.
    color = glm::sqrt(color / static_cast<float>(config.samples_per_pixel));
    glm::ivec3 pixel{
        256 * std::clamp(color.r, 0.0f, 0.999f),
        256 * std::clamp(color.g, 0.0f, 0.999f),
        256 * std::clamp(color.b, 0.0f, 0.999f)
    };

    out.set_pixel(pos.x, pos.y, pixel);
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
        // glm::vec3 target = intersect.point + intersect.normal + glm::normalize(random_in_unit_sphere());
        // return /* 0.5f */ color * ray_color(Ray{intersect.point, target - intersect.point}, scene, depth-1);
    }

    // Values are normalized from [-1, 1] to [0, 1].
    auto t = 0.5f * (glm::normalize(r.direction).g + 1.0f);
    return (1.0f-t) * glm::vec3{1.0f, 1.0f, 1.0f} + t * glm::vec3{0.5f, 0.7f, 1.0f};
}

void Renderer::render(Scene& scene, Camera& camera)
{
    const auto width = render_target.get_width();
    const auto height = render_target.get_height();
    for(size_t j = 0; j < height; ++j)
    {
        fmt::print("\rRows remaining: {}   ", height-j-1);
        fflush(stdout);
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
            write_color(render_target, glm::ivec2{i,height-j-1}, color);
        }
    }
    fmt::print("\n");
}