#pragma once

#include "intersection.hpp"
#include "material.hpp"

struct Sphere: public abc::Hittable
{
    glm::vec3      center;
    float          radius;
    abc::Material* material;

    public:
    Sphere(const glm::vec3& _center, float _radius, abc::Material* _material)
        : center(_center), radius(_radius), material(_material) {}

    bool hit(const Ray& r, double t_min, double t_max, Intersection& rec) const;
};

struct AnimatedSphere: public abc::Hittable
{
    glm::vec3      center0, center1;
    float          time0, time1;
    float          radius;
    abc::Material* material;

    public:
    AnimatedSphere(
        const glm::vec3& _center0,
        const glm::vec3& _center1,
        float _time0,
        float _time1,
        float _radius,
        abc::Material* _material)
        : center0(_center0)
        , center1(_center1)
        , time0(_time0)
        , time1(_time1)
        , radius(_radius)
        , material(_material) {}

    bool hit(const Ray& r, double t_min, double t_max, Intersection& rec) const;
    glm::vec3 center(float time) const;
};