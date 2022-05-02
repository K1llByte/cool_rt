#pragma once

#include <string_view>
#include <cstdlib>

#include <glm/glm.hpp>

class Image
{
    public:
    size_t               width;
    size_t               height;
    glm::vec<3,uint8_t>* data;

    public:
    Image(size_t _width, size_t _height)
        : width(_width)
        , height(_height)
        , data(reinterpret_cast<glm::vec<3,uint8_t>*>(calloc(_width*_height, sizeof(glm::vec<3,uint8_t>)))) {}

    constexpr void set_pixel(size_t x, size_t y, glm::vec<3,uint8_t> value)
    {
        data[y*width + x] = value;
    }

    constexpr glm::vec<3,uint8_t> get_pixel(size_t x, size_t y)
    {
        return data[y*width + x];
    }

    constexpr size_t get_width() const { return width; }
    constexpr size_t get_height() const { return height; }
    constexpr float aspect_ratio() const { return float(width) / float(height); }

    void save(const std::string_view& filename);
    
};