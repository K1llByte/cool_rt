#pragma once

#include <string_view>
#include <cstdlib>

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

    void set_pixel(size_t x, size_t y, glm::ivec3 value)
    {
        data[y*width + x] = value;
    }

    glm::ivec3 get_pixel(size_t x, size_t y)
    {
        return data[y*width + x];
    }
    void save(const std::string_view& filename)
    {
        // Open file
        std::ofstream file(filename.data());
        if(not file.is_open())
        {
            std::cerr << "Error: Cannot open file\n";
            // TODO: Throw exception
            return;
            // exit(1);
        }

        // Write info data
        file << "P3\n" << width << ' ' << height << "\n255\n";

        // Write pixels
        for(size_t i = 0; i < width*height; ++i)
        {
            file << data[i].r << ' ' << data[i].g << ' ' << data[i].b << '\n';
        }
        file.close();
    }
};