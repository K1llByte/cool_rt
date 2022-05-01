#pragma once

#include "intersection.hpp"
#include "material.hpp"

struct Sphere
{
    glm::vec3      center;
    float          radius;
    abc::Material* material;

    public:
    Sphere(const glm::vec3& _center, float _radius, abc::Material* _material)
        : center(_center), radius(_radius), material(_material) {}

    bool hit(const Ray& r, double t_min, double t_max, Intersection& rec) const;
};