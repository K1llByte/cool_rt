#include "sphere.hpp"

#include <glm/gtx/norm.hpp>

bool Sphere::hit(const Ray& r, double t_min, double t_max, Intersection& rec) const
{
    // Compute discriminant
    glm::vec3 oc = r.origin - center;
    auto a = glm::length2(r.direction);
    auto half_b = glm::dot(oc, r.direction);
    auto c = glm::length2(oc) - radius*radius;

    auto discriminant = half_b*half_b - a*c;
    // If discriminant is positive there's 2 solutions (two possible intersections)
    // If discriminant is zero there's 1 solutions (intersection)
    // If discriminant is negative there's 0 solutions (no intersection)
    if(discriminant < 0)
        return false;
    const auto sqrtd = sqrt(discriminant);

    // Find the nearest root that lies in the acceptable range.
    auto root = (-half_b - sqrtd) / a;
    if(root < t_min || t_max < root)
    {
        root = (-half_b + sqrtd) / a;
        if (root < t_min || t_max < root)
            return false;
    }

    rec.distance = root;
    rec.point = r.at(root);
    // NOTE: Not sure what's faster, this current code,
    // or glm::normalize
    auto outward_normal = (rec.point - center) / radius;
    rec.set_face_normal(r, outward_normal);
    rec.material = material;

    return true;
}

bool Sphere::make_bounding_box(float time0, float time1, AABB& output_box) const
{
    output_box = AABB(
        center - glm::vec3{radius, radius, radius},
        center + glm::vec3{radius, radius, radius});
    return true;
}


bool AnimatedSphere::hit(const Ray& r, double t_min, double t_max, Intersection& rec) const
{
    // Compute discriminant
    glm::vec3 oc = r.origin - center(r.time);
    auto a = glm::length2(r.direction);
    auto half_b = glm::dot(oc, r.direction);
    auto c = glm::length2(oc) - radius*radius;

    auto discriminant = half_b*half_b - a*c;
    // If discriminant is positive there's 2 solutions (two possible intersections)
    // If discriminant is zero there's 1 solutions (intersection)
    // If discriminant is negative there's 0 solutions (no intersection)
    if(discriminant < 0)
        return false;
    const auto sqrtd = sqrt(discriminant);

    // Find the nearest root that lies in the acceptable range.
    auto root = (-half_b - sqrtd) / a;
    if(root < t_min || t_max < root)
    {
        root = (-half_b + sqrtd) / a;
        if (root < t_min || t_max < root)
            return false;
    }

    rec.distance = root;
    rec.point = r.at(root);
    // NOTE: Not sure what's faster, this current code,
    // or glm::normalize
    auto outward_normal = (rec.point - center(r.time)) / radius;
    rec.set_face_normal(r, outward_normal);
    rec.material = material;

    return true;
}

bool AnimatedSphere::make_bounding_box(float _time0, float _time1, AABB& output_box) const
{
    // Temporary bounding boxing to compute the big box
    // surrounding all possible positions from the animation
    AABB box0(
        center(_time0) - glm::vec3{radius},
        center(_time0) + glm::vec3{radius});
    AABB box1(
        center(_time1) - glm::vec3{radius},
        center(_time1) + glm::vec3{radius});
    output_box = surrounding_box(box0, box1);
    return true;
}

glm::vec3 AnimatedSphere::center(float time) const
{
    return center0 + ((time - time0) / (time1 - time0))*(center1 - center0);
}