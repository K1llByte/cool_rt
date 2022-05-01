#include "image.hpp"

#include <iostream>
#include <fstream>

#include <fmt/core.h>

void Image::save(const std::string_view& filename)
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