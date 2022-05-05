#pragma once

#include <cstdint>

#include <glm/fwd.hpp>
#include <fmt/core.h>

namespace abc
{
    struct RenderTarget
    {
        virtual constexpr void set_pixel(size_t x, size_t y, glm::vec<3,uint8_t> value) = 0;
        virtual constexpr glm::vec<3,uint8_t> get_pixel(size_t x, size_t y) const = 0;
        virtual constexpr size_t get_width() const = 0;
        virtual constexpr size_t get_height() const = 0;
        constexpr float aspect_ratio() const { return float(get_width()) / float(get_height()); }
    };
}