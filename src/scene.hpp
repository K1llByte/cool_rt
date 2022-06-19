#pragma once

#include <vector>

#include "sphere.hpp"
#include "intersection.hpp"

class Scene: public abc::Hittable
{
    private:
    std::vector<abc::Hittable*> models;
    
    public:
    Scene(const std::vector<abc::Hittable*>& _models)
        : models(_models) {}

    bool hit(const Ray& r, double t_min, double t_max, Intersection& rec) const;
};