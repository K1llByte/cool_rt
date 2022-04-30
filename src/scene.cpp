#include "scene.hpp"

bool Scene::hit(const Ray& r, double t_min, double t_max, Intersection& rec) const
{
    Intersection tmp_rec;
    bool hit_anything = false;
    auto closest_so_far = t_max;

    for(const auto& model : models) {
        if (model.hit(r, t_min, closest_so_far, tmp_rec)) {
            hit_anything = true;
            closest_so_far = tmp_rec.distance;
            rec = tmp_rec;
        }
    }

    return hit_anything;
}