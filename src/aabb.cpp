#include "aabb.hpp"

bool AABB::hit(const Ray& r, double t_min, double t_max, Intersection& rec) const
{
    // Intersect aabb
    // auto t0 = glm::min(
    //     (p0 - r.origin) / r.direction,
    //     (p1 - r.origin) / r.direction);
    // auto t1 = glm::max(
    //     (p0 - r.origin) / r.direction,
    //     (p1 - r.origin) / r.direction);
    // auto v_min = glm::max(t0, glm::vec3{t_min});
    // auto v_max = glm::min(t1, glm::vec3{t_min});
    // return not glm::any(glm::lessThanEqual(v_max,v_min));

    // Optimized AABB Intersect Method (Andrew Kensler)

    auto inv_d = 1.0f / r.direction;
    auto t0 = (p0 - r.origin) * inv_d;
    auto t1 = (p1 - r.origin) * inv_d;

    for(size_t i = 0; i < 3; ++i) {
        if(inv_d[i] < 0.0f) {
            std::swap(t0[i], t1[i]);
        }
    }

    auto v_min = glm::max(t0, glm::vec3{t_min});
    auto v_max = glm::min(t1, glm::vec3{t_max});
    return not glm::any(glm::lessThanEqual(v_max,v_min));
}