#pragma once

#include <vector>

#include "sphere.hpp"

class Scene:
    public abc::Hittable
{
    private:
    std::vector<abc::Hittable*> models;
    
    public:
    Scene(const std::vector<abc::SceneObject*>& _models)
        : models(_models) {}

    bool hit(const Ray& r, double t_min, double t_max, Intersection& rec) const override;
    bool make_bounding_box(float _time0, float _time1, AABB& output_box) const override;
};