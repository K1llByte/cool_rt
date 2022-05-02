#pragma once

#include <chrono>

class Time
{
    public:
    // Gives time in milliseconds
    static inline uint64_t now()
    {
        return std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::system_clock::now().time_since_epoch()
        ).count();
    }

    // Gives delta time since last call in seconds
    static inline double delta()
    {
        static auto begin = std::chrono::high_resolution_clock::now();
        auto end = std::chrono::high_resolution_clock::now();
        auto seconds = std::chrono::duration_cast<std::chrono::duration<double>>(
            end - begin
        ).count();
        begin = end;
        return seconds;
    }
};