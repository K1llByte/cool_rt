#include "image.hpp"

#include <iostream>
#include <fstream>
#include <filesystem>

#include <fmt/core.h>

#include "TinyPngOut.hpp"

void Image::save(const std::string_view& filename)
{
    std::filesystem::path file_path(filename.data());
    if(file_path.extension() == ".png")
    {
        try {
            std::ofstream out(filename.data(), std::ios::binary);
            TinyPngOut pngout(static_cast<uint32_t>(width), static_cast<uint32_t>(height), out);
            pngout.write(reinterpret_cast<uint8_t*>(data), static_cast<size_t>(width * height));
        }
        catch(const char *msg) {
            std::cerr << msg << std::endl;
            return;
        }
    }
    else
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
            file << static_cast<uint32_t>(data[i].r) << ' '
                << static_cast<uint32_t>(data[i].g) << ' '
                << static_cast<uint32_t>(data[i].b) << '\n';
        }
        file.close();
    }
}