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

class Dielectric: public abc::Material
{
    public:
    // Index of Refraction
    float ir;

    public:
    Dielectric(float _ir)
        : ir(_ir) {}

    virtual bool scatter(
            const Ray& r_in, const Intersection& rec, glm::vec3& attenuation, Ray& scattered) const
    {
        // Transparent is white
        attenuation = glm::vec3{1.0, 1.0, 1.0};
        // Refraction ratio order will change depending on
        // the material we're hitting from
        float refraction_ratio = rec.front_face ? (1.0/ir) : ir;
        glm::vec3 unit_direction = glm::normalize(r_in.direction);
        const float cos_theta = fmin(dot(-unit_direction, rec.normal), 1.0);
        const float sin_theta = sqrt(1.0 - cos_theta*cos_theta);

        glm::vec3 scatter_direction;
        if(refraction_ratio * sin_theta > 1.f
            || reflectance(cos_theta, refraction_ratio) > random_float())
        {
            // Reflect
            scatter_direction = glm::reflect(unit_direction, rec.normal);
        }
        else {
            // Refract
            scatter_direction = glm::refract(unit_direction, rec.normal, refraction_ratio);
        }

        // glm::vec3 refracted = glm::refract(unit_direction, rec.normal, refraction_ratio);

        scattered = Ray{rec.point, scatter_direction};
        return true;
    }
};