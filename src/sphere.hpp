#pragma once

#include "intersection.hpp"

struct Sphere
{
    glm::vec3 center;
    float     radius;

    public:
    Sphere(const glm::vec3& _center, float _radius)
        : center(_center), radius(_radius) {}

    bool hit(const Ray& r, double t_min, double t_max, Intersection& rec) const;
};