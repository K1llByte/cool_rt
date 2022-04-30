#pragma once

#include <random>

#include <glm/fwd.hpp>

const float infinity = std::numeric_limits<float>::infinity();

inline float random_float() {
    static std::uniform_real_distribution<float> distribution(0.0, 1.0);
    static std::mt19937 generator;
    return distribution(generator);
}

inline float random_float(float min, float max) {
    static std::uniform_real_distribution<float> distribution(0.f, 1.f);
    static std::mt19937 generator;
    return distribution(generator)*(max - min) + min;
}

inline glm::vec3 random_vec3(float min, float max)
{
    return glm::vec3{random_float(min,max), random_float(min,max), random_float(min,max)};
}

inline glm::vec3 random_in_unit_sphere()
{
    while(true)
    {
        auto p = random_vec3(-1,1);
        if(glm::length2(p) >= 1)
            continue;
        return p;
    } 
}

constexpr glm::vec3 to_cartesian(const glm::vec3& v)
{
    const float s_p = v.x;
    const float s_theta = v.y;
    const float s_phi = v.z;

    return glm::vec3{
        sin(s_theta) * sin(s_phi) * s_p,
        cos(s_phi) * s_p,
        sin(s_phi) * s_p * cos(s_theta)
    };
}