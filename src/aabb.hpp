#pragma once

// #include <vector>

#include "intersection.hpp"

// A box is definable with 2 positions p0 and p1
class AABB: public abc::Hittable
{
    public:
    glm::vec3 p0;
    glm::vec3 p1;

    public:
    AABB() = default;
    AABB(const glm::vec3& min, const glm::vec3& max)
        : p0(min)
        , p1(max) {}

    bool hit(const Ray& r, double t_min, double t_max, Intersection& rec) const override;
};

static AABB surrounding_box(const AABB& box0, const AABB& box1)
{
    auto small = glm::min(box0.p0, box1.p0);
    auto big = glm::min(box0.p1, box1.p1);
    return AABB(small, big);
}