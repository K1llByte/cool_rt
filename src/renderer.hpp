#pragma once

#include "image.hpp"
#include "window.hpp"
#include "utils.hpp"
#include "camera.hpp"
#include "scene.hpp"
#include "ray.hpp"

#include <variant>

#include <fmt/core.h>

struct RayTracerConfig
{
    uint32_t samples_per_pixel = 50;
    uint32_t num_iterations    = 50;
};

class Renderer
{
    public:
    abc::RenderTarget*    render_target;
    RayTracerConfig  config;

    public:
    Renderer(abc::RenderTarget* _render_target, const RayTracerConfig& _config = {})
        : render_target(_render_target)
        , config(_config) {}

    void render(Scene& scene, Camera& camera);
    glm::vec3 ray_color(const Ray& r, Scene& scene, float depth);
    void write_color(const glm::ivec2& pos, glm::vec3 color);
};