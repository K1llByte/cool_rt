#pragma once

#include "render_target.hpp"

#include <string_view>
#include <cstdlib>

#include <glm/glm.hpp>

class GuiWindow;

class Image: public abc::RenderTarget
{
    friend class GuiWindow;

    private:
    size_t               width;
    size_t               height;
    glm::vec<3,uint8_t>* data;

    public:
    Image(size_t _width, size_t _height)
        : width(_width)
        , height(_height)
        , data(reinterpret_cast<glm::vec<3,uint8_t>*>(calloc(_width*_height, sizeof(glm::vec<3,uint8_t>))))
    {
        // fmt::print("Image({},{})\n", width, height);
        for(size_t i = 0; i < width*height; ++i)
            data[i] = glm::vec<3,uint8_t>{255,0,0};
        // fmt::print("First val {}\n", data[0].r);
    }

    constexpr void set_pixel(size_t x, size_t y, glm::vec<3,uint8_t> value) override
    {
        data[y*width + x] = value;
    }

    constexpr glm::vec<3,uint8_t> get_pixel(size_t x, size_t y) const override
    {
        return data[y*width + x];
    }

    constexpr size_t get_width() const override { return width; }
    constexpr size_t get_height() const override { return height; }

    void save(const std::string_view& filename);
    
};