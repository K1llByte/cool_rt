#pragma once

#include <glm/fwd.hpp>

#include "ray.hpp"

struct Intersection
{
    // Point of intersection
    glm::vec3 point;
    // Normal of this intersection point
    glm::vec3      normal;
    // Distance of the ray origin to intersection
    float     distance;
    // Intersection with a front face 
    bool      front_face;

    inline void set_face_normal(const Ray& r, const glm::vec3& outward_normal)
    {
        front_face = dot(r.direction, outward_normal) < 0;
        normal = front_face ? outward_normal : -outward_normal;
    }
};

// Compile time concept for hittable objects
template<typename T>
concept Hittable = requires(T a, const Ray& r, double t_min, double t_max, Intersection& rec)
{
    { a.hit(r, t_min, t_max, rec) };
};

// Interface to force the implementation of the hit method
namespace abc
{
    struct Hittable
    {
        virtual bool hit(const Ray&, double, double, Intersection&) const = 0;
    };
};