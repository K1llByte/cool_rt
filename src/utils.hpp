#pragma once

#include <random>

#include <glm/fwd.hpp>
#include <glm/gtx/norm.hpp>

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

struct RandomDistribution {
    std::uniform_real_distribution<float> distribution;
    std::mt19937                          generator;
    RandomDistribution()
        : distribution(0.f, 1.f) {}
};

inline float random_float(RandomDistribution& rd, float min, float max) {
    return rd.distribution(rd.generator)*(max - min) + min;
}

inline glm::vec3 random_vec3(float min, float max)
{
    return glm::vec3{random_float(min,max), random_float(min,max), random_float(min,max)};
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

inline glm::vec3 random_in_unit_sphere()
{
    // TODO: Improve this algorithms
    // auto radius = random_float(0, 1);
    // auto theta = random_float(0, glm::radians(360.f));
    // auto psi = random_float(glm::radians(-90.f), glm::radians(90.f));
    // return to_cartesian(glm::vec3{radius, theta, psi});
    while(true)
    {
        auto p = random_vec3(-1,1);
        if(glm::length2(p) >= 1)
            continue;
        return p;
    } 
}

inline glm::vec3 random_in_unit_disk()
{
    // TODO: Improve this algorithms
    while(true)
    {
        auto p = glm::vec3{random_float(-1,1), random_float(-1,1), 0};
        if(glm::length2(p) >= 1)
            continue;
        return p;
    }
}

inline bool near_zero(glm::vec3 v)
{
    // Return true if the vector is close to zero in all dimensions.
    const auto s = 1e-8;
    return (fabs(v.x) < s) && (fabs(v.y) < s) && (fabs(v.z) < s);
}

// Get the probability of reflectance acording to Schlick Approximation
// This takes into account the incidence angle and the refraction ratio
// and computes a probability
inline float reflectance(float cosine, float refraction_ratio)
{
    // Use Schlick's approximation for reflectance.
    auto r0 = (1-refraction_ratio) / (1+refraction_ratio);
    r0 *= r0;
    return r0 + (1-r0)*pow((1 - cosine),5);
}