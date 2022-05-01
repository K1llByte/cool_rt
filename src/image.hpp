#pragma once

#include <string_view>
#include <cstdlib>

#include <glm/glm.hpp>

class Image
{
    public:
    size_t      width;
    size_t      height;
    glm::ivec3* data;

    public:
    Image(size_t _width, size_t _height)
        : width(_width)
        , height(_height)
        , data(reinterpret_cast<glm::ivec3*>(calloc(_width*_height, sizeof(glm::ivec3)))) {}

    constexpr void set_pixel(size_t x, size_t y, glm::ivec3 value)
    {
        data[y*width + x] = value;
    }

    constexpr glm::ivec3 get_pixel(size_t x, size_t y)
    {
        return data[y*width + x];
    }

    constexpr size_t get_width() const { return width; }
    constexpr size_t get_height() const { return height; }
    constexpr float aspect_ratio() const { return float(width) / float(height); }

    void save(const std::string_view& filename);
    
};