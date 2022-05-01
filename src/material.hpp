#pragma once

#include "ray.hpp"
#include "utils.hpp"
#include "intersection.hpp"

// Forward declaration

// Compile time concept for hittable objects
// template<typename T>
// concept Material = requires(T a, const Ray& r, const Intersection& rec, glm::vec3& attenuation, Ray& scattered)
// {
//     { a.scatter(r, rec, t_max, rec) };
// };

namespace abc
{
    class Material
    {
        public:
        virtual bool scatter(const Ray&, const Intersection&, glm::vec3& , Ray&) const = 0;
    };
}


class Lambertian: public abc::Material
{
    public:
    glm::vec3 albedo;

    public:
    Lambertian(glm::vec3 _albedo)
        : albedo(_albedo) {}

    virtual bool scatter(
            const Ray& r_in, const Intersection& rec, glm::vec3& attenuation, Ray& scattered) const
    {
        auto scatter_direction = rec.normal + glm::normalize(random_in_unit_sphere());
        // Step to avoid NaN's and invalid results
        if(near_zero(scatter_direction))
            scatter_direction = rec.normal;
        scattered = Ray{rec.point, scatter_direction};
        attenuation = albedo;
        return true;
    }
};

class Metal: public abc::Material
{
    public:
    glm::vec3 albedo;
    float     fuzziness;

    public:
    Metal(glm::vec3 _albedo, float _fuzziness = 0)
        : albedo(_albedo), fuzziness(_fuzziness < 1 ? _fuzziness : 1) {}

    virtual bool scatter(
            const Ray& r_in, const Intersection& rec, glm::vec3& attenuation, Ray& scattered) const
    {
        glm::vec3 reflected = glm::reflect(glm::normalize(r_in.direction), rec.normal);
        scattered = Ray{rec.point, reflected + fuzziness * random_in_unit_sphere()};
        attenuation = albedo;
        return (dot(scattered.direction, rec.normal) > 0);
    }
};